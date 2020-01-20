#include "polhemus/cxx/DevHandleImpl.h"

#include "polhemus/cxx/Context.h"
#include "polhemus/cxx/lits.h"

#include <cstdint>
#include <fmt/format.h>

namespace polhemus {

DevHandle::Impl::Impl(Context* ctx, DevType const type, unsigned int const timeout_in)
    : info(dev_type_info_map_.at(type)), timeout(timeout_in), ctx_(ctx ? *ctx : Context()) {}

auto DevHandle::Impl::transfer(unsigned char* const buf, int const len, unsigned char const ep) const -> int {
  int transferred;
  auto err = static_cast<libusb_error>(libusb_bulk_transfer(handle, ep, buf, len, &transferred, timeout));

  if (err < 0)
    throw std::runtime_error("Failed to transfer data: "s + libusb_strerror(err));

  return transferred;
}

auto DevHandle::Impl::recv(std::string* const buf) const -> int {
  auto len = (buf->length() > INT_MAX) ? INT_MAX : static_cast<int>(buf->length());

  return transfer(reinterpret_cast<unsigned char*>(buf->data()), len, info.read_ep);
}

auto DevHandle::Impl::recv(int const max_size) const -> std::pair<std::string, int> {
  auto resp = std::string();
  resp.reserve(max_size);

  auto const received = transfer(reinterpret_cast<unsigned char*>(resp.data()), max_size, info.read_ep);

  return {resp, received};
}

auto DevHandle::Impl::send(std::string_view const buf) const -> int {
  if (buf.size() > INT_MAX)
    throw std::range_error(fmt::format("Input buffer size {} too large", buf.size()));

  return transfer(reinterpret_cast<unsigned char*>(const_cast<char*>(buf.data())), buf.size(), info.write_ep);
}

auto DevHandle::Impl::lctx() const noexcept -> libusb_context* { return ctx_.lctx(); }

std::unordered_map<DevType, DevHandle::Impl::DevInfo> const DevHandle::Impl::dev_type_info_map_ = {
    {DevType::PATRIOT, {DevType::PATRIOT, "Patriot", 0x0f44, 0xef12, 0x02, 0x82}},
    {DevType::PATRIOT_HS, {DevType::PATRIOT_HS, "High Speed Patriot", 0x0f44, 0xef20, 0x04, 0x88}},
};

} // namespace polhemus
