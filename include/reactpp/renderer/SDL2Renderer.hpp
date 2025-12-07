#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

namespace reactpp {
namespace renderer {

class SDL2Renderer {
public:
    SDL2Renderer();
    ~SDL2Renderer();
    
    // Window management
    bool createWindow(const std::string& title, int width, int height);
    void destroyWindow();
    
    // Rendering
    void clear();
    void present();
    
    // Drawing primitives
    void drawRect(int x, int y, int width, int height, uint32_t color);
    void fillRect(int x, int y, int width, int height, uint32_t color);
    void drawText(int x, int y, const std::string& text, uint32_t color);
    
    // Color utilities
    static uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
    
    // Getters
    SDL_Window* getWindow() const { return window_; }
    SDL_Renderer* getRenderer() const { return renderer_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    // Event handling
    bool pollEvent(SDL_Event& event);
    
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    int width_;
    int height_;
    bool initialized_;
    
    bool initializeSDL();
    void cleanup();
};

} // namespace renderer
} // namespace reactpp

