#include "DevHandle.h"

#include <chrono>
#include <cstdint>
#include <thread>
#include <unordered_map>

#include "libpolhemus.h"

DevHandle::DevHandle(DevType type)
    : _valid(false), _info(dev_type_info_map.at(type)), _timeout(50) {
    libusb_device** list;
    libusb_device* found = nullptr;
    int err = 0;

    ssize_t cnt = libusb_get_device_list(nullptr, &list);
    if (cnt < 0) return;

    for (int i = 0; i < cnt; i++) {
        libusb_device* device = list[i];

        libusb_device_descriptor desc;
        err = libusb_get_device_descriptor(device, &desc);
        if (err) continue;

        if (desc.idVendor == _info.vid && desc.idProduct == _info.pid) {
            found = device;
            break;
        }
    }

    if (found) {
        err = libusb_open(found, &_handle);
        if (!err) _valid = true;
    }

    libusb_free_device_list(list, 1);
}

bool DevHandle::valid() const { return _valid; }

DevInfo DevHandle::info() const { return _info; }

unsigned int DevHandle::timeout() const { return _timeout; }
void DevHandle::timeout(unsigned int timeout) { _timeout = timeout; }

int DevHandle::recv_raw(Buffer buf) const {
    return transfer_raw(buf, _info.read_ep);
}

int DevHandle::send_raw(Buffer buf) const {
    return transfer_raw(buf, _info.write_ep);
}

int DevHandle::check_connection(uint8_t attempts) const {
    auto cmd = Buffer{"\r", 1};
    auto resp =
        Buffer{std::unique_ptr<unsigned char[]>(new unsigned char[128]), 128};
    int err = 0;

    // Send useless command, await response
    // Try a total of 'attempts' times before giving up
    int i = 0;
    do {
        i++;
        if (send_raw(cmd) <= 0) continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        err = recv_raw(resp);
    } while (err <= 0 && i < attempts);

    if (err <= 0) return -1;

    i = 0;

    // Clear out the ingress buffer
    do {
        i++;
        err = recv_raw(resp);
    } while (err != LIBUSB_ERROR_TIMEOUT && i < attempts);

    if (err != LIBUSB_ERROR_TIMEOUT) return -1;

    return 0;
}

int DevHandle::send_cmd(Buffer cmd, Buffer resp) const {
    return send_buf(cmd, resp,
                    cmd.buf && cmd.buf[0] != '\r' && cmd.buf[0] != 'p' &&
                        cmd.buf[0] != 'P');
}

void DevHandle::close() {
    if (_valid) {
        libusb_close(_handle);
        _valid = false;
    }
}

DevHandle::~DevHandle() { close(); }

int DevHandle::transfer_raw(Buffer buf, unsigned char ep) const {
    int transferred;
    int err = libusb_bulk_transfer(_handle, ep, buf.data, buf.len, &transferred,
                                   _timeout);

    return err ? err < 0 : transferred;
}

int DevHandle::send_buf(Buffer cmd, Buffer resp, bool add_cr) const {
    for (int i = 0; i < cmd.len; i++) {
        int err = send_raw({cmd.data + i, 1});
        if (err != 1) return err;
    }

    if (add_cr) {
        int err = send_raw({"\r", 1});
        if (err != 1) return err;
    }

    return recv_raw(resp);
}
