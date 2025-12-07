#pragma once

#include <vector>
#include <any>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <atomic>

namespace reactpp {

class Component;

class HookManager {
public:
    using Ptr = std::shared_ptr<HookManager>;
    
    HookManager();
    
    // Reset hook index (call at start of render)
    void reset();
    
    // Get current hook index
    size_t getCurrentIndex() const { return currentIndex_; }
    
    // Get hook state at index
    std::any& getHookState(size_t index);
    const std::any& getHookState(size_t index) const;
    
    // Set hook state at index (creates if needed)
    void setHookState(size_t index, const std::any& state);
    
    // Get number of hooks
    size_t getHookCount() const { return hooks_.size(); }
    
    // Validate hook call (check if index is valid)
    void validateHookCall(size_t expectedIndex);
    
    // Mark as dirty (needs re-render)
    void markDirty() { dirty_ = true; }
    bool isDirty() const { return dirty_; }
    void clearDirty() { dirty_ = false; }
    
    // Component association
    void setComponentId(uint64_t id) { componentId_ = id; }
    uint64_t getComponentId() const { return componentId_; }
    
private:
    std::vector<std::any> hooks_;
    size_t currentIndex_;
    bool dirty_;
    uint64_t componentId_;
};

} // namespace reactpp

