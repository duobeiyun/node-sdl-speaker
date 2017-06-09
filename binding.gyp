{
  "targets": [
    {
      "target_name": "sdl-speaker",
      "sources": [
        "src/binding.cpp",
        "src/TPCircularBuffer/TPCircularBuffer.c",
        "src/SDLSpeaker.cpp",
        "src/SDLSpeakerWrapper.cpp"
      ],
      "cflags": [ "-Wall", "-std=c++11" ],
      "include_dirs": [
        "src/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "/usr/local/lib/libSDL2.dylib"
      ]
    }
  ]
}