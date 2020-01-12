#pragma once

#include <libusb-1.0/libusb.h>

#include <cstdint>
#include <string>

#include "export.h"

struct DevInfo {
    DevType dev_type;
    std::string name;
    std::uint16_t vid, pid;
    unsigned char write_ep, read_ep;
};

class Device {
   public:
    Device(DevType type);
    ~Device();
    Device(const Device&) = delete;
    Device& operator=(const Device& other) = delete;
    Device(Device&&) = default;
    Device& operator=(Device&& other) = default;

    [[nodiscard]] DevInfo info() const noexcept;

    [[nodiscard]] unsigned int timeout() const noexcept;
    void timeout(unsigned int timeout) noexcept;

    int send_raw(const Buffer& buf) const noexcept;
    int recv_raw(Buffer* buf) const noexcept;

    bool check_connection(unsigned int attempts = 10) const noexcept;

    int send_cmd(const Buffer& cmd, Buffer* resp) const noexcept;

   private:
    int transfer_raw(Buffer* buf, unsigned char ep) const noexcept;
    int send_buf(const Buffer& cmd, Buffer* resp, bool add_cr) const noexcept;

    DevInfo info_;
    libusb_device_handle* handle_;
    unsigned int timeout_ = 50;
};
