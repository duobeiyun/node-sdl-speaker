//
// Created by Wang Siyuan on 2017/7/6.
//

#include "Channel.h"

int Channel::write(void *buf, int32_t length) {
    rbuf_write(buffer, static_cast<unsigned char*>(buf), length);
    return rbuf_available(buffer);
}

int Channel::clean() {
    rbuf_clear(buffer);
    return 0;
}
