#include <napi.h>
#include <uv.h>
#include "SDLSpeakerWrapper.h"
using Napi::String;
using Napi::FunctionReference;
using Napi::GetFunction;
using Napi::New;
using Napi::Set;

void InitAll(Napi::Object exports) {
    SDLSpeakerWrapper::Init(exports);
    Set(
        exports, 
        New<String>("mix"),
        GetFunction(New<Napi::FunctionReference>(SDLSpeakerWrapper::Mix))
    );
}

NODE_API_MODULE(sdl_speaker, InitAll);
