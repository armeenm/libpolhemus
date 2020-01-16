#pragma once

#include <memory>
#include <string>

namespace polhemus {

extern "C" {
enum DevType { PATRIOT, PATRIOT_HS };

struct Buffer {
  unsigned char* data;
  int len;
};
}

class Context;

[[nodiscard]] auto context() -> std::unique_ptr<Context>;

class DevHandle {
public:
  DevHandle(Context* ctx, DevType type, unsigned int timeout = 50);

  DevHandle(DevHandle const&) = delete;
  DevHandle(DevHandle&&) noexcept = default;

  auto operator=(DevHandle const&) -> DevHandle& = delete;
  auto operator=(DevHandle&&) noexcept -> DevHandle& = default;

  ~DevHandle();

  [[nodiscard]] auto dev_type() const noexcept -> DevType;
  [[nodiscard]] auto name() const noexcept -> std::string const&;
  [[nodiscard]] auto timeout() const noexcept -> unsigned int;

  auto timeout(unsigned int timeout) noexcept -> void;

  auto check_connection(unsigned int attempts = 10) const noexcept -> bool;

  auto send_cmd(Buffer const& cmd, Buffer* resp) const noexcept -> int;

  auto send_raw(Buffer const& buf) const noexcept -> int;
  auto recv_raw(Buffer* buf) const noexcept -> int;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace polhemus
