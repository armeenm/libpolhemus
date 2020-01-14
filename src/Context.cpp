#include "ContextImpl.h"

namespace polhemus {

Context::Context() {
    int ret = libusb_init(&lctx);
    if (ret) throw std::runtime_error("Failed to create context");
}

Context::~Context() { libusb_exit(lctx); }

}  // namespace polhemus
