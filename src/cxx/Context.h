#pragma once

#include <libusb-1.0/libusb.h>

#include <memory>

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

    [[nodiscard]] libusb_context* lctx() const noexcept;

   private:
    libusb_context* lctx_;
};

}  // namespace polhemus
