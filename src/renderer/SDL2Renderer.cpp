#include "reactpp/renderer/SDL2Renderer.hpp"
#include "reactpp/core/Props.hpp"
#include "reactpp/elements/Elements.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace reactpp {
namespace renderer {

SDL2Renderer::SDL2Renderer()
    : window_(nullptr), renderer_(nullptr), defaultFont_(nullptr), 
      width_(0), height_(0), initialized_(false), ttfInitialized_(false) {
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
    
    if (TTF_Init() < 0) {
        // TTF is optional, continue without it
        ttfInitialized_ = false;
    } else {
        ttfInitialized_ = true;
    }
    
    return true;
}

void SDL2Renderer::cleanup() {
    destroyWindow();
    
    if (defaultFont_) {
        TTF_CloseFont(defaultFont_);
        defaultFont_ = nullptr;
    }
    
    if (ttfInitialized_) {
        TTF_Quit();
        ttfInitialized_ = false;
    }
    
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
    clear(0x000000FF); // Black
}

void SDL2Renderer::clear(uint32_t color) {
    if (renderer_) {
        uint8_t r, g, b, a;
        unpackColor(color, r, g, b, a);
        SDL_SetRenderDrawColor(renderer_, r, g, b, a);
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
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderDrawRect(renderer_, &rect);
}

void SDL2Renderer::fillRect(int x, int y, int width, int height, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer_, &rect);
}

void SDL2Renderer::drawRect(const Rect& rect, uint32_t color) {
    drawRect(rect.x, rect.y, rect.width, rect.height, color);
}

void SDL2Renderer::fillRect(const Rect& rect, uint32_t color) {
    fillRect(rect.x, rect.y, rect.width, rect.height, color);
}

void SDL2Renderer::drawCircle(int centerX, int centerY, int radius, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    // Draw circle using midpoint algorithm
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    auto drawCirclePoints = [this, centerX, centerY](int x, int y) {
        SDL_RenderDrawPoint(renderer_, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer_, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer_, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer_, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer_, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer_, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer_, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer_, centerX - y, centerY - x);
    };
    
    drawCirclePoints(x, y);
    
    while (x < y) {
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        drawCirclePoints(x, y);
    }
}

void SDL2Renderer::fillCircle(int centerX, int centerY, int radius, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    // Fill circle by drawing horizontal lines
    for (int y = -radius; y <= radius; y++) {
        int x = static_cast<int>(std::sqrt(radius * radius - y * y));
        SDL_RenderDrawLine(renderer_, centerX - x, centerY + y, centerX + x, centerY + y);
    }
}

void SDL2Renderer::drawEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    // Draw ellipse using parametric equation
    const int numPoints = 64;
    for (int i = 0; i < numPoints; i++) {
        double angle = 2.0 * M_PI * i / numPoints;
        int x = centerX + static_cast<int>(radiusX * std::cos(angle));
        int y = centerY + static_cast<int>(radiusY * std::sin(angle));
        
        if (i == 0) {
            SDL_RenderDrawPoint(renderer_, x, y);
        } else {
            double prevAngle = 2.0 * M_PI * (i - 1) / numPoints;
            int prevX = centerX + static_cast<int>(radiusX * std::cos(prevAngle));
            int prevY = centerY + static_cast<int>(radiusY * std::sin(prevAngle));
            SDL_RenderDrawLine(renderer_, prevX, prevY, x, y);
        }
    }
}

void SDL2Renderer::fillEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    // Fill ellipse by drawing horizontal lines
    for (int y = -radiusY; y <= radiusY; y++) {
        int x = static_cast<int>(radiusX * std::sqrt(1.0 - (double)(y * y) / (radiusY * radiusY)));
        SDL_RenderDrawLine(renderer_, centerX - x, centerY + y, centerX + x, centerY + y);
    }
}

