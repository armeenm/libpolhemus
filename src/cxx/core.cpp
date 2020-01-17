#include "Context.h"
#include "polhemus.hpp"

namespace polhemus {

auto context() -> std::unique_ptr<Context> { return std::make_unique<Context>(); }

} // namespace polhemus
