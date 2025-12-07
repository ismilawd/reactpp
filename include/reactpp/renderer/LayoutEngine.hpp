#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/renderer/RendererTypes.hpp"
#include <cstdint>
#include <unordered_map>

namespace reactpp {
namespace renderer {

enum class FlexDirection {
    Row,
    Column
};

enum class JustifyContent {
    Start,
    Center,
    End,
    SpaceBetween
};

enum class AlignItems {
    Start,
    Center,
    End,
    Stretch
};

class LayoutEngine {
public:
    LayoutEngine();
    
    // Calculate layout for VNode tree
    void calculateLayout(VNode::Ptr root, int containerWidth, int containerHeight);
    
    // Get computed layout for node
    Rect getLayout(uint64_t nodeId) const;
    
private:
    void layoutNode(VNode::Ptr node, int availableWidth, int availableHeight);
    std::unordered_map<uint64_t, Rect> layouts_;
};

} // namespace renderer
} // namespace reactpp

