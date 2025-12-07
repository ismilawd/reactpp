#pragma once

#include "SyntheticEvent.hpp"
#include "reactpp/core/VNode.hpp"
#include <functional>
#include <vector>

namespace reactpp {
namespace events {

class EventDispatcher {
public:
    using Handler = std::function<void(SyntheticEvent&)>;
    
    EventDispatcher();
    
    // Dispatch event through capture and bubble phases
    void dispatch(SyntheticEvent& event, VNode::Ptr target);
    
    // Register handler
    void addEventListener(VNode::Ptr node, const std::string& eventType, Handler handler);
    void removeEventListener(VNode::Ptr node, const std::string& eventType);
    
private:
    std::vector<VNode::Ptr> getEventPath(VNode::Ptr target);
};

} // namespace events
} // namespace reactpp

