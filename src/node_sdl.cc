#include <napi.h>
#include "Channel.h"
#include "SDLSpeaker.h"

#define EXPORT_METHOD(NAME, FUNC) \
     exports.Set(Napi::String::New(env, #NAME), Napi::Function::New(env, FUNC));

#define STATUS Napi::Number::New(info.Env(), r);

SDLSpeaker speaker;

Napi::Value InitSpeaker(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int freq = info[0].As<Napi::Number>().Int32Value();
    uint32_t channels = info[1].As<Napi::Number>().Uint32Value();
    uint32_t samples = info[2].As<Napi::Number>().Uint32Value();
    uint32_t format = info[3].As<Napi::Number>().Uint32Value();
    auto res = speaker.Init(freq, channels, samples, format);
    if (res) {
        throw Napi::Error::New(env, res);
    } else {
        return Napi::Number::New(env, 0);
    }
}

Napi::Value Write(const Napi::CallbackInfo& info) {
    void* buffer = info[1].As<Napi::Buffer<uint8_t >>().Data();
    size_t length = info[1].As<Napi::Buffer<uint8_t >>().Length();
    std::string ch_name = info[0].As<Napi::String>().ToString();
    int remain = speaker.Write(buffer, static_cast<int32_t>(length), ch_name);
    return Napi::Number::New(info.Env(), remain);
}

void Clean(const Napi::CallbackInfo& info) {
    std::string ch_name = info[0].As<Napi::String>().ToString();
    speaker.Clean(ch_name);
}

void CleanAll(const Napi::CallbackInfo& info) {
    speaker.CleanAll();
}


Napi::Value AddChannel(const Napi::CallbackInfo& info) {
    std::string ch_name = info[0].As<Napi::String>().ToString();
    int r = speaker.NewChannel(ch_name);
    return STATUS;
}


Napi::Value RemoveChannel(const Napi::CallbackInfo& info) {
    std::string ch_name = info[0].As<Napi::String>().ToString();
    int r = speaker.RemoveChannel(ch_name);
    return STATUS;
}


Napi::Value Start(const Napi::CallbackInfo& info) {
    int r = speaker.Start();
    return STATUS;
}


Napi::Value Stop(const Napi::CallbackInfo& info) {
    int r = speaker.Stop();
    return STATUS
}


Napi::Value Pause(const Napi::CallbackInfo& info) {
    int r = speaker.Pause();
    return STATUS
}

Napi::Value Resume(const Napi::CallbackInfo& info) {
    int r = speaker.Resume();
    return STATUS
}


Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    EXPORT_METHOD(init, InitSpeaker);
    EXPORT_METHOD(addChannel, AddChannel);
    EXPORT_METHOD(removeChannel, RemoveChannel);
    EXPORT_METHOD(start, Start);
    EXPORT_METHOD(stop, Stop);
    EXPORT_METHOD(pause, Pause);
    EXPORT_METHOD(resume, Resume);
    EXPORT_METHOD(write, Write);
    EXPORT_METHOD(clean, Clean);
    EXPORT_METHOD(cleanAll, CleanAll);
    return exports;
}

NODE_API_MODULE(sdl_speaker, InitAll)