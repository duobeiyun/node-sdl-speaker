//
// Created by Wang Siyuan on 2017/7/6.
//

#ifndef NODE_SDL_SPEAKER_CHANNEL_H
#define NODE_SDL_SPEAKER_CHANNEL_H

#include <unordered_map>
#include <string>
#include <iostream>
#include "hlring/rbuf.h"

using std::string;

class Channel {
public:
    Channel(string ch_name) {
        name = ch_name;
        buffer = rbuf_create(320 * 100);
    };
    ~Channel() {
        clean();
        rbuf_destroy(buffer);
    };
    int write(void * buf, int32_t length);
    int clean();
    string name;
    rbuf_t *buffer;
};

#endif //NODE_SDL_SPEAKER_CHANNEL_H
