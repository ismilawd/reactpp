#pragma once

#include <string>
#include <memory>

namespace reactpp {
namespace platform {

class Platform {
public:
    virtual ~Platform() = default;
    
    // Window management
    virtual bool createWindow(const std::string& title, int width, int height) = 0;
    virtual void destroyWindow() = 0;
    
    // Event handling
    virtual bool pollEvents() = 0;
    
    // Rendering
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
};

} // namespace platform
} // namespace reactpp

