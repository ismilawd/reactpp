#pragma once

#include "reactcpp/core/FiberNode.hpp"
#include <queue>
#include <functional>

namespace reactcpp {

class UpdateScheduler {
public:
    using Ptr = std::shared_ptr<UpdateScheduler>;
    
    UpdateScheduler();
    
    // Schedule update
    void scheduleUpdate(FiberNode::Ptr fiber, Priority priority);
    
    // Process updates
    void processUpdates();
    
    // Batch updates
    void batchUpdates(std::function<void()> fn);
    
    // Flush batched updates
    void flushBatchedUpdates();
    
private:
    struct Update {
        FiberNode::Ptr fiber;
        Priority priority;
        uint64_t timestamp;
        
        bool operator<(const Update& other) const {
            if (priority != other.priority) {
                return static_cast<int>(priority) > static_cast<int>(other.priority);
            }
            return timestamp > other.timestamp;
        }
    };
    
    std::priority_queue<Update> updateQueue_;
    std::vector<std::function<void()>> batchedUpdates_;
    bool batching_;
};

} // namespace reactcpp

