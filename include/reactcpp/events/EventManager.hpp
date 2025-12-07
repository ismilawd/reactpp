#pragma once

#include "EventDispatcher.hpp"
#include "reactcpp/core/VNode.hpp"
#include <unordered_map>
#include <vector>

namespace reactcpp {
namespace events {

class EventManager {
public:
    EventManager();
    
    // Register event handler for VNode
    void registerHandler(uint64_t nodeId, const std::string& eventType, EventDispatcher::Handler handler);
    
    // Remove event handler
    void unregisterHandler(uint64_t nodeId, const std::string& eventType);
    
    // Get handlers for node and event type
    std::vector<EventDispatcher::Handler> getHandlers(uint64_t nodeId, const std::string& eventType) const;
    
    // Cleanup handlers for node
    void cleanupNode(uint64_t nodeId);
    
private:
    using HandlerKey = std::pair<uint64_t, std::string>;
    std::unordered_map<HandlerKey, std::vector<EventDispatcher::Handler>> handlers_;
};

} // namespace events
} // namespace reactcpp

