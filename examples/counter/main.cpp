#include "reactcpp/ReactCPP.hpp"
#include <iostream>
#include <memory>
#include <string>

using namespace reactcpp;
using namespace reactcpp::elements;

// Counter component (will use hooks when implemented)
class Counter : public Component {
private:
    int count_;

public:
    Counter() : count_(0) {}
    
    VNode::Ptr render() override {
        Props buttonProps;
        buttonProps.set("onClick", [this]() {
            count_++;
            std::cout << "Count: " << count_ << std::endl;
        });
        
        return View({}, {
            Text("Count: " + std::to_string(count_)),
            Button(buttonProps, {
                Text("Increment")
            })
        });
    }
};

int main() {
    try {
        auto renderer = std::make_shared<renderer::SDL2Renderer>();
        
        if (!renderer->createWindow("ReactCPP Counter Example", 800, 600)) {
            std::cerr << "Failed to create window" << std::endl;
            return 1;
        }
        
        auto component = std::make_shared<Counter>();
        auto vnode = component->render();
        
        std::cout << "Counter component created" << std::endl;
        std::cout << vnode->serialize() << std::endl;
        
        bool running = true;
        SDL_Event event;
        
        while (running) {
            while (renderer->pollEvent(event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }
            
            renderer->clear();
            renderer->present();
            
            SDL_Delay(16);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

