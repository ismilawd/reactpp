#include "reactcpp/events/SyntheticEvent.hpp"

namespace reactcpp {
namespace events {

SyntheticEvent::SyntheticEvent(const std::string& eventType)
    : type(eventType), timestamp(0), defaultPrevented(false),
      propagationStopped(false), persistent_(false) {
    // TODO: Get actual timestamp
}

void SyntheticEvent::preventDefault() {
    defaultPrevented = true;
}

void SyntheticEvent::stopPropagation() {
    propagationStopped = true;
}

void SyntheticEvent::persist() {
    persistent_ = true;
}

} // namespace events
} // namespace reactcpp
