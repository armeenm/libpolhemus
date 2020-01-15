#include "capi.h"

#include <cstdio>
#include <iostream>

#include "Context.h"

using namespace polhemus;

extern "C" {
int libpolhemus_init(libpolhemus_context** ctx) {
    try {
        *ctx = conv(new Context);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int libpolhemus_open(libpolhemus_context* ctx,
                     const libpolhemus_device_type dev_type,
                     libpolhemus_device_handle** handle) {
    try {
        *handle = conv(new DevHandle(conv(ctx), conv(dev_type)));
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int libpolhemus_get_device_type(libpolhemus_device_handle* handle,
                                libpolhemus_device_type* dev_type) {
    if (!handle) return -1;

    *dev_type = conv(conv(handle)->dev_type());

    return 0;
}

int libpolhemus_get_name(libpolhemus_device_handle* handle, const char** name) {
    if (!handle) return -1;

    *name = conv(handle)->name().c_str();

    return 0;
}

int libpolhemus_get_timeout(libpolhemus_device_handle* handle,
                            unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = conv(handle)->timeout();

    return 0;
}

int libpolhemus_set_timeout(libpolhemus_device_handle* handle,
                            const unsigned int timeout) {
    if (!handle) return -1;

    conv(handle)->timeout(timeout);

    return 0;
}

int libpolhemus_send_raw(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* buf) {
    if (!handle) return -1;

    return conv(handle)->send_raw(*reinterpret_cast<const Buffer*>(buf));
}

int libpolhemus_recv_raw(libpolhemus_device_handle* handle,
                         libpolhemus_buffer* buf) {
    if (!handle) return -1;

    return conv(handle)->recv_raw(conv(buf));
}

int libpolhemus_check_connection_att(libpolhemus_device_handle* handle,
                                     unsigned int attempts) {
    if (!handle) return -1;

    return conv(handle)->check_connection(attempts);
}

int libpolhemus_check_connection(libpolhemus_device_handle* handle) {
    if (!handle) return -1;

    return conv(handle)->check_connection();
}

int libpolhemus_send_cmd(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* cmd,
                         libpolhemus_buffer* resp) {
    if (!handle) return -1;

    return conv(handle)->send_cmd(*reinterpret_cast<const Buffer*>(cmd),
                                  conv(resp));
}

void libpolhemus_close(libpolhemus_device_handle* handle) {
    delete conv(handle);
}

void libpolhemus_exit(libpolhemus_context* ctx) { delete conv(ctx); }
}
