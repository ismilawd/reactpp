#pragma once

#include "EventDispatcher.hpp"
#include "reactpp/core/VNode.hpp"
#include <unordered_map>
#include <vector>
#include <functional>

namespace reactpp {
namespace events {

// Hash function for std::pair<uint64_t, std::string>
struct HandlerKeyHash {
    std::size_t operator()(const std::pair<uint64_t, std::string>& key) const {
        std::size_t h1 = std::hash<uint64_t>{}(key.first);
        std::size_t h2 = std::hash<std::string>{}(key.second);
        // Combine hashes (from boost::hash_combine)
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

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
    std::unordered_map<HandlerKey, std::vector<EventDispatcher::Handler>, HandlerKeyHash> handlers_;
};

} // namespace events
} // namespace reactpp

