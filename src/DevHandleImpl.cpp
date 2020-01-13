#include "DevHandleImpl.h"

namespace polhemus {

DevHandle::Impl::Impl(DevType type) : info(dev_type_info_map_.at(type)) {}

int DevHandle::Impl::transfer_raw(Buffer* buf, unsigned char ep) const
    noexcept {
    int transferred;
    int err = libusb_bulk_transfer(handle, ep, buf->data, buf->len,
                                   &transferred, timeout);

    return err ? err < 0 : transferred;
}

int DevHandle::Impl::send_buf(const Buffer& cmd, Buffer* resp,
                              bool add_cr) const noexcept {
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

int DevHandle::Impl::recv_raw(Buffer* buf) const noexcept {
    return transfer_raw(buf, info.read_ep);
}

int DevHandle::Impl::send_raw(const Buffer& buf) const noexcept {
    return transfer_raw(const_cast<Buffer*>(&buf), info.write_ep);
}

const std::unordered_map<DevType, DevHandle::Impl::DevInfo>
    DevHandle::Impl::dev_type_info_map_ = {
        {DevType::PATRIOT,
         {DevType::PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},

        {DevType::PATRIOT_HS,
         {DevType::PATRIOT_HS, "High Speed Patriot", 0x0f44, 0xef20, 0x04,
          0x88}},
};

}  // namespace polhemus