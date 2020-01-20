#pragma once

#include <libusb-1.0/libusb.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "polhemus.hpp"
#include "polhemus/cxx/Context.h"

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

  auto transfer(unsigned char* buf, int size, unsigned char ep) const -> int;

  auto send(std::string_view buf) const -> int;

  auto recv(std::string* buf) const -> int;
  [[nodiscard]] auto recv(int max_size) const -> std::pair<std::string, int>;

  [[nodiscard]] auto lctx() const noexcept -> libusb_context*;

  DevInfo const info;
  libusb_device_handle* handle;
  unsigned int timeout;

private:
  static std::unordered_map<DevType, DevInfo> const dev_type_info_map_;
  Context ctx_;
};

} // namespace polhemus
