#include <libusb-1.0/libusb.h>

#include <cstdint>
#include <iostream>
#include <vector>

extern "C" {
#include "libpolhemus.h"
}
#include "libpolhemus.hpp"
#include "libpolhemus_macro.h"

using namespace polhemus;

int PLHM_ADD_PREFIX_(init)() { return libusb_init(nullptr); }

int PLHM_ADD_PREFIX_(open)(DevType dev_type, DevHandle** handle) {
    try {
        *handle = new DevHandle(dev_type);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int PLHM_ADD_PREFIX_(get_timeout)(DevHandle* handle, unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = handle->timeout();

    return 0;
}

int PLHM_ADD_PREFIX_(set_timeout)(DevHandle* handle, unsigned int timeout) {
    if (!handle) return -1;

    handle->timeout(timeout);

    return 0;
}

int PLHM_ADD_PREFIX_(send_raw)(DevHandle* handle, const Buffer* const buf) {
    if (!handle) return -1;

    return handle->send_raw(*buf);
}

int PLHM_ADD_PREFIX_(recv_raw)(DevHandle* handle, Buffer* buf) {
    if (!handle) return -1;

    return handle->recv_raw(buf);
}

int PLHM_ADD_PREFIX_(check_connection_att)(DevHandle* handle,
                                           unsigned int attempts) {
    if (!handle) return -1;

    return handle->check_connection(attempts);
}

int PLHM_ADD_PREFIX_(check_connection)(DevHandle* handle) {
    if (!handle) return -1;

    return handle->check_connection();
}

int PLHM_ADD_PREFIX_(send_cmd)(DevHandle* handle, const Buffer* const cmd,
                               Buffer* resp) {
    if (!handle) return -1;

    return handle->send_cmd(*cmd, resp);
}

void PLHM_ADD_PREFIX_(close)(DevHandle* handle) { delete handle; }

void PLHM_ADD_PREFIX_(exit)() { libusb_exit(nullptr); }

#include "libpolhemus_macro.h"
