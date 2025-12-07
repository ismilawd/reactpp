#pragma once

#include <cstdint>

namespace reactpp {
namespace renderer {

struct Point {
    int x, y;
};

struct Rect {
    int x, y, width, height;
};

struct GradientStop {
    float position;  // 0.0 to 1.0
    uint32_t color;
};

enum class GradientDirection {
    Horizontal,
    Vertical,
    Radial
};

} // namespace renderer
} // namespace reactpp

