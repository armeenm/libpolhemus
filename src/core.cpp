#include <cstdint>
#include <iostream>
#include <vector>

#include "DevHandle.h"
#include "libpolhemus.h"

#define CHECK_IDX(idx, err) \
    if (idx >= devices.size()) return err

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

bool libpolhemus_valid(uint8_t handle_idx) {
    CHECK_H(handle_idx, handle, 0);

    return handle.valid();
}

unsigned int libpolhemus_get_timeout(uint8_t handle_idx) {
    CHECK_H(handle_idx, handle, -10);

    return handle.timeout();
}

void libpolhemus_set_timeout(uint8_t handle_idx, unsigned int timeout) {
    CHECK_H(handle_idx, handle, void());

    handle.timeout(timeout);
}

int libpolhemus_send_raw(uint8_t handle_idx, Buffer buf) {
    CHECK_H(handle_idx, handle, -10);

    return handle.send_raw(buf);
}

int libpolhemus_recv_raw(uint8_t handle_idx, Buffer buf) {
    CHECK_H(handle_idx, handle, -10);

    return handle.recv_raw(buf);
}

int libpolhemus_check_connection_att(uint8_t handle_idx, uint8_t attempts) {
    CHECK_H(handle_idx, handle, -10);

    return handle.check_connection(attempts);
}

int libpolhemus_check_connection(uint8_t handle_idx) {
    CHECK_H(handle_idx, handle, -10);

    return handle.check_connection();
}

int libpolhemus_send_cmd(uint8_t handle_idx, Buffer cmd, Buffer resp) {
    CHECK_H(handle_idx, handle, -10);

    return handle.send_cmd(cmd, resp);
}

void libpolhemus_close(uint8_t handle_idx) {
    CHECK_H(handle_idx, handle, void());

    handle.close();
}

void libpolhemus_exit() {
    devices.clear();
    libusb_exit(nullptr);
}
