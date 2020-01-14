#include "libpolhemus.hpp"

namespace polhemus {

std::unique_ptr<Context> context() { return new Context(); }

}  // namespace polhemus
