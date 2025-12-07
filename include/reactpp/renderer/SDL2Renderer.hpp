#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"
#include "reactpp/renderer/RendererTypes.hpp"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

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
    void clear(uint32_t color);
    void present();
    
    // Drawing primitives - Rectangles
    void drawRect(int x, int y, int width, int height, uint32_t color);
    void fillRect(int x, int y, int width, int height, uint32_t color);
    void drawRect(const Rect& rect, uint32_t color);
    void fillRect(const Rect& rect, uint32_t color);
    
    // Drawing primitives - Circles/Ellipses
    void drawCircle(int centerX, int centerY, int radius, uint32_t color);
    void fillCircle(int centerX, int centerY, int radius, uint32_t color);
    void drawEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color);
    void fillEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color);
    
    // Drawing primitives - Lines
    void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    void drawLine(const Point& p1, const Point& p2, uint32_t color);
    void drawLines(const std::vector<Point>& points, uint32_t color);
    void drawPolygon(const std::vector<Point>& points, uint32_t color);
    void fillPolygon(const std::vector<Point>& points, uint32_t color);
    
    // Drawing primitives - Text
    void drawText(int x, int y, const std::string& text, uint32_t color);
    void drawText(int x, int y, const std::string& text, uint32_t color, int fontSize);
    void drawText(int x, int y, const std::string& text, uint32_t color, const std::string& fontPath, int fontSize);
    void getTextSize(const std::string& text, int fontSize, int& width, int& height);
    
    // Gradients
    void fillRectGradient(int x, int y, int width, int height, 
                         const std::vector<GradientStop>& stops, 
                         GradientDirection direction = GradientDirection::Horizontal);
    void fillRectGradient(const Rect& rect, 
                         const std::vector<GradientStop>& stops, 
                         GradientDirection direction = GradientDirection::Horizontal);
    
    // VNode rendering
    void render(VNode::Ptr root);
    
    // Color utilities
    static uint32_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
    static void unpackColor(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);
    
    // Getters
    SDL_Window* getWindow() const { return window_; }
    SDL_Renderer* getRenderer() const { return renderer_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    // Event handling
    bool pollEvent(SDL_Event& event);
    
    // Click/touch event handling
    VNode::Ptr findElementAt(int x, int y, VNode::Ptr root);
    bool handleClick(int x, int y, VNode::Ptr root);
    bool handleTouch(int x, int y, VNode::Ptr root);
    void processEvents(VNode::Ptr root);
    
    // Clear element layout cache (call before re-rendering)
    void clearLayoutCache();
    
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    TTF_Font* defaultFont_;
    int width_;
    int height_;
    bool initialized_;
    bool ttfInitialized_;
    
    // Element layout tracking for hit testing
    std::unordered_map<uint64_t, Rect> elementLayouts_;
    
    bool initializeSDL();
    void cleanup();
    TTF_Font* loadFont(const std::string& fontPath, int fontSize);
    void renderVNode(VNode::Ptr node, int offsetX = 0, int offsetY = 0);
    VNode::Ptr findElementAtRecursive(int x, int y, VNode::Ptr node);
    uint32_t getColorFromProps(const Props& props, const std::string& key, uint32_t defaultColor = 0xFFFFFFFF);
    Rect getRectFromProps(const Props& props, const Rect& defaultRect);
};

} // namespace renderer
} // namespace reactpp

