//
// Created by Wang Siyuan on 2017/6/8.
//

#include "SDLSpeakerWrapper.h"

SDLSpeakerWrapper::~SDLSpeakerWrapper() {}

void SDLSpeakerWrapper::Init(v8::Local<v8::Object> exports) {
    HandleScope scope;
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Speaker").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(7);

    SetPrototypeMethod(tpl, "init", InitSpeaker);
    SetPrototypeMethod(tpl, "write", Write);
    SetPrototypeMethod(tpl, "start", Start);
    SetPrototypeMethod(tpl, "stop", Stop);
    SetPrototypeMethod(tpl, "pause", Pause);
    SetPrototypeMethod(tpl, "resume", Resume);
    SetPrototypeMethod(tpl, "clean", Clean);

    constructor().Reset(GetFunction(tpl).ToLocalChecked());
    exports->Set(
            Nan::New("Speaker").ToLocalChecked(),
            GetFunction(tpl).ToLocalChecked()
    );
}

void SDLSpeakerWrapper::New(const FunctionCallbackInfo<v8::Value>& info) {
    if (info.IsConstructCall()) {
        int freq = To<int>(info[0]).FromJust();
        int channels = To<int>(info[1]).FromJust();
        int samples = To<int>(info[2]).FromJust();
        Callback *onErrorCallback = new Callback(info[3].As<v8::Function>());

        SDLSpeaker *speaker = new SDLSpeaker(freq, channels, samples, onErrorCallback, AUDIO_S16LSB);
        SDLSpeakerWrapper *wrapper = new SDLSpeakerWrapper(speaker);
        wrapper->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 5;
        v8::Local<v8::Value> argv[argc] = {info[0], info[1], info[2], info[3]};;
        v8::Local<v8::Function> cons = Nan::New(constructor());
        v8::Local<v8::Object> instance = NewInstance(cons, argc, argv).ToLocalChecked();
        info.GetReturnValue().Set(instance);
    }
}

void SDLSpeakerWrapper::InitSpeaker(const FunctionCallbackInfo<v8::Value>& info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    const char* res = wrapper->speaker->Init();
    if (res != nullptr) {
        std::string error = res;
        info.GetReturnValue().Set(
                v8::Exception::Error(Nan::New<v8::String>(error).ToLocalChecked())
        );
    }
}

void SDLSpeakerWrapper::Write(const FunctionCallbackInfo<v8::Value>& info) {
    void* buffer = node::Buffer::Data(info[0]->ToObject());
    int length = static_cast<int>(node::Buffer::Length(info[0]->ToObject()));
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    wrapper->speaker->Write(buffer, length);
}


void SDLSpeakerWrapper::Start(const FunctionCallbackInfo<v8::Value> &info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    int r = wrapper->speaker->Start();
    if (r) {
        info.GetReturnValue().Set(Nan::New(r));
    }
}


void SDLSpeakerWrapper::Stop(const FunctionCallbackInfo<v8::Value> &info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    int r = wrapper->speaker->Stop();
    if (r) {
        info.GetReturnValue().Set(Nan::New(r));
    }
}


void SDLSpeakerWrapper::Pause(const FunctionCallbackInfo<v8::Value> &info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    int r = wrapper->speaker->Pause();
    if (r) {
        info.GetReturnValue().Set(Nan::New(r));
    }
}

void SDLSpeakerWrapper::Resume(const FunctionCallbackInfo<v8::Value> &info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    int r = wrapper->speaker->Resume();
    if (r) {
        info.GetReturnValue().Set(Nan::New(r));
    }
}

void SDLSpeakerWrapper::Clean(const FunctionCallbackInfo<v8::Value> &info) {
    SDLSpeakerWrapper *wrapper = ObjectWrap::Unwrap<SDLSpeakerWrapper>(info.Holder());
    wrapper->speaker->Clean();
}
