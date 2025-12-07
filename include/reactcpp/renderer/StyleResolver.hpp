#pragma once

#include "reactcpp/core/VNode.hpp"
#include "reactcpp/core/Props.hpp"
#include <unordered_map>
#include <string>

namespace reactcpp {
namespace renderer {

class StyleResolver {
public:
    StyleResolver();
    
    // Resolve styles for VNode
    Props resolveStyles(VNode::Ptr node);
    
    // Compute final style values
    void computeStyles(VNode::Ptr node);
    
private:
    Props defaultStyles_;
    std::unordered_map<uint64_t, Props> computedStyles_;
};

} // namespace renderer
} // namespace reactcpp

