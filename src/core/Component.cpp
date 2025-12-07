#include "reactpp/core/Component.hpp"

namespace reactpp {

std::atomic<uint64_t> Component::next_id_{1};

Component::Component() 
    : id_(next_id_.fetch_add(1, std::memory_order_relaxed)),
      displayName_("Component") {
}

} // namespace reactpp

