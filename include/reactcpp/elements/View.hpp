#pragma once

#include "reactcpp/core/VNode.hpp"
#include "reactcpp/core/Props.hpp"
#include <string>
#include <vector>

namespace reactcpp {
namespace elements {

// View - Container element
inline VNode::Ptr View(
    const Props& props = Props(),
    const std::vector<VNode::Ptr>& children = {}) {
    return VNode::createElement("View", props, children);
}

} // namespace elements
} // namespace reactcpp

