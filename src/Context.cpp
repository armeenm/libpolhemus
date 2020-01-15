#include "Context.h"

#include "libpolhemus.hpp"

namespace polhemus {

Context::Context() {
    int ret = libusb_init(&lctx_);
    if (ret) throw std::runtime_error("Failed to create context");
}

Context::~Context() { libusb_exit(lctx_); }

libusb_context* Context::lctx() const noexcept { return lctx_; }

}  // namespace polhemus