void SDL2Renderer::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    if (!renderer_) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void SDL2Renderer::drawLine(const Point& p1, const Point& p2, uint32_t color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void SDL2Renderer::drawLines(const std::vector<Point>& points, uint32_t color) {
    if (!renderer_ || points.size() < 2) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    
    for (size_t i = 0; i < points.size() - 1; i++) {
        SDL_RenderDrawLine(renderer_, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
}

void SDL2Renderer::drawPolygon(const std::vector<Point>& points, uint32_t color) {
    if (points.size() < 3) return;
    
    drawLines(points, color);
    // Close the polygon
    if (points.size() > 2) {
        drawLine(points.back(), points.front(), color);
    }
}

void SDL2Renderer::fillPolygon(const std::vector<Point>& points, uint32_t color) {
    if (!renderer_ || points.size() < 3) return;
    
    // Convert to SDL points
    std::vector<SDL_Point> sdlPoints(points.size());
    for (size_t i = 0; i < points.size(); i++) {
        sdlPoints[i].x = points[i].x;
        sdlPoints[i].y = points[i].y;
    }
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
    SDL_RenderDrawLines(renderer_, sdlPoints.data(), static_cast<int>(sdlPoints.size()));
    SDL_RenderFillRects(renderer_, nullptr, 0); // SDL doesn't have direct polygon fill, use texture approach
    
    // Simple scanline fill (basic implementation)
    if (points.size() >= 3) {
        int minY = points[0].y, maxY = points[0].y;
        for (const auto& p : points) {
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
        
        for (int y = minY; y <= maxY; y++) {
            std::vector<int> intersections;
            for (size_t i = 0; i < points.size(); i++) {
                size_t j = (i + 1) % points.size();
                const Point& p1 = points[i];
                const Point& p2 = points[j];
                
                if ((p1.y < y && p2.y >= y) || (p2.y < y && p1.y >= y)) {
                    if (p1.y != p2.y) {
                        int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                        intersections.push_back(x);
                    }
                }
            }
            
            std::sort(intersections.begin(), intersections.end());
            for (size_t i = 0; i < intersections.size(); i += 2) {
                if (i + 1 < intersections.size()) {
                    SDL_RenderDrawLine(renderer_, intersections[i], y, intersections[i+1], y);
                }
            }
        }
    }
}

TTF_Font* SDL2Renderer::loadFont(const std::string& fontPath, int fontSize) {
    if (!ttfInitialized_) return nullptr;
    
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    return font;
}

void SDL2Renderer::drawText(int x, int y, const std::string& text, uint32_t color) {
    drawText(x, y, text, color, 16);
}

void SDL2Renderer::drawText(int x, int y, const std::string& text, uint32_t color, int fontSize) {
    drawText(x, y, text, color, "", fontSize);
}

void SDL2Renderer::drawText(int x, int y, const std::string& text, uint32_t color, const std::string& fontPath, int fontSize) {
    if (!renderer_ || !ttfInitialized_) return;
    
    TTF_Font* font = nullptr;
    if (!fontPath.empty()) {
        font = loadFont(fontPath, fontSize);
    } else {
        // Try to use default font or create one
        if (!defaultFont_) {
            // Try common system fonts
            const char* systemFonts[] = {
                "/System/Library/Fonts/Helvetica.ttc",
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                "C:/Windows/Fonts/arial.ttf",
                nullptr
            };
            
            for (int i = 0; systemFonts[i]; i++) {
                defaultFont_ = TTF_OpenFont(systemFonts[i], fontSize);
                if (defaultFont_) break;
            }
        }
        font = defaultFont_;
    }
    
    if (!font) return;
    
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    SDL_Color sdlColor = {r, g, b, a};
    
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), sdlColor);
    if (!surface) {
        if (font != defaultFont_ && font) {
            TTF_CloseFont(font);
        }
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    if (texture) {
        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer_, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
    }
    
    SDL_FreeSurface(surface);
    
    if (font != defaultFont_ && font) {
        TTF_CloseFont(font);
    }
}

void SDL2Renderer::getTextSize(const std::string& text, int fontSize, int& width, int& height) {
    width = 0;
    height = 0;
    
    if (!ttfInitialized_) return;
    
    if (!defaultFont_) {
        const char* systemFonts[] = {
            "/System/Library/Fonts/Helvetica.ttc",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "C:/Windows/Fonts/arial.ttf",
            nullptr
        };
        
        for (int i = 0; systemFonts[i]; i++) {
            defaultFont_ = TTF_OpenFont(systemFonts[i], fontSize);
            if (defaultFont_) break;
        }
    }
    
    if (!defaultFont_) return;
    
    TTF_SizeText(defaultFont_, text.c_str(), &width, &height);
}

void SDL2Renderer::fillRectGradient(int x, int y, int width, int height, 
                                   const std::vector<GradientStop>& stops, 
                                   GradientDirection direction) {
    if (!renderer_ || stops.size() < 2) return;
    
    // Sort stops by position
    std::vector<GradientStop> sortedStops = stops;
    std::sort(sortedStops.begin(), sortedStops.end(), 
              [](const GradientStop& a, const GradientStop& b) {
                  return a.position < b.position;
              });
    
    if (direction == GradientDirection::Horizontal) {
        for (int px = 0; px < width; px++) {
            float t = static_cast<float>(px) / width;
            
            // Find the two stops to interpolate between
            size_t stopIndex = 0;
            for (size_t i = 0; i < sortedStops.size() - 1; i++) {
                if (t >= sortedStops[i].position && t <= sortedStops[i+1].position) {
                    stopIndex = i;
                    break;
                }
            }
            
            if (stopIndex >= sortedStops.size() - 1) {
                stopIndex = sortedStops.size() - 2;
            }
            
            const GradientStop& stop1 = sortedStops[stopIndex];
            const GradientStop& stop2 = sortedStops[stopIndex + 1];
            
            float localT = (t - stop1.position) / (stop2.position - stop1.position);
            localT = std::max(0.0f, std::min(1.0f, localT));
            
            uint8_t r1, g1, b1, a1, r2, g2, b2, a2;
            unpackColor(stop1.color, r1, g1, b1, a1);
            unpackColor(stop2.color, r2, g2, b2, a2);
            
            uint8_t r = static_cast<uint8_t>(r1 + (r2 - r1) * localT);
            uint8_t g = static_cast<uint8_t>(g1 + (g2 - g1) * localT);
            uint8_t b = static_cast<uint8_t>(b1 + (b2 - b1) * localT);
            uint8_t a = static_cast<uint8_t>(a1 + (a2 - a1) * localT);
            
            SDL_SetRenderDrawColor(renderer_, r, g, b, a);
            SDL_RenderDrawLine(renderer_, x + px, y, x + px, y + height);
        }
    } else if (direction == GradientDirection::Vertical) {
        for (int py = 0; py < height; py++) {
            float t = static_cast<float>(py) / height;
            
            size_t stopIndex = 0;
            for (size_t i = 0; i < sortedStops.size() - 1; i++) {
                if (t >= sortedStops[i].position && t <= sortedStops[i+1].position) {
                    stopIndex = i;
                    break;
                }
            }
            
            if (stopIndex >= sortedStops.size() - 1) {
                stopIndex = sortedStops.size() - 2;
            }
            
            const GradientStop& stop1 = sortedStops[stopIndex];
            const GradientStop& stop2 = sortedStops[stopIndex + 1];
            
            float localT = (t - stop1.position) / (stop2.position - stop1.position);
            localT = std::max(0.0f, std::min(1.0f, localT));
            
            uint8_t r1, g1, b1, a1, r2, g2, b2, a2;
            unpackColor(stop1.color, r1, g1, b1, a1);
            unpackColor(stop2.color, r2, g2, b2, a2);
            
            uint8_t r = static_cast<uint8_t>(r1 + (r2 - r1) * localT);
            uint8_t g = static_cast<uint8_t>(g1 + (g2 - g1) * localT);
            uint8_t b = static_cast<uint8_t>(b1 + (b2 - b1) * localT);
            uint8_t a = static_cast<uint8_t>(a1 + (a2 - a1) * localT);
            
            SDL_SetRenderDrawColor(renderer_, r, g, b, a);
            SDL_RenderDrawLine(renderer_, x, y + py, x + width, y + py);
        }
    } else if (direction == GradientDirection::Radial) {
        int centerX = x + width / 2;
        int centerY = y + height / 2;
        float maxRadius = std::sqrt(width * width + height * height) / 2.0f;
        
        for (int py = 0; py < height; py++) {
            for (int px = 0; px < width; px++) {
                float dx = px - width / 2.0f;
                float dy = py - height / 2.0f;
                float distance = std::sqrt(dx * dx + dy * dy);
                float t = distance / maxRadius;
                t = std::max(0.0f, std::min(1.0f, t));
                
                size_t stopIndex = 0;
                for (size_t i = 0; i < sortedStops.size() - 1; i++) {
                    if (t >= sortedStops[i].position && t <= sortedStops[i+1].position) {
                        stopIndex = i;
                        break;
                    }
                }
                
                if (stopIndex >= sortedStops.size() - 1) {
                    stopIndex = sortedStops.size() - 2;
                }
                
                const GradientStop& stop1 = sortedStops[stopIndex];
                const GradientStop& stop2 = sortedStops[stopIndex + 1];
                
                float localT = (t - stop1.position) / (stop2.position - stop1.position);
                localT = std::max(0.0f, std::min(1.0f, localT));
                
                uint8_t r1, g1, b1, a1, r2, g2, b2, a2;
                unpackColor(stop1.color, r1, g1, b1, a1);
                unpackColor(stop2.color, r2, g2, b2, a2);
                
                uint8_t r = static_cast<uint8_t>(r1 + (r2 - r1) * localT);
                uint8_t g = static_cast<uint8_t>(g1 + (g2 - g1) * localT);
                uint8_t b = static_cast<uint8_t>(b1 + (b2 - b1) * localT);
                uint8_t a = static_cast<uint8_t>(a1 + (a2 - a1) * localT);
                
                SDL_SetRenderDrawColor(renderer_, r, g, b, a);
                SDL_RenderDrawPoint(renderer_, x + px, y + py);
            }
        }
    }
}

void SDL2Renderer::fillRectGradient(const Rect& rect, 
                                   const std::vector<GradientStop>& stops, 
                                   GradientDirection direction) {
    fillRectGradient(rect.x, rect.y, rect.width, rect.height, stops, direction);
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

void SDL2Renderer::unpackColor(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
    r = (color >> 24) & 0xFF;
    g = (color >> 16) & 0xFF;
    b = (color >> 8) & 0xFF;
    a = color & 0xFF;
}

uint32_t SDL2Renderer::getColorFromProps(const Props& props, const std::string& key, uint32_t defaultColor) {
    if (props.has(key)) {
        try {
            if (props.getType(key) == std::type_index(typeid(uint32_t))) {
                return props.get<uint32_t>(key);
            } else if (props.getType(key) == std::type_index(typeid(std::string))) {
                std::string colorStr = props.get<std::string>(key);
                // Simple hex color parser (e.g., "#FF0000" or "#FF0000FF")
                if (colorStr.size() >= 7 && colorStr[0] == '#') {
                    uint32_t color = 0;
                    for (size_t i = 1; i < colorStr.size() && i < 9; i++) {
                        char c = colorStr[i];
                        uint32_t val = 0;
                        if (c >= '0' && c <= '9') val = c - '0';
                        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
                        else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
                        else continue;
                        color = (color << 4) | val;
                    }
                    if (colorStr.size() == 7) {
                        // RGB, add alpha
                        color = (color << 8) | 0xFF;
                    }
                    return color;
                }
            }
        } catch (...) {
            // Fall through to default
        }
    }
    return defaultColor;
}

Rect SDL2Renderer::getRectFromProps(const Props& props, const Rect& defaultRect) {
    Rect rect = defaultRect;
    
    if (props.has("x")) {
        try {
            rect.x = props.get<int>("x");
        } catch (...) {}
    }
    if (props.has("y")) {
        try {
            rect.y = props.get<int>("y");
        } catch (...) {}
    }
    if (props.has("width")) {
        try {
            rect.width = props.get<int>("width");
        } catch (...) {}
    }
    if (props.has("height")) {
        try {
            rect.height = props.get<int>("height");
        } catch (...) {}
    }
    
    return rect;
}

void SDL2Renderer::renderVNode(VNode::Ptr node, int offsetX, int offsetY) {
    if (!node || !renderer_) return;
    
    switch (node->getType()) {
        case VNodeType::Element: {
            std::string tag = node->getTag();
            
            // Special handling for Button elements
            if (tag == "Button") {
                Rect layout = getRectFromProps(node->getProps(), {offsetX, offsetY, 150, 40});
                
                // Store layout for hit testing
                elementLayouts_[node->getId()] = layout;
                
                // Button styling
                uint32_t bgColor = getColorFromProps(node->getProps(), "backgroundColor", 0x4A90E2FF); // Blue
                uint32_t borderColor = getColorFromProps(node->getProps(), "borderColor", 0x357ABDFF); // Darker blue
                uint32_t textColor = getColorFromProps(node->getProps(), "color", 0xFFFFFFFF); // White text
                
                // Draw button background
                fillRect(layout, bgColor);
                
                // Draw button border
                drawRect(layout, borderColor);
                
                // Render button text/children
                int textX = layout.x + layout.width / 2;
                int textY = layout.y + layout.height / 2;
                for (const auto& child : node->getChildren()) {
                    if (child) {
                        if (child->getType() == VNodeType::Text) {
                            std::string text = child->getText();
                            if (!text.empty()) {
                                int fontSize = 14;
                                if (node->getProps().has("fontSize")) {
                                    try {
                                        fontSize = node->getProps().get<int>("fontSize");
                                    } catch (...) {}
                                }
                                int textW, textH;
                                getTextSize(text, fontSize, textW, textH);
                                drawText(textX - textW / 2, textY - textH / 2, text, textColor, fontSize);
                            }
                        } else {
                            renderVNode(child, layout.x + 10, layout.y + 10);
                        }
                    }
                }
                break;
            }
            
            // Special handling for Text wrapper elements
            if (tag == "Text") {
                // Text elements wrap text nodes, render the text
                for (const auto& child : node->getChildren()) {
                    if (child && child->getType() == VNodeType::Text) {
                        std::string text = child->getText();
                        if (!text.empty()) {
                            uint32_t textColor = getColorFromProps(node->getProps(), "color", 0x000000FF);
                            int fontSize = 16;
                            if (node->getProps().has("fontSize")) {
                                try {
                                    fontSize = node->getProps().get<int>("fontSize");
                                } catch (...) {}
                            }
                            
                            // Calculate text bounds for hit testing
                            int textW, textH;
                            getTextSize(text, fontSize, textW, textH);
                            Rect textLayout = {offsetX, offsetY, textW, textH};
                            
                            // Store layout if element has onClick handler
                            if (node->getProps().has("onClick")) {
                                elementLayouts_[node->getId()] = textLayout;
                            }
                            
                            drawText(offsetX, offsetY, text, textColor, fontSize);
                        }
                    } else if (child) {
                        renderVNode(child, offsetX, offsetY);
                    }
                }
                break;
            }
            
            // Get layout from props or use defaults
            // For View, use full width/height if not specified
            Rect layout = getRectFromProps(node->getProps(), {offsetX, offsetY, width_ - offsetX, height_ - offsetY});
            if (layout.width <= 0) layout.width = width_ - offsetX;
            if (layout.height <= 0) layout.height = height_ - offsetY;
            
            // Store layout for hit testing (if element has onClick or is interactive)
            if (node->getProps().has("onClick") || tag == "Button" || tag == "View") {
                elementLayouts_[node->getId()] = layout;
            }
            
            // Get background color (default to transparent for View)
            uint32_t bgColor = 0x00000000; // Transparent by default
            if (node->getProps().has("backgroundColor")) {
                bgColor = getColorFromProps(node->getProps(), "backgroundColor", 0x00000000);
            }
            uint32_t borderColor = getColorFromProps(node->getProps(), "borderColor", 0x000000FF);
            
            // Check for gradient
            if (node->getProps().has("gradient")) {
                try {
                    auto gradient = node->getProps().get<std::vector<GradientStop>>("gradient");
                    std::string directionStr = "horizontal";
                    if (node->getProps().has("gradientDirection")) {
                        directionStr = node->getProps().get<std::string>("gradientDirection");
                    }
                    GradientDirection dir = GradientDirection::Horizontal;
                    if (directionStr == "vertical") dir = GradientDirection::Vertical;
                    else if (directionStr == "radial") dir = GradientDirection::Radial;
                    
                    fillRectGradient(layout, gradient, dir);
                } catch (...) {
                    if (bgColor != 0x00000000) {
                        fillRect(layout, bgColor);
                    }
                }
            } else {
                if (bgColor != 0x00000000) {
                    fillRect(layout, bgColor);
                }
            }
            
            // Draw border if specified
            if (node->getProps().has("borderWidth")) {
                try {
                    int borderWidth = node->getProps().get<int>("borderWidth");
                    if (borderWidth > 0) {
                        for (int i = 0; i < borderWidth; i++) {
                            drawRect(layout.x + i, layout.y + i, 
                                    layout.width - 2*i, layout.height - 2*i, borderColor);
                        }
                    }
                } catch (...) {}
            }
            
            // Render children
            int childOffsetY = offsetY;
            for (const auto& child : node->getChildren()) {
                if (child) {
                    if (child->getType() == VNodeType::Text) {
                        // Text node directly - render it
                        std::string text = child->getText();
                        if (!text.empty()) {
                            uint32_t textColor = getColorFromProps(node->getProps(), "color", 0x000000FF);
                            int fontSize = 16;
                            if (node->getProps().has("fontSize")) {
                                try {
                                    fontSize = node->getProps().get<int>("fontSize");
                                } catch (...) {}
                            }
                            drawText(layout.x + 10, childOffsetY + 20, text, textColor, fontSize);
                            // Estimate text height for next element
                            int textW, textH;
                            getTextSize(text, fontSize, textW, textH);
                            childOffsetY += textH + 5;
                        }
                    } else {
                        renderVNode(child, layout.x + 10, childOffsetY);
                        childOffsetY += 50; // Simple spacing
                    }
                }
            }
            break;
        }
        
        case VNodeType::Text: {
            std::string text = node->getText();
            if (!text.empty()) {
                uint32_t textColor = getColorFromProps(node->getProps(), "color", 0x000000FF);
                int fontSize = 16;
                if (node->getProps().has("fontSize")) {
                    try {
                        fontSize = node->getProps().get<int>("fontSize");
                    } catch (...) {}
                }
                drawText(offsetX, offsetY, text, textColor, fontSize);
            }
            break;
        }
        
        case VNodeType::Component:
        case VNodeType::Fragment: {
            // Render children
            for (const auto& child : node->getChildren()) {
                if (child) {
                    renderVNode(child, offsetX, offsetY);
                }
            }
            break;
        }
    }
}

void SDL2Renderer::clearLayoutCache() {
    elementLayouts_.clear();
}

void SDL2Renderer::render(VNode::Ptr root) {
    if (!root) return;
    clearLayoutCache(); // Clear previous layouts
    renderVNode(root, 0, 0);
}

VNode::Ptr SDL2Renderer::findElementAtRecursive(int x, int y, VNode::Ptr node) {
    if (!node) return nullptr;
    
    // Check if this element contains the point
    auto it = elementLayouts_.find(node->getId());
    if (it != elementLayouts_.end()) {
        const Rect& rect = it->second;
        
        if (x >= rect.x && x < rect.x + rect.width &&
            y >= rect.y && y < rect.y + rect.height) {
            
            // Check children first (they're on top) - but only if they have layouts
            VNode::Ptr bestChild = nullptr;
            for (const auto& child : node->getChildren()) {
                if (child) {
                    auto found = findElementAtRecursive(x, y, child);
                    if (found) {
                        // Prefer interactive elements (Button or with onClick)
                        if (found->getType() == VNodeType::Element) {
                            const std::string& tag = found->getTag();
                            if (tag == "Button" || found->getProps().has("onClick")) {
                                return found;
                            }
                        }
                        // Keep track of any found child
                        if (!bestChild) bestChild = found;
                    }
                }
            }
            
            // If we found a child, return it
            if (bestChild) return bestChild;
            
            // If no child contains the point, return this element
            // Only return if it has an onClick handler or is a Button
            if (node->getType() == VNodeType::Element) {
                const std::string& tag = node->getTag();
                if (tag == "Button" || node->getProps().has("onClick")) {
                    return node;
                }
            }
        }
    }
    
    return nullptr;
}

VNode::Ptr SDL2Renderer::findElementAt(int x, int y, VNode::Ptr root) {
    if (!root) return nullptr;
    return findElementAtRecursive(x, y, root);
}

bool SDL2Renderer::handleClick(int x, int y, VNode::Ptr root) {
    VNode::Ptr clicked = findElementAt(x, y, root);
    if (!clicked) {
        return false;
    }
    
    // Check if element has onClick handler
    if (clicked->getType() == VNodeType::Element) {
        const Props& props = clicked->getProps();
        if (props.has("onClick")) {
            try {
                // Try to get as std::function<void()>
                auto typeIndex = props.getType("onClick");
                if (typeIndex && *typeIndex == std::type_index(typeid(std::function<void()>))) {
                    auto onClick = props.get<std::function<void()>>("onClick");
                    if (onClick) {
                        onClick();
                        return true;
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error calling onClick handler: " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "Unknown error calling onClick handler" << std::endl;
            }
        }
    }
    
    return false;
}

bool SDL2Renderer::handleTouch(int x, int y, VNode::Ptr root) {
    // Touch events are handled the same as clicks
    return handleClick(x, y, root);
}

void SDL2Renderer::processEvents(VNode::Ptr root) {
    SDL_Event event;
    while (pollEvent(event)) {
        if (event.type == SDL_QUIT) {
            // Quit event should be handled by the application
            continue;
        } else if (event.type == SDL_FINGERDOWN) {
            // Handle touch events first
            // Convert touch coordinates to screen coordinates
            int x = static_cast<int>(event.tfinger.x * width_);
            int y = static_cast<int>(event.tfinger.y * height_);
            handleTouch(x, y, root);
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Only handle mouse events if they're NOT from touch emulation
            // SDL_TOUCH_MOUSEID indicates the mouse event was generated from touch
            if (event.button.button == SDL_BUTTON_LEFT && event.button.which != SDL_TOUCH_MOUSEID) {
                handleClick(event.button.x, event.button.y, root);
            }
            // If event.button.which == SDL_TOUCH_MOUSEID, ignore it
            // because we already handled it via SDL_FINGERDOWN above
        }
    }
}

bool SDL2Renderer::pollEvent(SDL_Event& event) {
    return SDL_PollEvent(&event) != 0;
}

} // namespace renderer
} // namespace reactpp

