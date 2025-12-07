#include "reactpp/ReactPP.hpp"
#ifdef __linux__
#include "reactpp/renderer/FramebufferRenderer.hpp"
#include <unistd.h>
#endif
#include <iostream>
#include <memory>
#include <string>
#include <SDL2/SDL.h>

using namespace reactpp;
using namespace reactpp::elements;
using reactpp::elements::props;

// Counter component (will use hooks when implemented)
class Counter : public Component {
private:
    int count_;

public:
    Counter() : count_(0) {}
    
    VNode::Ptr render() override {
        using namespace reactpp::renderer;
        
        return View(
            props()
                .width(800)
                .height(600),
            {
                Text("Count: " + std::to_string(count_),
                    props()
                        .x(50)
                        .y(50)
                        .fontSize(32)
                        .color(0x000000FF)  // Black color (RGBA format)
                ),
                Button(
                    props()
                        .onClick([this]() {
                            std::cout << "Button clicked" << std::endl;
                            count_++;
                            std::cout << "Count: " << count_ << std::endl;
                        })
                        .x(300)
                        .y(300)
                        .width(200)
                        .height(50),
                    {
                        Text("Increment")
                    }
                )
            }
        );
    }
};

int main(int argc, char* argv[]) {
    try {
        // Check for framebuffer mode (Linux only)
        bool useFramebuffer = false;
        std::string fbDevice = "/dev/fb0";
        
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--fb" || arg == "--framebuffer") {
#ifdef __linux__
                useFramebuffer = true;
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    fbDevice = argv[++i];
                }
#else
                std::cerr << "Error: Framebuffer mode is only available on Linux" << std::endl;
                return 1;
#endif
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Usage: " << argv[0] << " [options]\n"
                          << "Options:\n"
#ifdef __linux__
                          << "  --fb, --framebuffer [device]  Use framebuffer renderer (Linux only, default: /dev/fb0)\n"
#endif
                          << "  --help, -h                    Show this help message\n";
                return 0;
            }
        }
        
        std::shared_ptr<renderer::SDL2Renderer> sdlRenderer;
#ifdef __linux__
        std::shared_ptr<renderer::FramebufferRenderer> fbRenderer;
#endif
        
        bool running = true;
        bool needsRender = true;
        VNode::Ptr currentVNode = nullptr;
        
#ifdef __linux__
        if (useFramebuffer) {
            std::cout << "Initializing framebuffer renderer on " << fbDevice << std::endl;
            fbRenderer = std::make_shared<renderer::FramebufferRenderer>(fbDevice);
            
            if (!fbRenderer->createWindow("ReactPP Counter Example", 0, 0)) {
                std::cerr << "Failed to initialize framebuffer" << std::endl;
                return 1;
            }
            
            auto component = std::make_shared<Counter>();
            std::cout << "Counter component created (framebuffer mode)" << std::endl;
            std::cout << "Resolution: " << fbRenderer->getWidth() << "x" << fbRenderer->getHeight() << std::endl;
            
            // Framebuffer mode: render once and exit (or implement input handling separately)
            while (running) {
                if (needsRender || !currentVNode) {
                    currentVNode = component->render();
                    
                    fbRenderer->clear(fbRenderer->rgb(240, 240, 240));
                    fbRenderer->render(currentVNode);
                    fbRenderer->present();
                    
                    needsRender = false;
                    std::cout << "Rendered to framebuffer. Press Ctrl+C to exit." << std::endl;
                }
                
                // In framebuffer mode, we don't have SDL events
                // You would need to implement input handling via /dev/input or other methods
                // For now, just render once and wait
                usleep(100000); // 100ms
            }
        } else
#endif // __linux__
        {
            std::cout << "Initializing SDL2 renderer" << std::endl;
            sdlRenderer = std::make_shared<renderer::SDL2Renderer>();
            
            if (!sdlRenderer->createWindow("ReactPP Counter Example", 800, 600)) {
                std::cerr << "Failed to create window" << std::endl;
                return 1;
            }
            
            auto component = std::make_shared<Counter>();
            std::cout << "Counter component created" << std::endl;
            
            while (running) {
                // Render first to build layout cache
                if (needsRender || !currentVNode) {
                    currentVNode = component->render();
                    
                    sdlRenderer->clear(sdlRenderer->rgb(240, 240, 240));
                    sdlRenderer->render(currentVNode);
                    sdlRenderer->present();
                    
                    needsRender = false;
                }
                
                // Process events (handles both mouse and touch)
                SDL_Event event;
                while (sdlRenderer->pollEvent(event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                    } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
                        bool handled = false;
                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            handled = sdlRenderer->handleClick(event.button.x, event.button.y, currentVNode);
                        } else if (event.type == SDL_FINGERDOWN) {
                            int x = static_cast<int>(event.tfinger.x * sdlRenderer->getWidth());
                            int y = static_cast<int>(event.tfinger.y * sdlRenderer->getHeight());
                            handled = sdlRenderer->handleTouch(x, y, currentVNode);
                        }
                        
                        if (handled) {
                            needsRender = true;
                        }
                    }
                }
                
                SDL_Delay(16);
            }
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

