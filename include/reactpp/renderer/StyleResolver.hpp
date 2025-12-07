#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"
#include <unordered_map>
#include <string>

namespace reactpp {
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
} // namespace reactpp

