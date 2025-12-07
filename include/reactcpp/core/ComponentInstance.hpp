#pragma once

#include "Component.hpp"
#include "VNode.hpp"
#include "reactcpp/hooks/HookManager.hpp"
#include <memory>
#include <unordered_map>
#include <atomic>

namespace reactcpp {

class ComponentInstanceManager {
public:
    using Ptr = std::shared_ptr<ComponentInstanceManager>;
    
    ComponentInstanceManager();
    
    // Create component instance
    std::shared_ptr<Component> createInstance(
        std::shared_ptr<Component> component,
        const Props& props = Props()
    );
    
    // Get instance by ID
    std::shared_ptr<Component> getInstance(uint64_t componentId) const;
    
    // Update component props
    void updateInstance(uint64_t componentId, const Props& newProps);
    
    // Destroy instance
    void destroyInstance(uint64_t componentId);
    
    // Get hook manager for component
    std::shared_ptr<HookManager> getHookManager(uint64_t componentId);
    
    // Lifecycle management
    void mountInstance(uint64_t componentId);
    void unmountInstance(uint64_t componentId);
    
    // Check if instance is mounted
    bool isMounted(uint64_t componentId) const;
    
private:
    struct InstanceData {
        std::shared_ptr<Component> component;
        std::shared_ptr<HookManager> hookManager;
        bool mounted;
    };
    
    std::unordered_map<uint64_t, InstanceData> instances_;
    std::atomic<uint64_t> nextInstanceId_;
};

} // namespace reactcpp

