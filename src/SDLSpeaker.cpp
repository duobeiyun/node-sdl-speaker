//
// Created by Wang Siyuan on 2017/6/9.
//

#include "SDLSpeaker.h"
#include <iostream>
#include "nanomsg\nn.h"
#include "nanomsg\pubsub.h"
#include <functional>
#include <thread>

/**
 * nanomsg 操作类
 */
class nanomsg_t{
private:
    int nnfd = -1;
    int nnerrno = 0;
    bool runing = true;
    std::thread readThread;
    bool isServer = false;
public:
    nanomsg_t(){};
    ~nanomsg_t(){};
    std::string url;
    int timeout = 300;
    std::function<int(void *,int)> recvCallBack = nullptr;

    void readThreadFun(){
        struct nn_pollfd pfd[1];
        pfd[0].fd = nnfd;
        pfd[0].events = NN_POLLIN;
        while (runing) {
            int rc = nn_poll(pfd, 1, timeout);
            if (rc == 0) {
                continue;
            }
            if (rc == -1) {
                break;
            }
            if (pfd[0].revents & NN_POLLIN) {
                uint8_t *msg;
                rc = nn_recv(pfd[0].fd, &msg, NN_MSG, 0);
                if (rc < 0) {
                    break;
                }
                if (recvCallBack) {
                    recvCallBack(msg, rc);
                    freeReadBuf(msg);
                } else {
                    nn_freemsg(msg);
                }

            }
        }
    }
    bool initAsPub(){
        /*  Create the socket. */
        isServer = true;
        nnfd = nn_socket (AF_SP, NN_PUB);
        if (nnfd < 0) {
            nnerrno = nn_errno ();
            return false;
        }
        if (nn_bind (nnfd, url.c_str()) < 0) {
            nnerrno = nn_errno ();
            nn_close (nnfd);
            return false;
        }
        return true;
    }
    bool initAsSub(){
        nnfd = nn_socket (AF_SP, NN_SUB);
        if (nnfd < 0) {
            nnerrno = nn_errno ();
            return false;
        }

        if (nn_connect (nnfd, url.c_str()) < 0) {
            nnerrno = nn_errno ();
            nn_close (nnfd);
            return false;
        }
        /*  We want all messages, so just subscribe to the empty value. */
        if (nn_setsockopt (nnfd, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
            nnerrno = nn_errno ();
            nn_close (nnfd);
            return false;
        }
        readThread = std::thread(std::bind(&nanomsg_t::readThreadFun,this));
        return true;
    }
    void freeReadBuf(void * msg){
        nn_freemsg(msg);
    }
    void close(){
        runing = false;
        if(readThread.joinable()){
            readThread.join();
        }
    }
    int send(const char *buf,int len){
        int rc = nn_send (nnfd, buf, len, NN_DONTWAIT);
        if (rc < 0) {
            nnerrno = nn_errno ();
            return rc;
        }
        return rc;
    }
};


bool need_mix(rbuf_t *buf, int len) {
    return rbuf_used(buf) >= len;
}

bool is_silent(unsigned char* src, int len) {
    for (int i = 0; i < len; i++) {
        if (src[i] != 0) {
            return false;
        }
    }
    return true;
}

static void fill_audio(void *udata, Uint8 *stream, int len) {
    // TODO: handle silent volume;
    auto *channels_map_ptr = static_cast<std::unordered_map<std::string, Channel*> *>(udata);
    int channel_count = 0;
    for (auto it = channels_map_ptr->begin(); it != channels_map_ptr->end(); ++it) {
        if (need_mix(it->second->buffer, len)) {
            channel_count++;
        }
    }
    auto *src = new unsigned char[len];
    SDL_memset(stream, 0, static_cast<size_t>(len));
    for (auto it = channels_map_ptr->begin(); it != channels_map_ptr->end(); ++it) {
        if (need_mix(it->second->buffer, len)) {
            rbuf_read(it->second->buffer, src, len);
            SDL_MixAudioFormat(
                    stream,
                    static_cast<const Uint8 *>(src),
                    AUDIO_S16LSB,
                    static_cast<Uint32 >(len),
                    SDL_MIX_MAXVOLUME 
            );
        }
    }
    delete[] src;
}

static SDL_AudioCallback audio_callback_ptr = fill_audio;

SDLSpeaker::SDLSpeaker(int freq, int channels, int samples, Callback *onError, int format) {
    option.freq = freq;
    option.channels = channels;
    option.samples = samples;
    option.format = format;
    onErrorCallback = onError;
}

SDLSpeaker::~SDLSpeaker() {
    SDL_CloseAudio();
    CleanAll();
    state = stop;
}

const char* SDLSpeaker::Init() {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        const char* err = SDL_GetError();
        return err;
    }
    wanted_spec.freq = option.freq;
    wanted_spec.format = static_cast<SDL_AudioFormat >(option.format);
    wanted_spec.channels = static_cast<Uint8 >(option.channels);
    wanted_spec.silence = 0;
    wanted_spec.samples = static_cast<Uint16>(option.samples);
    wanted_spec.userdata = &channels_map;
    wanted_spec.callback = audio_callback_ptr;
    return nullptr;
}

