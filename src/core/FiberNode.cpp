#include "reactcpp/core/FiberNode.hpp"

namespace reactcpp {

FiberNode::FiberNode()
    : type(VNodeType::Element),
      effectTag(EffectTag::None),
      priority(Priority::Normal) {
}

FiberNode::Ptr FiberNode::create(VNode::Ptr vnode) {
    auto fiber = std::make_shared<FiberNode>();
    fiber->vnode = vnode;
    if (vnode) {
        fiber->type = vnode->getType();
        fiber->memoizedProps = vnode->getProps();
        fiber->pendingProps = vnode->getProps();
    }
    return fiber;
}

void FiberNode::appendChild(Ptr child) {
    if (!child) return;
    
    child->return_ = shared_from_this();
    
    if (!this->child) {
        this->child = child;
    } else {
        Ptr current = this->child;
        while (current->sibling) {
            current = current->sibling;
        }
        current->sibling = child;
    }
}

FiberNode::Ptr FiberNode::findChildByKey(const std::string& key) const {
    Ptr current = child;
    while (current) {
        if (current->vnode && current->vnode->getKey() && 
            *current->vnode->getKey() == key) {
            return current;
        }
        current = current->sibling;
    }
    return nullptr;
}

void FiberNode::markForUpdate(EffectTag tag) {
    effectTag = tag;
}

void FiberNode::addToEffectList(Ptr fiber) {
    if (fiber && fiber->effectTag != EffectTag::None) {
        effectList.push_back(fiber);
    }
}

} // namespace reactcpp

