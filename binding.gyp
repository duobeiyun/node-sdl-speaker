{
  "targets": [
    {
      "target_name": "sdl-speaker",
      "sources": [
        "src/binding.cpp",
        "src/SDLSpeaker.cpp",
        "src/SDLSpeakerWrapper.cpp",
        "src/Channel.cpp",
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
            "-framework", "Cocoa",
            "<(module_root_dir)/vendors/SDL2-2.0.5/osx/libSDL2.a"
          ],
          "include_dirs": [
            "platform-include/darwin"
          ]
        },
        "OS=='win'", {
          "libraries": [
            "<(module_root_dir)/vendors/SDL2-2.0.5/win32/x86/SDL2.lib"
          ],
          "include_dirs": [
            "<(module_root_dir)/platform-include/win32"
          ]
        }]
      ]
    }
  ]
}
