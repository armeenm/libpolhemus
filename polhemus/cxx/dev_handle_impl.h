#pragma once

#include <libusb-1.0/libusb.h>

#include <array>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "polhemus.hpp"
#include "polhemus/cxx/context.h"

namespace polhemus {

class DevHandle::Impl {
public:
  struct DevInfo {
    std::string_view name;
    std::uint16_t vid, pid;
    unsigned char write_ep, read_ep;
  };

  Impl(Context* ctx, DevType type, unsigned int timeout);

  auto transfer(unsigned char* buf, int size, unsigned char ep) const -> int;

  auto send(std::string_view buf) const -> int;

  auto recv(std::string& resp) const -> int;
  auto recv(char* resp, int max_resp_size) const -> int;
  [[nodiscard]] auto recv(int max_resp_size) const -> std::string;

  [[nodiscard]] auto lctx() const noexcept -> libusb_context*;

  DevInfo info;
  DevType type;
  libusb_device_handle* handle;
  unsigned int timeout;

private:
  Context ctx_;
  constexpr static DevInfo devinfos_[] = {
      [PATRIOT] = {"Patriot", 0x0f44, 0xef12, 0x02, 0x82}, [PATRIOT_HS] = {"Patriot HS", 0x0f44, 0xef20, 0x04, 0x88}};
};

} // namespace polhemus
