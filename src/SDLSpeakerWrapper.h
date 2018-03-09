//
// Created by Wang Siyuan on 2017/6/8.
//

#ifndef NODE_SDL_SPEAKER_SDLSPEAKERWRAPPER_H
#define NODE_SDL_SPEAKER_SDLSPEAKERWRAPPER_H

#include <string>
#include <iostream>
#include <nan.h>
#include <SDL2/SDL.h>
#include <map>
#include "SDLSpeaker.h"

using v8::Isolate;
using namespace Nan;
using namespace std;


class SDLSpeakerWrapper : public ObjectWrap {
public:
    static void Init(v8::Local<v8::Object> exports);
    static NAN_METHOD(New);
    static NAN_METHOD(InitSpeaker);
    static NAN_METHOD(Write);
    static NAN_METHOD(Start);
    static NAN_METHOD(Stop);
    static NAN_METHOD(Resume);
    static NAN_METHOD(Pause);
    static NAN_METHOD(Clean);
    static NAN_METHOD(CleanAll);
    static NAN_METHOD(NewChannel);
    static NAN_METHOD(RemoveChannel);
    static NAN_METHOD(Mix);

private:
    explicit SDLSpeakerWrapper(SDLSpeaker *speaker) : speaker(speaker) {};
    ~SDLSpeakerWrapper();

    static inline Persistent<v8::Function> &constructor() {
        static Persistent<v8::Function> _constructor;
        return _constructor;
    }
    SDLSpeaker *speaker;
};

#endif //NODE_SDL_SPEAKER_SDLSPEAKERWRAPPER_H
