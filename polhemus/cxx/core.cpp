#include "polhemus.hpp"
#include "polhemus/cxx/Context.h"

namespace polhemus {

auto context() -> std::unique_ptr<Context> { return std::make_unique<Context>(); }

} // namespace polhemus
