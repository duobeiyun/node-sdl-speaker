//
// Created by Wang Siyuan on 2017/6/9.
//

#include "SDLSpeaker.h"
#include <iostream>

static void fill_audio(void *udata, Uint8 *stream, int len) {
    TPCircularBuffer *buf = static_cast<TPCircularBuffer*>(udata);
    SDL_memset(stream, 0, static_cast<size_t>(len));
    int32_t size = 0;
    void *pcmFrame = TPCircularBufferTail(buf, &size);
    if (size >= len) {
        memcpy(stream, pcmFrame, static_cast<size_t >(len));
        TPCircularBufferConsume(buf, len);
    } else {
        memset(stream, 0, static_cast<size_t>(len));
    }
}

static SDL_AudioCallback audio_callback_ptr = fill_audio;

SDLSpeaker::SDLSpeaker(int freq, int channels, int samples, Callback *onError, int format) {
    option.freq = freq;
    option.channels = channels;
    option.samples = samples;
    option.format = format;
    onErrorCallback = onError;
    TPCircularBufferInit(&pcmBuffer, option.samples * 32);
}

SDLSpeaker::~SDLSpeaker() {
    SDL_CloseAudio();
    Clean();
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
    wanted_spec.userdata = &pcmBuffer;
    wanted_spec.callback = audio_callback_ptr;
    return nullptr;
}

void SDLSpeaker::Clean() {
    TPCircularBufferClear(&pcmBuffer);
}

int SDLSpeaker::Start() {
    if (state == playing) { return 0; }
    if (state != stop) { return -1; }
    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        return -2;
    }
    SDL_PauseAudio(0);
    state = playing;
    return 0;
}

void SDLSpeaker::Write(void * buf, int32_t length) {
    TPCircularBufferProduceBytes(&pcmBuffer, buf, length);
}

int SDLSpeaker::Stop() {
    if (state == stop) { return 0; }
    SDL_CloseAudio();
    Clean();
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
