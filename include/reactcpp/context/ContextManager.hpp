#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

namespace reactcpp {

class Component;

class ContextManager {
public:
    using Ptr = std::shared_ptr<ContextManager>;
    
    ContextManager();
    
    // Provider registration
    void registerProvider(uint64_t contextId, uint64_t componentId);
    void unregisterProvider(uint64_t contextId, uint64_t componentId);
    
    // Find nearest provider
    uint64_t findProvider(uint64_t contextId, uint64_t componentId) const;
    
    // Consumer management
    void addConsumer(uint64_t providerId, uint64_t componentId);
    void removeConsumer(uint64_t providerId, uint64_t componentId);
    
    // Notify consumers
    void notifyConsumers(uint64_t providerId);
    
private:
    std::unordered_map<uint64_t, std::vector<uint64_t>> providers_;
    std::unordered_map<uint64_t, std::vector<uint64_t>> consumers_;
};

} // namespace reactcpp

