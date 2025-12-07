#include "reactcpp/ReactCPP.hpp"
#include <iostream>
#include <memory>

using namespace reactcpp;
using namespace reactcpp::elements;

// Simple component example
class HelloWorld : public Component {
public:
    VNode::Ptr render() override {
        return View({}, {
            Text("Hello, World!"),
            Text("This is a ReactCPP example")
        });
    }
};

int main() {
    try {
        // Create renderer
        auto renderer = std::make_shared<renderer::SDL2Renderer>();
        
        if (!renderer->createWindow("ReactCPP Simple Example", 800, 600)) {
            std::cerr << "Failed to create window" << std::endl;
            return 1;
        }
        
        // Create component
        auto component = std::make_shared<HelloWorld>();
        auto vnode = component->render();
        
        std::cout << "Virtual DOM tree:" << std::endl;
        std::cout << vnode->serialize() << std::endl;
        
        // Simple render loop
        bool running = true;
        SDL_Event event;
        
        while (running) {
            while (renderer->pollEvent(event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }
            }
            
            renderer->clear();
            // TODO: Render VNode tree
            renderer->present();
            
            SDL_Delay(16); // ~60 FPS
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

