#include "polhemus.hpp"
#include "cxx/context.h"

namespace polhemus {

auto init() -> std::unique_ptr<Context> { return std::make_unique<Context>(); }

} // namespace polhemus
