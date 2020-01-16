#include "DevHandleImpl.h"

#include "Context.h"

namespace polhemus {

DevHandle::Impl::Impl(Context* ctx, DevType const type, unsigned int const timeout_in)
    : info(dev_type_info_map_.at(type)), timeout(timeout_in), ctx_(ctx ? *ctx : Context()) {}

auto DevHandle::Impl::transfer_raw(Buffer* buf, unsigned char const ep) const noexcept -> int {
  int transferred;
  int err = libusb_bulk_transfer(handle, ep, buf->data, buf->len, &transferred, timeout);

  return err ? err < 0 : transferred;
}

auto DevHandle::Impl::send_buf(Buffer const& cmd, Buffer* resp, bool const add_cr) const noexcept -> int {
  for (int i = 0; i < cmd.len; i++) {
    int err = send_raw({cmd.data + i, 1});
    if (err != 1)
      return err;
  }

  if (add_cr) {
    unsigned char cr[] = "\r";
    int err = send_raw({cr, 1});
    if (err != 1)
      return err;
  }

  return recv_raw(resp);
}

auto DevHandle::Impl::recv_raw(Buffer* buf) const noexcept -> int { return transfer_raw(buf, info.read_ep); }

auto DevHandle::Impl::send_raw(Buffer const& buf) const noexcept -> int {
  return transfer_raw(const_cast<Buffer*>(&buf), info.write_ep);
}

auto DevHandle::Impl::lctx() const noexcept -> libusb_context* { return ctx_.lctx(); }

std::unordered_map<DevType, DevHandle::Impl::DevInfo> const DevHandle::Impl::dev_type_info_map_ = {
    {PATRIOT, {PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},
    {PATRIOT_HS, {PATRIOT_HS, "High Speed Patriot", 0x0f44, 0xef20, 0x04, 0x88}},
};

} // namespace polhemus
