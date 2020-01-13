#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace polhemus {

namespace internal {
#include "libpolhemus_common.h"
}  // namespace internal

using DevType = internal::libpolhemus_device_type;
using Buffer = internal::libpolhemus_buffer;

class DevHandle {
   public:
    explicit DevHandle(DevType type, unsigned int timeout = 50);
    ~DevHandle();
    DevHandle(const DevHandle&) = delete;
    DevHandle& operator=(const DevHandle&) = delete;
    DevHandle(DevHandle&&) = default;
    DevHandle& operator=(DevHandle&&) = default;

    [[nodiscard]] DevType dev_type() const noexcept;
    [[nodiscard]] const std::string& name() const noexcept;
    [[nodiscard]] std::uint16_t vid() const noexcept;
    [[nodiscard]] std::uint16_t pid() const noexcept;
    [[nodiscard]] unsigned char write_ep() const noexcept;
    [[nodiscard]] unsigned char read_ep() const noexcept;
    [[nodiscard]] unsigned int timeout() const noexcept;

    void timeout(unsigned int timeout) noexcept;

    bool check_connection(unsigned int attempts = 10) const noexcept;

    int send_cmd(const Buffer& cmd, Buffer* resp) const noexcept;

    int send_raw(const Buffer& buf) const noexcept;
    int recv_raw(Buffer* buf) const noexcept;

   private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace polhemus
