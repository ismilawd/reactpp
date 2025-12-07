#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"
#include <string>
#include <functional>

namespace reactpp {
namespace elements {

// Input element
inline VNode::Ptr Input(const Props& props = Props()) {
    return VNode::createElement("Input", props, {});
}

// Convenience function for input with onChange
inline VNode::Ptr Input(
    std::function<void(const std::string&)> onChange,
    const std::string& value = "",
    const Props& additionalProps = Props()) {
    Props props = additionalProps;
    props.set("onChange", onChange);
    props.set("value", value);
    return VNode::createElement("Input", props, {});
}

} // namespace elements
} // namespace reactpp

