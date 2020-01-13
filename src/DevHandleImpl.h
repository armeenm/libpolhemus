#pragma once

#include <libusb-1.0/libusb.h>

#include <unordered_map>

#include "libpolhemus.hpp"

namespace polhemus {

class DevHandle::Impl {
   public:
    struct DevInfo {
        DevType dev_type;
        std::string name;
        std::uint16_t vid, pid;
        unsigned char write_ep, read_ep;
    };

    Impl(DevType type);

    int transfer_raw(Buffer* buf, unsigned char ep) const noexcept;
    int send_buf(const Buffer& cmd, Buffer* resp, bool add_cr) const noexcept;

    int send_raw(const Buffer& buf) const noexcept;
    int recv_raw(Buffer* buf) const noexcept;

    const DevInfo info;
    libusb_device_handle* handle;
    unsigned int timeout;

   private:
    static const std::unordered_map<DevType, DevInfo> dev_type_info_map_;
};

}  // namespace polhemus
