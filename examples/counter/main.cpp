#include "reactpp/ReactPP.hpp"
#include <iostream>
#include <memory>
#include <string>

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
                        .color(SDL2Renderer::rgb(0, 0, 0))
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

int main() {
    try {
        auto renderer = std::make_shared<renderer::SDL2Renderer>();
        
        if (!renderer->createWindow("ReactPP Counter Example", 800, 600)) {
            std::cerr << "Failed to create window" << std::endl;
            return 1;
        }
        
        auto component = std::make_shared<Counter>();
        
        std::cout << "Counter component created" << std::endl;
        
        bool running = true;
        bool needsRender = true;
        VNode::Ptr currentVNode = nullptr;
        
        while (running) {
            // Render first to build layout cache
            if (needsRender || !currentVNode) {
                currentVNode = component->render();
                
                renderer->clear(renderer->rgb(240, 240, 240)); // Light gray background
                renderer->render(currentVNode);
                renderer->present();
                
                needsRender = false;
            }
            
            // Process events (handles both mouse and touch)
            SDL_Event event;
            while (renderer->pollEvent(event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_FINGERDOWN) {
                    // Use the already-rendered VNode tree for hit testing
                    // (layout cache is still valid from the render call above)
                    
                    // Handle click/touch
                    bool handled = false;
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        handled = renderer->handleClick(event.button.x, event.button.y, currentVNode);
                    } else if (event.type == SDL_FINGERDOWN) {
                        int x = static_cast<int>(event.tfinger.x * renderer->getWidth());
                        int y = static_cast<int>(event.tfinger.y * renderer->getHeight());
                        handled = renderer->handleTouch(x, y, currentVNode);
                    }
                    
                    if (handled) {
                        needsRender = true; // State changed, need to re-render
                    }
                }
            }
            
            SDL_Delay(16);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

