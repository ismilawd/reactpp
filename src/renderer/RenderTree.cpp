#include "reactpp/renderer/RenderTree.hpp"

namespace reactpp {
namespace renderer {

RenderTreeNode::RenderTreeNode(VNode::Ptr vnode)
    : vnode(vnode), needsRepaint(false) {
    layout = {0, 0, 0, 0};
}

RenderTree::RenderTree() {
}

void RenderTree::update(VNode::Ptr root) {
    // TODO: Update render tree from VNode tree
    (void)root;
}

void RenderTree::markDirty(uint64_t nodeId) {
    auto it = nodes_.find(nodeId);
    if (it != nodes_.end()) {
        it->second->needsRepaint = true;
    }
}

std::shared_ptr<RenderTreeNode> RenderTree::getNode(uint64_t nodeId) const {
    auto it = nodes_.find(nodeId);
    if (it != nodes_.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace renderer
} // namespace reactpp
