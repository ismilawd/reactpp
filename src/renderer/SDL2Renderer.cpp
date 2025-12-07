#include "reactpp/renderer/SDL2Renderer.hpp"
#include <stdexcept>

namespace reactpp {
namespace renderer {

SDL2Renderer::SDL2Renderer()
    : window_(nullptr), renderer_(nullptr), width_(0), height_(0), initialized_(false) {
    if (!initializeSDL()) {
        throw std::runtime_error("Failed to initialize SDL2");
    }
}

SDL2Renderer::~SDL2Renderer() {
    cleanup();
}

bool SDL2Renderer::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }
    initialized_ = true;
    return true;
}

void SDL2Renderer::cleanup() {
    destroyWindow();
    if (initialized_) {
        SDL_Quit();
        initialized_ = false;
    }
}

bool SDL2Renderer::createWindow(const std::string& title, int width, int height) {
    if (window_) {
        destroyWindow();
    }
    
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    
    if (!window_) {
        return false;
    }
    
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        return false;
    }
    
    width_ = width;
    height_ = height;
    
    return true;
}

void SDL2Renderer::destroyWindow() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    width_ = 0;
    height_ = 0;
}

void SDL2Renderer::clear() {
    if (renderer_) {
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);
    }
}

void SDL2Renderer::present() {
    if (renderer_) {
        SDL_RenderPresent(renderer_);
    }
}

void SDL2Renderer::drawRect(int x, int y, int width, int height, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;
    uint8_t a = color & 0xFF;
    
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderDrawRect(renderer_, &rect);
}

void SDL2Renderer::fillRect(int x, int y, int width, int height, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r = (color >> 24) & 0xFF;
    uint8_t g = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;
    uint8_t a = color & 0xFF;
    
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer_, &rect);
}

void SDL2Renderer::drawText(int x, int y, const std::string& text, uint32_t color) {
    // Basic text rendering - can be extended with SDL_ttf later
    // For now, this is a placeholder
    (void)x;
    (void)y;
    (void)text;
    (void)color;
}

uint32_t SDL2Renderer::rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (static_cast<uint32_t>(r) << 24) |
           (static_cast<uint32_t>(g) << 16) |
           (static_cast<uint32_t>(b) << 8) |
           static_cast<uint32_t>(a);
}

uint32_t SDL2Renderer::rgb(uint8_t r, uint8_t g, uint8_t b) {
    return rgba(r, g, b, 255);
}

bool SDL2Renderer::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event) != 0;
}

} // namespace renderer
} // namespace reactpp

