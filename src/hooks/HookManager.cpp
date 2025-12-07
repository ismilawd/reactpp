#include "reactcpp/hooks/HookManager.hpp"
#include <stdexcept>

namespace reactcpp {

HookManager::HookManager()
    : currentIndex_(0), dirty_(false), componentId_(0) {
}

void HookManager::reset() {
    currentIndex_ = 0;
}

std::any& HookManager::getHookState(size_t index) {
    if (index >= hooks_.size()) {
        hooks_.resize(index + 1);
    }
    return hooks_[index];
}

const std::any& HookManager::getHookState(size_t index) const {
    if (index >= hooks_.size()) {
        throw std::runtime_error("Hook index out of range: " + std::to_string(index));
    }
    return hooks_[index];
}

void HookManager::setHookState(size_t index, const std::any& state) {
    if (index >= hooks_.size()) {
        hooks_.resize(index + 1);
    }
    hooks_[index] = state;
}

void HookManager::validateHookCall(size_t expectedIndex) {
    if (currentIndex_ != expectedIndex) {
        throw std::runtime_error(
            "Hook call order violation: expected index " + 
            std::to_string(expectedIndex) + 
            " but got " + 
            std::to_string(currentIndex_)
        );
    }
}

} // namespace reactcpp

