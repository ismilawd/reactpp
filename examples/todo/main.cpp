#include "reactpp/ReactPP.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace reactpp;
using namespace reactpp::elements;

// Simple todo component (will use hooks when implemented)
class TodoApp : public Component {
private:
    std::vector<std::string> todos_;
    std::string inputText_;

public:
    VNode::Ptr render() override {
        Props inputProps;
        inputProps.set("value", inputText_);
        inputProps.set("onChange", [this](const std::string& value) {
            inputText_ = value;
        });
        
        Props addButtonProps;
        addButtonProps.set("onClick", [this]() {
            if (!inputText_.empty()) {
                todos_.push_back(inputText_);
                inputText_.clear();
            }
        });
        
        std::vector<VNode::Ptr> todoNodes;
        for (const auto& todo : todos_) {
            todoNodes.push_back(Text(todo));
        }
        
        return View({}, {
            Text("Todo App"),
            Input(inputProps),
            Button(addButtonProps, {Text("Add")}),
            View({}, todoNodes)
        });
    }
};

int main() {
    try {
        auto renderer = std::make_shared<renderer::SDL2Renderer>();
        
        if (!renderer->createWindow("ReactPP Todo Example", 800, 600)) {
            std::cerr << "Failed to create window" << std::endl;
            return 1;
        }
        
        auto component = std::make_shared<TodoApp>();
        auto vnode = component->render();
        
        std::cout << "Todo component created" << std::endl;
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

