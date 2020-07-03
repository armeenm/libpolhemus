#pragma once

/* Workaround for static_assert(false) */
template <typename...> constexpr std::false_type always_false{};
