#pragma once

#include "reactcpp/core/FiberNode.hpp"
#include "UpdateScheduler.hpp"
#include <functional>

namespace reactcpp {

class WorkLoop {
public:
    WorkLoop(std::shared_ptr<UpdateScheduler> scheduler);
    
    // Main work loop
    void run();
    void stop();
    
    // Process single unit of work
    bool work(FiberNode::Ptr fiber);
    
    // Check if should yield
    bool shouldYield() const;
    
private:
    std::shared_ptr<UpdateScheduler> scheduler_;
    bool running_;
    uint64_t frameStartTime_;
    static constexpr uint64_t FRAME_BUDGET_MS = 16; // ~60 FPS
};

} // namespace reactcpp

