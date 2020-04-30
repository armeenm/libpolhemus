#include <libusb-1.0/libusb.h>

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>

#include "polhemus.hpp"
#include "polhemus/cxx/context.h"
#include "polhemus/cxx/dev_handle_impl.h"
#include "polhemus/cxx/lits.h"
#include "third_party/magic_enum.hpp"

namespace polhemus {

/***** Ctors *****/
// {{{

DevHandle::DevHandle(Context* const ctx, DevType const type) : DevHandle{ctx, type, 50} {}

DevHandle::DevHandle(Context* const ctx, DevType const type, unsigned int const timeout)
    : impl_{std::make_unique<Impl>(ctx ? ctx->lctx() : nullptr, type, timeout)} {

  libusb_device** dev_list;
  libusb_device* found = nullptr;

  ssize_t const cnt = libusb_get_device_list(impl_->lctx, &dev_list);
  if (cnt < 0) {
    throw std::runtime_error("Failed to get device list");
  }

  for (ssize_t i = 0; i < cnt; i++) {
    libusb_device_descriptor desc;
    auto err = libusb_get_device_descriptor(dev_list[i], &desc);
    /* If we failed to get the descriptor, move on */
    if (err)
      continue;

    /* Check if USB VID:PID matches */
    /* TODO: Account for the possibility of multiple devs */
    if (desc.idVendor == impl_->info.vid && desc.idProduct == impl_->info.pid) {
      found = dev_list[i];
      break;
    }
  }

  if (found) {
    auto err = libusb_open(found, &(impl_->handle));
    libusb_free_device_list(dev_list, 1);
    if (err)
      throw std::runtime_error("Failed to open device");
  } else {
    libusb_free_device_list(dev_list, 1);
    throw std::runtime_error("Failed to find device");
  }
}

DevHandle::~DevHandle() { libusb_close(impl_->handle); }

// }}}

/***** Accessors *****/
// {{{

auto DevHandle::dev_type() const noexcept -> DevType { return impl_->type; }
auto DevHandle::name() const noexcept -> std::string_view { return impl_->info.name; }
auto DevHandle::timeout() const noexcept -> unsigned int { return impl_->timeout; }

// }}}

/***** Mutators *****/
// {{{

auto DevHandle::timeout(unsigned int const timeout) noexcept -> void { impl_->timeout = timeout; }

// }}}

/***** I/O *****/
// {{{

auto DevHandle::check_connection(unsigned int const attempts) const noexcept -> bool {
  auto constexpr WAIT_TIME = 100ms;
  auto constexpr RESP_SIZE = 128;

  auto const cmd = "\r"s;
  auto resp = std::string();
  resp.reserve(RESP_SIZE);

  auto err = 0;

  /* Send useless command, await response
   * Try a total of 'attempts' times before giving up
   */
  auto i = 0u;
  do {
    if (++i == attempts)
      return false;

    if (impl_->send(cmd) <= 0)
      continue;

    std::this_thread::sleep_for(WAIT_TIME);

    err = impl_->recv(resp);
  } while (err <= 0);

  i = 0;

  /* Clear out the ingress buffer */
  /* TODO: Consider breaking this out into another function */
  do {
    if (++i == attempts)
      return false;

    err = impl_->recv(resp);
  } while (err != 1);

  return true;
}

auto DevHandle::send_cmd(std::string_view const cmd, std::string& resp) const -> int {
  auto transferred = decltype(cmd)::size_type{0};

  for (auto const& chr : cmd)
    transferred += impl_->send({&chr, 1});

  if (transferred != cmd.length())
    throw std::runtime_error("Failed to send command");

  if (cmd[0] != '\r' && cmd[0] != 'p' && cmd[0] != 'P')
    impl_->send("\r");

  return impl_->recv(resp);
}

auto DevHandle::send_cmd(std::string_view const cmd, int const max_resp_size) const -> std::string {
  std::string resp;
  resp.reserve(max_resp_size);

  resp.resize(send_cmd(cmd, resp));

  return resp;
}

auto DevHandle::recv_raw(std::string& resp) const -> int { return impl_->recv(resp); }
auto DevHandle::recv_raw(int const max_resp_size) const -> std::string {
  return impl_->recv(max_resp_size);
}
auto DevHandle::recv_raw(char* const resp, int const max_resp_size) const -> int {
  return impl_->recv(resp, max_resp_size);
}

auto DevHandle::send_raw(std::string_view const buf) const -> int { return impl_->send(buf); }

// }}}

} // namespace polhemus
