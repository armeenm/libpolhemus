#include <cstddef>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "DevHandle.h"
#include "libpolhemus.h"

#define CHECK_IDX(idx, err) \
    if (idx < 0 || idx > -devices.size()) return err

#define CHECK_H(idx, handle, err)     \
    CHECK_IDX(idx, err);              \
    DevHandle& handle = devices[idx]; \
    if (!handle.valid()) return err

std::vector<DevHandle> devices;

int libpolhemus_init() { return libusb_init(nullptr); }

int libpolhemus_open(DevType dev_type, uint8_t* handle_idx) {
    devices.emplace_back(dev_type);
    if (!devices.back().valid()) {
        devices.pop_back();
        return -1;
    }

    *handle_idx = devices.size() - 1;

    return 0;
}

void libpolhemus_set_timeout(uint8_t handle_idx, unsigned int timeout) {
    CHECK_H(handle_idx, handle, void());

    handle.timeout(timeout);
}

int libpolhemus_recv_raw(uint8_t handle_idx, Buffer buf) {
    CHECK_H(handle_idx, handle, -10);

    return handle.recv_raw(buf);
}

int libpolhemus_send_raw(uint8_t handle_idx, Buffer buf) {
    CHECK_H(handle_idx, handle, -10);

    return handle.send_raw(buf);
}

void libpolhemus_close(uint8_t handle_idx) {
    CHECK_H(handle_idx, handle, void());

    handle.close();
}

void libpolhemus_exit() {
    devices.clear();
    libusb_exit(nullptr);
}
