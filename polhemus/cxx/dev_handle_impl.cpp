#include "polhemus/cxx/dev_handle_impl.h"

#include "polhemus/cxx/context.h"
#include "polhemus/cxx/lits.h"

#include <cstdint>
#include <fmt/format.h>

namespace polhemus {

// TODO: Make a templated function to convert to unsigned char*

DevHandle::Impl::Impl(Context* ctx, DevType const dtype, unsigned int const timeout_in)
    : info(devinfos_[dtype]), type(dtype), timeout(timeout_in), ctx_(ctx ? *ctx : Context()) {}

auto DevHandle::Impl::transfer(unsigned char* const buf, int const len, unsigned char const ep) const -> int {
  int transferred;
  auto err = static_cast<libusb_error>(libusb_bulk_transfer(handle, ep, buf, len, &transferred, timeout));

  if (err < 0)
    throw std::runtime_error("Failed to transfer data: "s + libusb_strerror(err));

  return transferred;
}

auto DevHandle::Impl::send(std::string_view const buf) const -> int {
  if (buf.size() > INT_MAX)
    throw std::range_error(fmt::format("Input buffer size {} too large", buf.size()));

  return transfer(reinterpret_cast<unsigned char*>(const_cast<char*>(buf.data())), static_cast<int>(buf.size()),
                  info.write_ep);
}

auto DevHandle::Impl::recv(std::string& resp) const -> int {
  auto len = (resp.capacity() > INT_MAX) ? INT_MAX : static_cast<int>(resp.capacity());

  return transfer(reinterpret_cast<unsigned char*>(resp.data()), len, info.read_ep);
}

auto DevHandle::Impl::recv(char* const resp, int const max_resp_size) const -> int {
  return transfer(reinterpret_cast<unsigned char*>(resp), max_resp_size, info.read_ep);
}

auto DevHandle::Impl::recv(int const max_resp_size) const -> std::string {
  auto resp = std::string();
  resp.reserve(max_resp_size);

  auto const received = transfer(reinterpret_cast<unsigned char*>(resp.data()), max_resp_size, info.read_ep);
  resp.resize(received);

  return resp;
}

auto DevHandle::Impl::lctx() const noexcept -> libusb_context* { return ctx_.lctx(); }

} // namespace polhemus
