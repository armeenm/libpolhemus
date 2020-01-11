#include <chrono>
#include <cstdint>
#include <thread>
#include <unordered_map>

#include "Device.h"
#include "libpolhemus.h"

Device::Device(DevType type)
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

bool Device::valid() const { return _valid; }

DevInfo Device::info() const { return _info; }

unsigned int Device::timeout() const { return _timeout; }
void Device::timeout(unsigned int timeout) { _timeout = timeout; }

int Device::recv_raw(Buffer buf) const {
    return transfer_raw(buf, _info.read_ep);
}

int Device::send_raw(Buffer buf) const {
    return transfer_raw(buf, _info.write_ep);
}

int Device::check_connection(uint8_t attempts) const {
    auto cr = reinterpret_cast<unsigned char*>(const_cast<char*>("\r"));
    auto cmd = Buffer{cr, 1};
    auto resp_buf = std::unique_ptr<unsigned char[]>(new unsigned char[128]);
    auto resp = Buffer{resp_buf.get(), 128};
    int err = 0;

    // Send useless command, await response
    // Try a total of 'attempts' times before giving up
    int i = 0;
    do {
        if (++i == attempts) return -1;
        if (send_raw(cmd) <= 0) continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        err = recv_raw(resp);
    } while (err <= 0);

    i = 0;

    // Clear out the ingress buffer
    do {
        if (++i == attempts) return -2;
        err = recv_raw(resp);
    } while (err != 1);

    return 0;
}

int Device::send_cmd(Buffer cmd, Buffer resp) const {
    return send_buf(cmd, resp,
                    cmd.data && cmd.data[0] != '\r' && cmd.data[0] != 'p' &&
                        cmd.data[0] != 'P');
}

void Device::close() {
    if (_valid) {
        libusb_close(_handle);
        _valid = false;
    }
}

Device::~Device() { close(); }

int Device::transfer_raw(Buffer buf, unsigned char ep) const {
    int transferred;
    int err = libusb_bulk_transfer(_handle, ep, buf.data, buf.len, &transferred,
                                   _timeout);

    return err ? err < 0 : transferred;
}

int Device::send_buf(Buffer cmd, Buffer resp, bool add_cr) const {
    for (int i = 0; i < cmd.len; i++) {
        int err = send_raw({cmd.data + i, 1});
        if (err != 1) return err;
    }

    if (add_cr) {
        auto cr = reinterpret_cast<unsigned char*>(const_cast<char*>("\r"));
        int err = send_raw(Buffer{cr, 1});
        if (err != 1) return err;
    }

    return recv_raw(resp);
}