int SDLSpeaker::Start() {
    if (state == playing) { return 0; }
    if (state != stop) { return -1; }
    if (int errorCode = SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        SDL_Log("Audio Device Open failed %d: %s", errorCode, SDL_GetError());
        return -2;
    }
    SDL_PauseAudio(0);
    state = playing;
    return 0;
}

Channel* SDLSpeaker::find_channel(string name) {
    auto got = channels_map.find(name);
    if (got == channels_map.end()) {
        return nullptr;
    } else {
        return got->second;
    }
}

int SDLSpeaker::NewChannel(string channel_name) {
    auto got = channels_map.find(channel_name);
    if (got == channels_map.end()) {
        Channel* channel = new Channel(channel_name);
        channels_map[channel_name] = channel;
        
        nanomsg_t *pnanomsg = new nanomsg_t();
        pnanomsg->url = "ipc://audio-" + channel_name;
        pnanomsg->recvCallBack = std::bind(&Channel::write,channel,std::placeholders::_1,std::placeholders::_2);
        pnanomsg->initAsSub();
        nanomsg_t_map[channel_name] = pnanomsg;
        return 0;
    } else {
        return -1;
    }
}

int SDLSpeaker::RemoveChannel(string channel_name) {
    auto got = channels_map.find(channel_name);
    if (got == channels_map.end()) {
        return -1;
    } else {
    	auto nt = nanomsg_t_map.find(channel_name);
    	if(nt != nanomsg_t_map.end()){
    		((nanomsg_t*)(nt->second))->close();
    		delete nt->second;
    		nanomsg_t_map.erase(channel_name);
    	}
        delete got->second;
        channels_map.erase(channel_name);
        return 0;
    }
}

void SDLSpeaker::Clean(string channel_name) {
    Channel* ch = find_channel(channel_name);
    if (ch != nullptr) {
        ch->clean();
    }
}

void SDLSpeaker::CleanAll() {
    for (auto ch = channels_map.begin(); ch != channels_map.end();) {
        ch->second->clean();
    }
}

int SDLSpeaker::Write(void * buf, int32_t length, string channel_name) {
    Channel* ch = find_channel(channel_name);
    if (ch != nullptr) {
        return ch->write(buf, length);
    }
	return -1;
}

int SDLSpeaker::Stop() {
    if (state == stop) { return 0; }
    SDL_CloseAudio();
    CleanAll();
    state = stop;
    return 0;
}

int SDLSpeaker::Pause() {
    if (state == pause) { return 0; }
    if (state != playing) { return -1; }
    SDL_PauseAudio(1);
    state = pause;
    return 0;
}

int SDLSpeaker::Resume() {
    if (state == playing) { return 0; }
    if (state != pause) { return -1; }
    SDL_PauseAudio(0);
    state = playing;
    return 0;
}
