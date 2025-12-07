#include "reactcpp/scheduler/UpdateScheduler.hpp"

namespace reactcpp {

UpdateScheduler::UpdateScheduler() : batching_(false) {
}

void UpdateScheduler::scheduleUpdate(FiberNode::Ptr fiber, Priority priority) {
    Update update;
    update.fiber = fiber;
    update.priority = priority;
    update.timestamp = 0; // TODO: Get actual timestamp
    updateQueue_.push(update);
}

void UpdateScheduler::processUpdates() {
    while (!updateQueue_.empty()) {
        Update update = updateQueue_.top();
        updateQueue_.pop();
        // TODO: Process update
    }
}

void UpdateScheduler::batchUpdates(std::function<void()> fn) {
    batching_ = true;
    fn();
    batching_ = false;
    flushBatchedUpdates();
}

void UpdateScheduler::flushBatchedUpdates() {
    for (auto& update : batchedUpdates_) {
        update();
    }
    batchedUpdates_.clear();
}

} // namespace reactcpp
