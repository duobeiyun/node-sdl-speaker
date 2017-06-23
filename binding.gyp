{
  "targets": [
    {
      "target_name": "sdl-speaker",
      "sources": [
        "src/binding.cpp",
        "src/SDLSpeaker.cpp",
        "src/SDLSpeakerWrapper.cpp"
      ],
      "cflags": [ "-Wall", "-std=c++11" ],
      "include_dirs": [
        "include",
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ["OS=='mac'", {
          "libraries": [
            "/usr/local/lib/libSDL2.dylib"
          ],
          "sources": [
            "src/TPCircularBuffer/TPCircularBuffer.c",
          ]
        },
        "OS=='win'", {
          "libraries": [
            "C:/dev/vendors/SDL2-2.0.5/lib/x86/SDL2.lib"
          ],
          "sources": [
            "src/hlring/rbuf.c"
          ]
        }]
      ]
    }
  ]
}