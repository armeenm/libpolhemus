#include "capi.h"

#include <cstdio>
#include <iostream>

#include "cxx/Context.h"

using namespace polhemus;

extern "C" {
auto libpolhemus_init(libpolhemus_context** ctx) -> int {
  try {
    *ctx = conv(new Context);
  } catch (const std::exception& e) {
    std::cerr << "Failed to open device: " << e.what() << '\n';
    return -1;
  }

  return 0;
}

auto libpolhemus_open(libpolhemus_context* ctx, const libpolhemus_device_type dev_type,
                      libpolhemus_device_handle** handle) -> int {
  try {
    *handle = conv(new DevHandle(conv(ctx), conv(dev_type)));
  } catch (const std::exception& e) {
    std::cerr << "Failed to open device: " << e.what() << '\n';
    return -1;
  }

  return 0;
}

auto libpolhemus_get_device_type(libpolhemus_device_handle* handle, libpolhemus_device_type* dev_type) -> int {
  if (!handle)
    return -1;

  *dev_type = conv(conv(handle)->dev_type());

  return 0;
}

auto libpolhemus_get_name(libpolhemus_device_handle* handle, const char** name) -> int {
  if (!handle)
    return -1;

  *name = conv(handle)->name().c_str();

  return 0;
}

auto libpolhemus_get_timeout(libpolhemus_device_handle* handle, unsigned int* timeout) -> int {
  if (!handle)
    return -1;

  *timeout = conv(handle)->timeout();

  return 0;
}

auto libpolhemus_set_timeout(libpolhemus_device_handle* handle, const unsigned int timeout) -> int {
  if (!handle)
    return -1;

  conv(handle)->timeout(timeout);

  return 0;
}

auto libpolhemus_send_raw(libpolhemus_device_handle* handle, const libpolhemus_buffer* buf) -> int {
  if (!handle)
    return -1;

  return conv(handle)->send_raw(*reinterpret_cast<const Buffer*>(buf));
}

auto libpolhemus_recv_raw(libpolhemus_device_handle* handle, libpolhemus_buffer* buf) -> int {
  if (!handle)
    return -1;

  return conv(handle)->recv_raw(conv(buf));
}

auto libpolhemus_check_connection_att(libpolhemus_device_handle* handle, unsigned int attempts) -> int {
  if (!handle)
    return -1;

  return conv(handle)->check_connection(attempts);
}

auto libpolhemus_check_connection(libpolhemus_device_handle* handle) -> int {
  if (!handle)
    return -1;

  return conv(handle)->check_connection();
}

auto libpolhemus_send_cmd(libpolhemus_device_handle* handle, const libpolhemus_buffer* cmd, libpolhemus_buffer* resp)
    -> int {
  if (!handle)
    return -1;

  return conv(handle)->send_cmd(*reinterpret_cast<const Buffer*>(cmd), conv(resp));
}

auto libpolhemus_close(libpolhemus_device_handle* handle) -> void { delete conv(handle); }

auto libpolhemus_exit(libpolhemus_context* ctx) -> void { delete conv(ctx); }
}
