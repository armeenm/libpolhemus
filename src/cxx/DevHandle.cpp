#include <libusb-1.0/libusb.h>

#include <chrono>
#include <exception>
#include <thread>

#include "DevHandleImpl.h"
#include "libpolhemus.hpp"

namespace polhemus {

/***** Ctors *****/
// {{{

DevHandle::DevHandle(Context* ctx, const DevType type, const unsigned int timeout)
    : impl_(new Impl(ctx, type, timeout)) {
  libusb_device** dev_list;
  libusb_device* found = nullptr;
  int err = 0;

  const ssize_t cnt = libusb_get_device_list(impl_->lctx(), &dev_list);
  if (cnt < 0)
    throw std::runtime_error("Failed to get device list");

  for (ssize_t i = 0; i < cnt; i++) {
    libusb_device_descriptor desc;
    err = libusb_get_device_descriptor(dev_list[i], &desc);
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
    err = libusb_open(found, &(impl_->handle));
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

DevType DevHandle::dev_type() const noexcept { return impl_->info.dev_type; }
const std::string& DevHandle::name() const noexcept { return impl_->info.name; }
unsigned int DevHandle::timeout() const noexcept { return impl_->timeout; }

// }}}

/***** Mutators *****/
// {{{

void DevHandle::timeout(const unsigned int timeout) noexcept { impl_->timeout = timeout; }

// }}}

/***** I/O *****/
// {{{

bool DevHandle::check_connection(const unsigned int attempts) const noexcept {
  unsigned char cr[] = "\r";
  const auto cmd = Buffer{cr, 1};
  auto resp_buf = std::unique_ptr<unsigned char[]>(new unsigned char[128]);
  auto resp = Buffer{resp_buf.get(), 128};
  int err = 0;

  /* Send useless command, await response
   * Try a total of 'attempts' times before giving up
   */
  unsigned int i = 0;
  do {
    if (++i == attempts)
      return false;
    if (send_raw(cmd) <= 0)
      continue;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    err = recv_raw(&resp);
  } while (err <= 0);

  i = 0;

  /* Clear out the ingress buffer */
  /* TODO: Consider breaking this out into another function */
  do {
    if (++i == attempts)
      return false;
    err = recv_raw(&resp);
  } while (err != 1);

  return true;
}

int DevHandle::send_cmd(const Buffer& cmd, Buffer* resp) const noexcept {
  return impl_->send_buf(cmd, resp, cmd.data && cmd.data[0] != '\r' && cmd.data[0] != 'p' && cmd.data[0] != 'P');
}

int DevHandle::send_raw(const Buffer& buf) const noexcept { return impl_->send_raw(buf); }
int DevHandle::recv_raw(Buffer* buf) const noexcept { return impl_->recv_raw(buf); }

// }}}

} // namespace polhemus
