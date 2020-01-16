#include "Context.h"
#include "libpolhemus.hpp"

namespace polhemus {

auto context() -> std::unique_ptr<Context> { return std::unique_ptr<Context>(new Context); }

} // namespace polhemus
