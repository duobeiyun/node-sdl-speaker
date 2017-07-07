//
// Created by Wang Siyuan on 2017/6/9.
//

#ifndef NODE_SDL_SPEAKER_SDLSPEAKER_H
#define NODE_SDL_SPEAKER_SDLSPEAKER_H
#include <unordered_map>
#include <iostream>
#include <string>
#include <nan.h>
#include <SDL2/SDL.h>
#include "Channel.h"
#include "hlring/rbuf.h"

using v8::Isolate;
using namespace Nan;
using std::string;

struct SpeakerOpt {
    int freq;
    int channels;
    int samples;
    int format;
};

enum CurrentPlayState { stop, pause, playing };

class SDLSpeaker {
public:
    SDLSpeaker(
            int freq = 16000,
            int channels = 1,
            int samples = 320,
            Callback *onError = nullptr,
            int format = AUDIO_S16LSB
    );
    int Start();
    const char* Init();
    int Stop();
    int Pause();
    int Resume();
    int Write(void * buf, int32_t length, string channel_name);
    void Clean(string channel_name);;
    int NewChannel(string channel_name);
    int RemoveChannel(string channel_name);
    void CleanAll();
    CurrentPlayState state = stop;
    Callback *onErrorCallback;

private:
    ~SDLSpeaker();
    Channel* find_channel(string);
    SpeakerOpt option;
    SDL_AudioSpec wanted_spec;
    std::unordered_map<std::string, Channel*> channels_map;
};


#endif //NODE_SDL_SPEAKER_SDLSPEAKER_H
