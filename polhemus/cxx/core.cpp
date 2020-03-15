#include "polhemus.hpp"
#include "polhemus/cxx/context.h"

namespace polhemus {

auto context() -> std::unique_ptr<Context> { return std::make_unique<Context>(); }

} // namespace polhemus
