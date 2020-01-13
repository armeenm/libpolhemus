#include <libusb-1.0/libusb.h>

#include <cstdint>
#include <iostream>
#include <vector>

#include "libpolhemus.hpp"

using namespace polhemus;

int libpolhemus_init() { return libusb_init(nullptr); }

int libpolhemus_open(DevType dev_type, DevHandle** handle) {
    try {
        *handle = new DevHandle(dev_type);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int libpolhemus_get_dev_type(DevHandle* handle, DevType* dev_type) {
    if (!handle) return -1;

    *dev_type = handle->dev_type();

    return 0;
}

int libpolhemus_get_name(DevHandle* handle, const char** name) {
    if (!handle) return -1;

    *name = handle->name().c_str();

    return 0;
}

int libpolhemus_get_timeout(DevHandle* handle, unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = handle->timeout();

    return 0;
}

int libpolhemus_set_timeout(DevHandle* handle, unsigned int timeout) {
    if (!handle) return -1;

    handle->timeout(timeout);

    return 0;
}

int libpolhemus_send_raw(DevHandle* handle, const Buffer* buf) {
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

int libpolhemus_send_cmd(DevHandle* handle, const Buffer* cmd, Buffer* resp) {
    if (!handle) return -1;

    return handle->send_cmd(*cmd, resp);
}

void libpolhemus_close(DevHandle* handle) { delete handle; }

void libpolhemus_exit() { libusb_exit(nullptr); }
