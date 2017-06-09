#include <nan.h>
#include "SDLSpeakerWrapper.h"

void InitAll(v8::Local<v8::Object> exports) {
    SDLSpeakerWrapper::Init(exports);
}

NODE_MODULE(sdl_speaker, InitAll);
