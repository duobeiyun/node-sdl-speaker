{
  "targets": [
    {
      "target_name": "sdl-speaker",
      "sources": [
        "src/binding.cpp",
        "src/SDLSpeaker.cpp",
        "src/SDLSpeakerWrapper.cpp",
        "src/hlring/rbuf.c"
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
          "include_dirs": [
            "platform-include/darwin"
          ]
        },
        "OS=='win'", {
          "libraries": [
            "C:/dev/vendors/SDL2-2.0.5/lib/x86/SDL2.lib"
          ],
          "include_dirs": [
            "platform-include/win32"
          ]
        }]
      ]
    }
  ]
}