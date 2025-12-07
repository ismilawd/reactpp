#pragma once

#include "reactcpp/core/VNode.hpp"
#include "reactcpp/core/Props.hpp"
#include <string>

namespace reactcpp {
namespace elements {

// Text element
inline VNode::Ptr Text(const std::string& content, const Props& props = Props()) {
    auto textNode = VNode::createText(content);
    if (!props.empty()) {
        // For text nodes, we can store additional props in a wrapper element
        // or extend VNode to support props on text nodes
        auto wrapper = VNode::createElement("Text", props, {textNode});
        return wrapper;
    }
    return textNode;
}

} // namespace elements
} // namespace reactcpp

