#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"
#include <string>

namespace reactpp {
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
} // namespace reactpp

