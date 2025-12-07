#include "reactpp/events/EventManager.hpp"

namespace reactpp {
namespace events {

EventManager::EventManager() {
}

void EventManager::registerHandler(uint64_t nodeId, const std::string& eventType, EventDispatcher::Handler handler) {
    HandlerKey key(nodeId, eventType);
    handlers_[key].push_back(handler);
}

void EventManager::unregisterHandler(uint64_t nodeId, const std::string& eventType) {
    HandlerKey key(nodeId, eventType);
    handlers_.erase(key);
}

std::vector<EventDispatcher::Handler> EventManager::getHandlers(uint64_t nodeId, const std::string& eventType) const {
    HandlerKey key(nodeId, eventType);
    auto it = handlers_.find(key);
    if (it != handlers_.end()) {
        return it->second;
    }
    return {};
}

void EventManager::cleanupNode(uint64_t nodeId) {
    // Remove all handlers for this node
    auto it = handlers_.begin();
    while (it != handlers_.end()) {
        if (it->first.first == nodeId) {
            it = handlers_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace events
} // namespace reactpp
