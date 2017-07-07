#include <nan.h>
#include "SDLSpeakerWrapper.h"
using v8::String;
using v8::FunctionTemplate;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

void InitAll(v8::Local<v8::Object> exports) {
    SDLSpeakerWrapper::Init(exports);
    Set(
        exports, 
        New<String>("mix").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(SDLSpeakerWrapper::Mix)).ToLocalChecked()
    );
}

NODE_MODULE(sdl_speaker, InitAll);
