#include "cxx/dev_handle_impl.h"

#include "cxx/context.h"
#include "cxx/lits.h"

#include <cstdint>
#include <fmt/format.h>

namespace polhemus {

// TODO: Make a templated function to convert to unsigned char*

DevHandle::Impl::Impl(libusb_context* const lctx_in, DevType const dtype,
                      unsigned int const timeout_in)
    : info{devinfos_[std::size_t(dtype)]}, type{dtype}, timeout{timeout_in}, lctx{lctx_in} {}

auto DevHandle::Impl::transfer(unsigned char* const buf, int const len,
                               unsigned char const ep) const -> int {
  int transferred;
  auto err =
      static_cast<libusb_error>(libusb_bulk_transfer(handle, ep, buf, len, &transferred, timeout));

  if (err < 0)
    throw std::runtime_error("Failed to transfer data: "s + libusb_strerror(err));

  return transferred;
}

auto DevHandle::Impl::send(std::string_view const buf) const -> int {
  if (buf.size() > INT_MAX)
    throw std::range_error(fmt::format("Input buffer size {} too large", buf.size()));

  return transfer(reinterpret_cast<unsigned char*>(const_cast<char*>(buf.data())),
                  static_cast<int>(buf.size()), info.write_ep);
}

auto DevHandle::Impl::recv(std::string& resp) const -> int {
  auto const len = (resp.capacity() > INT_MAX) ? INT_MAX : static_cast<int>(resp.capacity());

  return transfer(reinterpret_cast<unsigned char*>(resp.data()), len, info.read_ep);
}

auto DevHandle::Impl::recv(char* const resp, int const max_resp_size) const -> int {
  return transfer(reinterpret_cast<unsigned char*>(resp), max_resp_size, info.read_ep);
}

auto DevHandle::Impl::recv(int const max_resp_size) const -> std::string {
  auto resp = std::string();
  resp.reserve(max_resp_size);

  auto const received =
      transfer(reinterpret_cast<unsigned char*>(resp.data()), max_resp_size, info.read_ep);
  resp.resize(received);

  return resp;
}

} // namespace polhemus
