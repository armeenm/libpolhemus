#include "Device.h"

#include <chrono>
#include <exception>
#include <thread>
#include <unordered_map>

static const std::unordered_map<DevType, DevInfo> dev_type_info_map = {
    {DevType::PATRIOT,
     {DevType::PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},

    {DevType::PATRIOT_HS,
     {DevType::PATRIOT_HS, "High Speed Patriot", 0x0f44, 0xef20, 0x04, 0x88}},
};

Device::Device(DevType type) : info_(dev_type_info_map.at(type)) {
    libusb_device** dev_list;
    libusb_device* found = nullptr;
    int err = 0;

    ssize_t cnt = libusb_get_device_list(nullptr, &dev_list);
    if (cnt < 0) throw std::runtime_error("Failed to get device list");

    for (int i = 0; i < cnt; i++) {
        libusb_device_descriptor desc;
        err = libusb_get_device_descriptor(dev_list[i], &desc);
        /* If we failed to get the descriptor, move on */
        if (err) continue;

        /* Check if USB VID:PID matches */
        /* TODO: Account for the possibility of multiple devs */
        if (desc.idVendor == info_.vid && desc.idProduct == info_.pid) {
            found = dev_list[i];
            break;
        }
    }

    if (err) throw std::runtime_error("Failed to find device");

    if (found) err = libusb_open(found, &handle_);

    /* Make sure we clean up the dev list */
    libusb_free_device_list(dev_list, 1);

    /* libusb_open failed */
    if (err) throw std::runtime_error("Failed to open device");
}

Device::~Device() { libusb_close(handle_); }

DevInfo Device::info() const noexcept { return info_; }

unsigned int Device::timeout() const noexcept { return timeout_; }
void Device::timeout(unsigned int timeout) noexcept { timeout_ = timeout; }

int Device::recv_raw(Buffer* buf) const noexcept {
    return transfer_raw(buf, info_.read_ep);
}

int Device::send_raw(const Buffer& buf) const noexcept {
    return transfer_raw(const_cast<Buffer*>(&buf), info_.write_ep);
}

bool Device::check_connection(unsigned int attempts) const noexcept {
    auto cr = reinterpret_cast<unsigned char*>(const_cast<char*>("\r"));
    auto cmd = Buffer{cr, 1};
    auto resp_buf = std::unique_ptr<unsigned char[]>(new unsigned char[128]);
    auto resp = Buffer{resp_buf.get(), 128};
    int err = 0;

    /* Send useless command, await response
     * Try a total of 'attempts' times before giving up
     */
    unsigned int i = 0;
    do {
        if (++i == attempts) return false;
        if (send_raw(cmd) <= 0) continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        err = recv_raw(&resp);
    } while (err <= 0);

    i = 0;

    /* Clear out the ingress buffer */
    /* TODO: Consider breaking this out into another function */
    do {
        if (++i == attempts) return false;
        err = recv_raw(&resp);
    } while (err != 1);

    return true;
}

int Device::send_cmd(const Buffer& cmd, Buffer* resp) const noexcept {
    return send_buf(cmd, resp,
                    cmd.data && cmd.data[0] != '\r' && cmd.data[0] != 'p' &&
                        cmd.data[0] != 'P');
}

int Device::transfer_raw(Buffer* buf, unsigned char ep) const noexcept {
    int transferred;
    int err = libusb_bulk_transfer(handle_, ep, buf->data, buf->len,
                                   &transferred, timeout_);

    return err ? err < 0 : transferred;
}

int Device::send_buf(const Buffer& cmd, Buffer* resp, bool add_cr) const
    noexcept {
    for (int i = 0; i < cmd.len; i++) {
        int err = send_raw({cmd.data + i, 1});
        if (err != 1) return err;
    }

    if (add_cr) {
        auto cr = reinterpret_cast<unsigned char*>(const_cast<char*>("\r"));
        int err = send_raw({cr, 1});
        if (err != 1) return err;
    }

    return recv_raw(resp);
}
