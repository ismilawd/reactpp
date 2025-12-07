#include "reactcpp/context/ContextManager.hpp"
#include <algorithm>

namespace reactcpp {

ContextManager::ContextManager() {
}

void ContextManager::registerProvider(uint64_t contextId, uint64_t componentId) {
    providers_[contextId].push_back(componentId);
}

void ContextManager::unregisterProvider(uint64_t contextId, uint64_t componentId) {
    auto& providers = providers_[contextId];
    providers.erase(
        std::remove(providers.begin(), providers.end(), componentId),
        providers.end()
    );
}

uint64_t ContextManager::findProvider(uint64_t contextId, uint64_t componentId) const {
    auto it = providers_.find(contextId);
    if (it == providers_.end() || it->second.empty()) {
        return 0;
    }
    return it->second.back(); // Return most recent provider
}

void ContextManager::addConsumer(uint64_t providerId, uint64_t componentId) {
    consumers_[providerId].push_back(componentId);
}

void ContextManager::removeConsumer(uint64_t providerId, uint64_t componentId) {
    auto& consumers = consumers_[providerId];
    consumers.erase(
        std::remove(consumers.begin(), consumers.end(), componentId),
        consumers.end()
    );
}

void ContextManager::notifyConsumers(uint64_t providerId) {
    // TODO: Implement consumer notification
    (void)providerId;
}

} // namespace reactcpp
