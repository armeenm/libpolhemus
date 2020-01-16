#include "Context.h"
#include "libpolhemus.hpp"

namespace polhemus {

std::unique_ptr<Context> context() {
    return std::unique_ptr<Context>(new Context);
}

}  // namespace polhemus
