#include "polhemus/capi/capi.h"

#include <cassert>
#include <cstdio>
#include <iostream>

#include "polhemus/cxx/Context.h"

using namespace polhemus;

extern "C" {
auto libpolhemus_init(libpolhemus_context** ctx) -> int {
  try {
    *ctx = conv(new Context);
  } catch (std::exception const& e) {
    std::cerr << "Failed to open device: " << e.what() << '\n';
    return -1;
  }

  return 0;
}

auto libpolhemus_open(libpolhemus_context* ctx, libpolhemus_device_type const dev_type,
                      libpolhemus_device_handle** handle) -> int {
  try {
    *handle = conv(new DevHandle(conv(ctx), conv(dev_type)));
  } catch (std::exception const& e) {
    std::cerr << "Failed to open device: " << e.what() << '\n';
    return -1;
  }

  return 0;
}

auto libpolhemus_get_device_type(libpolhemus_device_handle* handle, libpolhemus_device_type* dev_type) -> int {
  assert(handle);

  *dev_type = conv(conv(handle)->dev_type());

  return 0;
}

auto libpolhemus_get_name(libpolhemus_device_handle* handle, char const** name) -> int {
  assert(handle);

  *name = conv(handle)->name().c_str();

  return 0;
}

auto libpolhemus_get_timeout(libpolhemus_device_handle* handle, unsigned int* timeout) -> int {
  assert(handle);

  *timeout = conv(handle)->timeout();

  return 0;
}

auto libpolhemus_set_timeout(libpolhemus_device_handle* handle, unsigned int const timeout) -> int {
  assert(handle);

  conv(handle)->timeout(timeout);

  return 0;
}

auto libpolhemus_check_connection_att(libpolhemus_device_handle* handle, unsigned int attempts) -> int {
  assert(handle);

  return conv(handle)->check_connection(attempts);
}

auto libpolhemus_check_connection(libpolhemus_device_handle* handle) -> int {
  assert(handle);

  return conv(handle)->check_connection();
}

auto libpolhemus_send_cmd(libpolhemus_device_handle* handle, libpolhemus_buffer const* cmd, libpolhemus_buffer* resp)
    -> int {
  assert(handle);

  return conv(handle)->send_cmd(*reinterpret_cast<Buffer const*>(cmd), conv(resp));
}

auto libpolhemus_close(libpolhemus_device_handle* handle) -> void { delete conv(handle); }

auto libpolhemus_exit(libpolhemus_context* ctx) -> void { delete conv(ctx); }
}
