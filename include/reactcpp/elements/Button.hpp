#pragma once

#include "reactcpp/core/VNode.hpp"
#include "reactcpp/core/Props.hpp"
#include <string>
#include <vector>
#include <functional>

namespace reactcpp {
namespace elements {

// Button element
inline VNode::Ptr Button(
    const Props& props = Props(),
    const std::vector<VNode::Ptr>& children = {}) {
    return VNode::createElement("Button", props, children);
}

// Convenience function for button with onClick
inline VNode::Ptr Button(
    std::function<void()> onClick,
    const std::vector<VNode::Ptr>& children = {}) {
    Props props;
    props.set("onClick", onClick);
    return VNode::createElement("Button", props, children);
}

} // namespace elements
} // namespace reactcpp

