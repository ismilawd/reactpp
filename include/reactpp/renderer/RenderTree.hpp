#pragma once

#include "reactpp/core/VNode.hpp"
#include "reactpp/core/FiberNode.hpp"
#include "LayoutEngine.hpp"
#include <unordered_map>

namespace reactpp {
namespace renderer {

class RenderTreeNode {
public:
    VNode::Ptr vnode;
    Rect layout;
    Props computedStyles;
    bool needsRepaint;
    
    RenderTreeNode(VNode::Ptr vnode);
};

class RenderTree {
public:
    RenderTree();
    
    // Update render tree from VNode tree
    void update(VNode::Ptr root);
    
    // Mark dirty for layout
    void markDirty(uint64_t nodeId);
    
    // Get render node
    std::shared_ptr<RenderTreeNode> getNode(uint64_t nodeId) const;
    
private:
    std::unordered_map<uint64_t, std::shared_ptr<RenderTreeNode>> nodes_;
    LayoutEngine layoutEngine_;
};

} // namespace renderer
} // namespace reactpp

