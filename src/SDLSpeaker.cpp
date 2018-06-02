//
// Created by Wang Siyuan on 2017/6/9.
//

#include "SDLSpeaker.h"

bool need_mix(rbuf_t *buf, int len) {
    return rbuf_used(buf) >= len;
}

static void fill_audio(void *udata, Uint8 *stream, int len) {
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

SDLSpeaker::~SDLSpeaker() {
    SDL_CloseAudio();
    CleanAll();
    state = stop;
}

const char* SDLSpeaker::Init(int freq, uint32_t channels, uint32_t samples, uint32_t format) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        const char* err = SDL_GetError();
        return err;
    }
    wanted_spec.freq = freq;
    wanted_spec.format = static_cast<SDL_AudioFormat >(format);
    wanted_spec.channels = static_cast<Uint8 >(channels);
    wanted_spec.silence = 0;
    wanted_spec.samples = static_cast<Uint16>(samples);
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
    for (auto ch = channels_map.begin(); ch != channels_map.end(); ++ch) {
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
