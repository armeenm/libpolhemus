#pragma once

#include <libusb-1.0/libusb.h>

#include <unordered_map>

#include "Context.h"
#include "polhemus.hpp"

namespace polhemus {

class DevHandle::Impl {
public:
  struct DevInfo {
    DevType dev_type;
    std::string name;
    std::uint16_t vid, pid;
    unsigned char write_ep, read_ep;
  };

  Impl(Context* ctx, DevType type, unsigned int timeout);

  auto transfer_raw(Buffer* buf, unsigned char ep) const noexcept -> int;
  auto send_buf(Buffer const& cmd, Buffer* resp, bool add_cr) const noexcept -> int;

  auto send_raw(Buffer const& buf) const noexcept -> int;
  auto recv_raw(Buffer* buf) const noexcept -> int;

  [[nodiscard]] auto lctx() const noexcept -> libusb_context*;

  DevInfo const info;
  libusb_device_handle* handle;
  unsigned int timeout;

private:
  static std::unordered_map<DevType, DevInfo> const dev_type_info_map_;
  Context ctx_;
};

} // namespace polhemus
