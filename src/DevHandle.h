#pragma once

#include <cstdio>
#include <string>
#include <unordered_map>

#include "libpolhemus.h"

struct DevInfo {
    DevType dev_type;
    std::string name;
    uint16_t vid, pid;
    unsigned char write_ep, read_ep;
};

class DevHandle {
   public:
    DevHandle(DevType type);
    DevHandle(const DevHandle&) = delete;
    DevHandle& operator=(const DevHandle& other) = delete;
    DevHandle(DevHandle&&) = default;
    DevHandle& operator=(DevHandle&& other) = default;

    bool valid() const;
    DevInfo info() const;

    unsigned int timeout() const;
    void timeout(unsigned int timeout);

    int send_raw(Buffer buf) const;
    int recv_raw(Buffer buf) const;

    int check_connection(uint8_t attempts = 10) const;

    int send_cmd(Buffer cmd, Buffer resp) const;

    void close();
    ~DevHandle();

   private:
    int transfer_raw(Buffer buf, unsigned char ep) const;
    int send_buf(Buffer cmd, Buffer resp, bool add_cr) const;

    bool _valid;
    DevInfo _info;
    libusb_device_handle* _handle;
    unsigned int _timeout;
};

const std::unordered_map<DevType, DevInfo> dev_type_info_map = {
    {DevType::PATRIOT,
     {DevType::PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},

    {DevType::PATRIOT_HS,
     {DevType::PATRIOT_HS, "High Speed Patriot", 0x0f44, 0xef20, 0x04, 0x88}},
};
