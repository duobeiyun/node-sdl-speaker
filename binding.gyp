{
  "targets": [
    {
      "target_name": "sdl-speaker",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": { "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1 }
      },
      "sources": [
        "src/node_sdl.cc",
        "src/SDLSpeaker.cpp",
        "src/Channel.cpp",
        "src/hlring/rbuf.c"
      ],
      "cflags": [ "-Wall", "-std=c++11" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "include"
      ],
      "conditions": [
        ["OS=='mac'", {
          "libraries": [
            "-framework", "Cocoa",
            "<(module_root_dir)/vendors/SDL2-2.0.5/osx/libSDL2.a"
          ],
          "include_dirs": [
            "<(module_root_dir)/include/platform-include/darwin"
          ]
        },
        "OS=='win'", {
          "libraries": [
            "<(module_root_dir)/vendors/SDL2-2.0.5/win32/x86/SDL2.lib"
          ],
          "include_dirs": [
            "<(module_root_dir)/include/platform-include/win32"
          ]
        }]
      ]
    }
  ]
}
