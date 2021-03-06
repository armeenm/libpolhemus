#pragma once

#include "polhemus.hpp"
extern "C" {
#include "polhemus.h"
}

#include "common.h"
#include <type_traits>

/* Macro to ease checking and casting in conv
 * res##c pasting is to have unique variable names to avoid shadowing warnings
 */
#define PLHM_CONV_CHK_(cxx, c)                                                                                         \
  if constexpr (auto res##c = multi_magic_cast<cxx, libpolhemus_##c>(val); !std::is_null_pointer_v<decltype(res##c)>)  \
  return res##c

template <typename T, typename U> inline constexpr auto data_equal() {
  return std::is_same_v<std::remove_pointer_t<std::decay_t<T>>, U>;
}

/* Cast according to pointerness */
template <typename Target, typename Input> constexpr auto magic_cast(Input val) {
  if constexpr (std::is_pointer_v<Input>)
    return reinterpret_cast<std::add_pointer_t<Target>>(val);
  else
    return static_cast<Target>(val);
}

/* Call inverted magic_cast based on which Target the Input is */
template <typename Target1, typename Target2, typename Input>
constexpr auto multi_magic_cast([[maybe_unused]] Input val) {
  if constexpr (data_equal<Input, Target1>())
    return magic_cast<Target2>(val);
  else if constexpr (data_equal<Input, Target2>())
    return magic_cast<Target1>(val);
  else
    return nullptr;
}

/* Automagically convert C API types to C++ API types and vice-versa */
template <typename T> constexpr auto conv(T val) {
  using namespace polhemus;

  PLHM_CONV_CHK_(Context, context);
  else PLHM_CONV_CHK_(DevHandle, device_handle);
  else PLHM_CONV_CHK_(DevType, device_type);
  else static_assert(always_false<T>, "Cannot convert unknown type");
}

#undef PLHM_CONV_CHK_
