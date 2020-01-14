#include <cstdint>
#include <iostream>
#include <vector>

#include "Context.h"
#include "libpolhemus.hpp"

using namespace polhemus;

extern "C" {
#include "libpolhemus.h"

struct libpolhemus_device_handle {
    void* handle;
};

int libpolhemus_init(libpolhemus_context** ctx) {
    try {
        *ctx = reinterpret_cast<libpolhemus_context*>(new Context);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int libpolhemus_open(libpolhemus_context* ctx, libpolhemus_device_type dev_type,
                     libpolhemus_device_handle** handle) {
    try {
        *handle = reinterpret_cast<libpolhemus_device_handle*>(new DevHandle(
            reinterpret_cast<Context*>(ctx), static_cast<DevType>(dev_type)));
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

int libpolhemus_get_dev_type(libpolhemus_device_handle* handle,
                             libpolhemus_device_type* dev_type) {
    if (!handle) return -1;

    *dev_type = static_cast<libpolhemus_device_type>(
        reinterpret_cast<DevHandle*>(handle)->dev_type());

    return 0;
}

int libpolhemus_get_name(libpolhemus_device_handle* handle, const char** name) {
    if (!handle) return -1;

    *name = reinterpret_cast<DevHandle*>(handle)->name().c_str();

    return 0;
}

int libpolhemus_get_timeout(libpolhemus_device_handle* handle,
                            unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = reinterpret_cast<DevHandle*>(handle)->timeout();

    return 0;
}

int libpolhemus_set_timeout(libpolhemus_device_handle* handle,
                            const unsigned int timeout) {
    if (!handle) return -1;

    reinterpret_cast<DevHandle*>(handle)->timeout(timeout);

    return 0;
}

int libpolhemus_send_raw(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* buf) {
    if (!handle) return -1;

    return reinterpret_cast<DevHandle*>(handle)->send_raw(
        *reinterpret_cast<const Buffer*>(buf));
}

int libpolhemus_recv_raw(libpolhemus_device_handle* handle,
                         libpolhemus_buffer* buf) {
    if (!handle) return -1;

    return reinterpret_cast<DevHandle*>(handle)->recv_raw(
        reinterpret_cast<Buffer*>(buf));
}

int libpolhemus_check_connection_att(libpolhemus_device_handle* handle,
                                     unsigned int attempts) {
    if (!handle) return -1;

    return reinterpret_cast<DevHandle*>(handle)->check_connection(attempts);
}

int libpolhemus_check_connection(libpolhemus_device_handle* handle) {
    if (!handle) return -1;

    return reinterpret_cast<DevHandle*>(handle)->check_connection();
}

int libpolhemus_send_cmd(libpolhemus_device_handle* handle,
                         const libpolhemus_buffer* cmd,
                         libpolhemus_buffer* resp) {
    if (!handle) return -1;

    return reinterpret_cast<DevHandle*>(handle)->send_cmd(
        *reinterpret_cast<const Buffer*>(cmd), reinterpret_cast<Buffer*>(resp));
}

void libpolhemus_close(libpolhemus_device_handle* handle) {
    delete reinterpret_cast<DevHandle*>(handle);
}

void libpolhemus_exit(libpolhemus_context* ctx) {
    delete reinterpret_cast<Context*>(ctx);
}
}
