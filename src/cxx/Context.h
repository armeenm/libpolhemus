#pragma once

#include <libusb-1.0/libusb.h>

#include "libpolhemus.hpp"

namespace polhemus {

class Context {
public:
  Context();

  Context(Context const&) = default;
  Context(Context&&) noexcept = default;

  auto operator=(Context const&) -> Context& = default;
  auto operator=(Context&&) noexcept -> Context& = default;

  ~Context();

  [[nodiscard]] auto lctx() const noexcept -> libusb_context*;

private:
  libusb_context* lctx_;
};

} // namespace polhemus
