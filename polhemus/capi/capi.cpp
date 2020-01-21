#include "polhemus/capi/capi.h"

#include <cassert>
#include <cstdio>
#include <fmt/format.h>

#include "polhemus/cxx/Context.h"

using namespace polhemus;

extern "C" {
auto libpolhemus_init(libpolhemus_context const** const ctx) -> int {
  if (ctx != nullptr) {
    try {
      *ctx = conv(new Context);
    } catch (std::exception const& e) {
      fmt::print(stderr, "Failed to initialize: {}\n", e.what());
      return -1;
    }
  }

  return 0;
}

auto libpolhemus_open(libpolhemus_context* const ctx, libpolhemus_device_type const dev_type,
                      unsigned int const timeout, libpolhemus_device_handle const** const handle) -> int {
  try {
    *handle = conv(new DevHandle(conv(ctx), conv(dev_type), timeout));
  } catch (std::exception const& e) {
    fmt::print(stderr, "Failed to open device: {}\n", e.what());
    return -1;
  }

  return 0;
}

auto libpolhemus_get_device_type(libpolhemus_device_handle* const handle) -> libpolhemus_device_type {
  return conv(conv(handle)->dev_type());
}

auto libpolhemus_get_name(libpolhemus_device_handle* const handle) -> char const* {
  return conv(handle)->name().data();
}

auto libpolhemus_get_timeout(libpolhemus_device_handle* const handle) -> unsigned int {
  return conv(handle)->timeout();
}

auto libpolhemus_set_timeout(libpolhemus_device_handle* const handle, unsigned int const timeout) -> void {
  conv(handle)->timeout(timeout);
}

auto libpolhemus_check_connection(libpolhemus_device_handle* const handle, int const attempts) -> bool {
  return (attempts < 0) ? conv(handle)->check_connection() : conv(handle)->check_connection(attempts);
}

auto libpolhemus_send_cmd(libpolhemus_device_handle* const handle, char const* const cmd, char* const resp,
                          int const max_resp_size) -> int {
  try {
    return conv(handle)->send_cmd(cmd, resp, max_resp_size);
  } catch (std::exception const& e) {
    return 0;
  }
}

auto libpolhemus_close(libpolhemus_device_handle* handle) -> void { delete conv(handle); }

auto libpolhemus_exit(libpolhemus_context* ctx) -> void { delete conv(ctx); }

} // extern "C"
