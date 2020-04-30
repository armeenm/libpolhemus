#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace polhemus {

class DevHandle;
class Context;

enum class DevType { PATRIOT, PATRIOT_HS };

[[nodiscard]] auto init() -> std::unique_ptr<Context>;

class DevHandle {
public:
  DevHandle(Context*, DevType);
  DevHandle(Context*, DevType, unsigned int timeout);

  DevHandle(DevHandle const&) = delete;
  DevHandle(DevHandle&&) noexcept = default;

  auto operator=(DevHandle const&) -> DevHandle& = delete;
  auto operator=(DevHandle&&) noexcept -> DevHandle& = default;

  ~DevHandle();

  [[nodiscard]] auto dev_type() const noexcept -> DevType;
  [[nodiscard]] auto name() const noexcept -> std::string_view;
  [[nodiscard]] auto timeout() const noexcept -> unsigned int;

  auto timeout(unsigned int timeout) noexcept -> void;

  auto check_connection() const noexcept -> bool;
  auto check_connection(unsigned int attempts) const noexcept -> bool;

  auto send_cmd(std::string_view cmd, std::string& resp) const -> int;
  [[nodiscard]] auto send_cmd(std::string_view cmd) const -> std::string;
  [[nodiscard]] auto send_cmd(std::string_view cmd, int max_resp_size) const -> std::string;
  auto send_cmd(std::string_view cmd, char* resp, int max_resp_size) const -> int;

  auto send_raw(std::string_view buf) const -> int;

  auto recv_raw(std::string& buf) const -> int;
  auto recv_raw(int max_size) const -> std::string;
  auto recv_raw(char* resp, int max_resp_size) const -> int;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace polhemus
