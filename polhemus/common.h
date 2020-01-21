#pragma once

#ifdef __cpp_lib_source_location
#include <source_location>
#else
#include <experimental/source_location>
#endif
#include <chrono>
#include <ctime>
#include <string_view>

#include "third_party/magic_enum.hpp"
#include <fmt/chrono.h>
#include <fmt/format.h>

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };

void log(LogLevel lvl, std::string_view msg,
         const std::experimental::source_location& location = std::experimental::source_location::current()) {
  auto now = std::time(nullptr);

  fmt::print(stderr, "[{:%Y-%m-%d}]: {}:{}:{}:{}: {}", *std::localtime(&now), magic_enum::enum_name(lvl),
             location.file_name(), location.function_name(), location.line(), msg);
}
