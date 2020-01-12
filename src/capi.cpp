#include <libusb-1.0/libusb.h>

#include <cstdint>
#include <iostream>
#include <vector>

extern "C" {
#include "libpolhemus.h"
}

#include "libpolhemus.hpp"

int libpolhemus_init() { return libusb_init(nullptr); }

int libpolhemus_open(DevType dev_type, DevHandle** handle) {
    try {
        *handle = new DevHandle(dev_type);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }
}

int libpolhemus_get_timeout(const DevHandle* const handle,
                            unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = handle->timeout();

    return 0;
}

int libpolhemus_set_timeout(DevHandle* const handle, unsigned int timeout) {
    if (!handle) return -1;

    handle->timeout(timeout);

    return 0;
}

int libpolhemus_send_raw(DevHandle* handle, const Buffer* const buf) {
    if (!handle) return -1;

    return handle->send_raw(*buf);
}

int libpolhemus_recv_raw(DevHandle* handle, Buffer* buf) {
    if (!handle) return -1;

    return handle->recv_raw(buf);
}

int libpolhemus_check_connection_att(DevHandle* handle, unsigned int attempts) {
    if (!handle) return -1;

    return handle->check_connection(attempts);
}

int libpolhemus_check_connection(DevHandle* handle) {
    if (!handle) return -1;

    return handle->check_connection();
}

int libpolhemus_send_cmd(DevHandle* handle, const Buffer* const cmd,
                         Buffer* resp) {
    if (!handle) return -1;

    return handle->send_cmd(*cmd, resp);
}

void libpolhemus_close(DevHandle* handle) {
    if (handle) delete handle;
}

void libpolhemus_exit() { libusb_exit(nullptr); }
