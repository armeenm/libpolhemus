#include "polhemus/cxx/context.h"

#include "polhemus.hpp"

#include <stdexcept>

namespace polhemus {

Context::Context() {
  int const ret = libusb_init(&lctx_);
  if (ret != 0)
    throw std::runtime_error("Failed to create context");
}

Context::~Context() { libusb_exit(lctx_); }

auto Context::lctx() const noexcept -> libusb_context* { return lctx_; }

} // namespace polhemus
