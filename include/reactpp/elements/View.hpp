#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"
#include <string>
#include <vector>

namespace reactpp {
namespace elements {

// View - Container element
inline VNode::Ptr View(
    const Props& props = Props(),
    const std::vector<VNode::Ptr>& children = {}) {
    return VNode::createElement("View", props, children);
}

} // namespace elements
} // namespace reactpp

