#pragma once

#include <string>
#include <cstdint>

namespace reactcpp {
namespace events {

class SyntheticEvent {
public:
    std::string type;
    uint64_t timestamp;
    bool defaultPrevented;
    bool propagationStopped;
    
    SyntheticEvent(const std::string& eventType);
    
    void preventDefault();
    void stopPropagation();
    void persist();
    
    bool isPersistent() const { return persistent_; }
    
private:
    bool persistent_;
};

} // namespace events
} // namespace reactcpp

