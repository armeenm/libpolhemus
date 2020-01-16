#pragma once

#include <libusb-1.0/libusb.h>

#include "libpolhemus.hpp"

namespace polhemus {

class Context {
   public:
    Context();
    ~Context();
    Context(const Context&) = default;
    Context& operator=(const Context&) = default;
    Context(Context&&) noexcept = default;
    Context& operator=(Context&&) noexcept = default;

    [[nodiscard]] auto lctx() const noexcept -> libusb_context*;

   private:
    libusb_context* lctx_ = nullptr;
};

}  // namespace polhemus
