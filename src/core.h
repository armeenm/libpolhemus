#pragma once

#include <cstdint>

#include "libpolhemus.h"

struct DevInfo {
    DevType dev_type;
    const char* name;
    uint16_t vid, pid;
    unsigned char write_ep, read_ep;
};

struct libpolhemus_device_handle {
    libusb_device_handle* libusb_handle;
    DevInfo info;
};
