#pragma once

#include "VNode.hpp"
#include "Component.hpp"
#include <memory>
#include <vector>
#include <optional>

namespace reactcpp {

enum class EffectTag {
    None,
    Placement,  // Insert new node
    Update,     // Update existing node
    Deletion    // Remove node
};

enum class Priority {
    Immediate,
    UserBlocking,
    Normal,
    Low,
    Idle
};

class FiberNode : public std::enable_shared_from_this<FiberNode> {
public:
    using Ptr = std::shared_ptr<FiberNode>;
    
    // Node type
    VNodeType type;
    
    // State node (reference to actual DOM or component instance)
    std::any stateNode;
    
    // Tree structure
    Ptr return_;  // Parent fiber
    Ptr child;    // First child fiber
    Ptr sibling;  // Next sibling fiber
    Ptr alternate; // Alternate fiber for double-buffering
    
    // Work tags
    EffectTag effectTag;
    std::vector<Ptr> effectList; // Effects to apply in commit phase
    
    // Props and state
    Props memoizedProps;
    Props pendingProps;
    std::any memoizedState;
    
    // Update queue
    std::vector<std::any> updateQueue;
    
    // Priority
    Priority priority;
    
    // Reference to VNode
    VNode::Ptr vnode;
    
    // Component reference
    std::shared_ptr<Component> component;
    
    FiberNode();
    static Ptr create(VNode::Ptr vnode);
    
    // Tree operations
    void appendChild(Ptr child);
    Ptr findChildByKey(const std::string& key) const;
    
    // Work phase helpers
    void markForUpdate(EffectTag tag);
    void addToEffectList(Ptr fiber);
};

} // namespace reactcpp

