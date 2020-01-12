#pragma once

extern "C" {

enum libpolhemus_devtype { PATRIOT, PATRIOT_HS };

struct libpolhemus_buffer {
    unsigned char* data;
    int len;
};

} /* extern "C" */

using DevType = libpolhemus_devtype;
using Buffer = libpolhemus_buffer;
