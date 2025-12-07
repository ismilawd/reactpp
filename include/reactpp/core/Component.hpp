#pragma once

#include "VNode.hpp"
#include "Props.hpp"
#include <memory>
#include <string>
#include <atomic>

namespace reactpp {

class HookManager;

class Component {
public:
    using Ptr = std::shared_ptr<Component>;
    
    Component();
    virtual ~Component() = default;
    
    // Pure virtual render method
    virtual VNode::Ptr render() = 0;
    
    // Lifecycle methods (can be overridden)
    virtual void onCreate() {}
    virtual void onMount() {}
    virtual void onUpdate(const Props& oldProps, const Props& newProps) {}
    virtual void onUnmount() {}
    
    // Props access
    const Props& getProps() const { return props_; }
    Props& getProps() { return props_; }
    void setProps(const Props& props) { props_ = props; }
    
    // Hook manager access
    std::shared_ptr<HookManager> getHookManager() const { return hookManager_; }
    void setHookManager(std::shared_ptr<HookManager> manager) { hookManager_ = manager; }
    
    // Component ID
    uint64_t getId() const { return id_; }
    
    // Display name for debugging
    const std::string& getDisplayName() const { return displayName_; }
    void setDisplayName(const std::string& name) { displayName_ = name; }
    
protected:
    Props props_;
    
private:
    static std::atomic<uint64_t> next_id_;
    uint64_t id_;
    std::string displayName_;
    std::shared_ptr<HookManager> hookManager_;
};

} // namespace reactpp

