//
// Created by Wang Siyuan on 2017/6/9.
//

#ifndef NODE_SDL_SPEAKER_SDLSPEAKER_H
#define NODE_SDL_SPEAKER_SDLSPEAKER_H
#include <nan.h>
#include <SDL2/SDL.h>
#include "TPCircularBuffer/TPCircularBuffer.h"
using v8::Isolate;
using namespace Nan;

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
    void Clean();
    const char* Init();
    int Stop();
    int Pause();
    int Resume();
    void Write(void * buf, int32_t length);
    CurrentPlayState state = stop;
    Callback *onErrorCallback;

private:
    ~SDLSpeaker();
    SpeakerOpt option;
    TPCircularBuffer pcmBuffer;
    SDL_AudioSpec wanted_spec;
};


#endif //NODE_SDL_SPEAKER_SDLSPEAKER_H
