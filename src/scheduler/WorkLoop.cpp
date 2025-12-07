#include "reactcpp/scheduler/WorkLoop.hpp"

namespace reactcpp {

WorkLoop::WorkLoop(std::shared_ptr<UpdateScheduler> scheduler)
    : scheduler_(scheduler), running_(false), frameStartTime_(0) {
}

void WorkLoop::run() {
    running_ = true;
    // TODO: Implement main loop
}

void WorkLoop::stop() {
    running_ = false;
}

bool WorkLoop::work(FiberNode::Ptr fiber) {
    // TODO: Implement work processing
    (void)fiber;
    return false;
}

bool WorkLoop::shouldYield() const {
    // TODO: Check if time budget exceeded
    return false;
}

} // namespace reactcpp
