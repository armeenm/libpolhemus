#include <cstdint>
#include <iostream>
#include <vector>

#include "Device.h"
#include "export.h"

std::vector<Device> devices;

using libpolhemus_device_handle = Device;
using libpolhemus_devtype = DevType;
using libpolhemus_Buffer = Buffer;

int libpolhemus_init() { return libusb_init(nullptr); }

int libpolhemus_open(DevType dev_type, Device** handle) {
    try {
        *handle = new Device(dev_type);
    } catch (const std::exception& e) {
        std::cerr << "Failed to open device: " << e.what() << '\n';
        return -1;
    }
}

int libpolhemus_get_timeout(const Device* const handle, unsigned int* timeout) {
    if (!handle) return -1;

    *timeout = handle->timeout();

    return 0;
}

int libpolhemus_set_timeout(Device* const handle, unsigned int timeout) {
    if (!handle) return -1;

    handle->timeout(timeout);

    return 0;
}

int libpolhemus_send_raw(Device* handle, const Buffer* const buf) {
    if (!handle) return -1;

    return handle->send_raw(*buf);
}

int libpolhemus_recv_raw(Device* handle, Buffer* buf) {
    if (!handle) return -1;

    return handle->recv_raw(buf);
}

int libpolhemus_check_connection_att(Device* handle, unsigned int attempts) {
    if (!handle) return -1;

    return handle->check_connection(attempts);
}

int libpolhemus_check_connection(Device* handle) {
    if (!handle) return -1;

    return handle->check_connection();
}

int libpolhemus_send_cmd(Device* handle, const Buffer* const cmd,
                         Buffer* resp) {
    if (!handle) return -1;

    return handle->send_cmd(*cmd, resp);
}

void libpolhemus_close(Device* handle) {
    if (handle) delete handle;
}

void libpolhemus_exit() { libusb_exit(nullptr); }
