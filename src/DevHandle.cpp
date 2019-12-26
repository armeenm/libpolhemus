#include "DevHandle.h"

#include <unordered_map>

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

unsigned int timeout() const { return _timeout; }
void timeout(unsigned int timeout) { _timeout = timeout; }

int recv_raw(Buffer buf) const { return transfer_raw(buf, _info.read_ep); }

int send_raw(Buffer buf) const { return transfer_raw(buf, _info.write_ep); }

void DevHandle::close() {
    if (_valid) {
        libusb_close(_handle);
        _valid = false;
    }
}

int send_cmd(Buffer cmd, Buffer resp) const {
    return send_buf(cmd, resp, cmd[0] != 'p' || cmd[0] != 'P');
}

DevHandle::~DevHandle() { close(); }

int transfer_raw(Buffer buf, unsigned char ep) const {
    int transferred;
    int err = libusb_bulk_transfer(_handle, ep, buf.data, buf.len, &transferred,
                                   _timeout);

    return err ? err < 0 : transferred;
}

int send_buf(Buffer cmd, Buffer resp, bool add_cr) const {
    for (int i = 0; i < cmd.len; i++) {
        int err = raw_send({cmd.data + i, 1});
        if (err != 1) return -100;
    }

    if (add_cr) {
        int err = raw_send({"\r", 1});
        if (err != 1) return -100;
    }

    return raw_recv(resp);
}
