//
// Created by Wang Siyuan on 2017/6/9.
//

#ifndef NODE_SDL_SPEAKER_SDLSPEAKER_H
#define NODE_SDL_SPEAKER_SDLSPEAKER_H
#include <unordered_map>
#include <iostream>
#include <string>
#include <napi.h>
#include <uv.h>
#include <SDL2/SDL.h>
#include "Channel.h"
#include "hlring/rbuf.h"

using namespace Napi;
using std::string;

struct SpeakerOpt {
    int freq;
    int channels;
    int samples;
    int format;
};


class SDLSpeaker {
public:
    SDLSpeaker() {
        option.freq = 44100;
        option.channels = 2;
        option.samples = 1024;
        option.format = AUDIO_S16LSB;
    };
    ~SDLSpeaker();
    enum CurrentPlayState { stop, pause, playing };
    int Start();
    const char* Init(int freq, uint32_t channels, uint32_t samples, uint32_t format);
    int Stop();
    int Pause();
    int Resume();
    int Write(void * buf, int32_t length, string channel_name);
    void Clean(string channel_name);;
    int NewChannel(string channel_name);
    int RemoveChannel(string channel_name);
    void CleanAll();
    CurrentPlayState state = stop;

private:
    Channel* find_channel(string);
    SpeakerOpt option;
    SDL_AudioSpec wanted_spec;
    std::unordered_map<std::string, Channel*> channels_map;
};


#endif //NODE_SDL_SPEAKER_SDLSPEAKER_H
