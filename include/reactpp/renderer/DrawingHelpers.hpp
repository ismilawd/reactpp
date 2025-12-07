#pragma once

#include "reactpp/renderer/RendererTypes.hpp"
#include "SDL2Renderer.hpp"
#include <vector>
#include <string>

namespace reactpp {
namespace renderer {

// Helper functions for creating gradients
inline std::vector<GradientStop> createGradient(uint32_t startColor, uint32_t endColor) {
    return {
        {0.0f, startColor},
        {1.0f, endColor}
    };
}

inline std::vector<GradientStop> createGradient(uint32_t color1, uint32_t color2, uint32_t color3) {
    return {
        {0.0f, color1},
        {0.5f, color2},
        {1.0f, color3}
    };
}

inline std::vector<GradientStop> createGradient(const std::vector<std::pair<float, uint32_t>>& stops) {
    std::vector<GradientStop> gradientStops;
    for (const auto& stop : stops) {
        gradientStops.push_back({stop.first, stop.second});
    }
    return gradientStops;
}

// Color constants
namespace Colors {
    constexpr uint32_t Black = 0x000000FF;
    constexpr uint32_t White = 0xFFFFFFFF;
    constexpr uint32_t Red = 0xFF0000FF;
    constexpr uint32_t Green = 0x00FF00FF;
    constexpr uint32_t Blue = 0x0000FFFF;
    constexpr uint32_t Yellow = 0xFFFF00FF;
    constexpr uint32_t Cyan = 0x00FFFFFF;
    constexpr uint32_t Magenta = 0xFF00FFFF;
    constexpr uint32_t Gray = 0x808080FF;
    constexpr uint32_t LightGray = 0xC0C0C0FF;
    constexpr uint32_t DarkGray = 0x404040FF;
    constexpr uint32_t Transparent = 0x00000000;
}

// Helper to create color from RGB
inline uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return SDL2Renderer::rgba(r, g, b, a);
}

// Helper to create color from hex string
inline uint32_t colorHex(const std::string& hex) {
    if (hex.size() < 7 || hex[0] != '#') {
        return Colors::Black;
    }
    
    uint32_t color = 0;
    for (size_t i = 1; i < hex.size() && i < 9; i++) {
        char c = hex[i];
        uint32_t val = 0;
        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
        else continue;
        color = (color << 4) | val;
    }
    
    if (hex.size() == 7) {
        color = (color << 8) | 0xFF;
    }
    
    return color;
}

} // namespace renderer
} // namespace reactpp

