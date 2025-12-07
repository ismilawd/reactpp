#include "reactpp/renderer/FramebufferRenderer.hpp"
#include "reactpp/core/Props.hpp"
#include "reactpp/elements/Elements.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace reactpp {
namespace renderer {

// Simple 8x8 bitmap font for digits
static const uint8_t bitmap_digits[10][8] = {
    {0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x63, 0x3E}, // 0
    {0x18, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E}, // 1
    {0x3E, 0x63, 0x60, 0x3C, 0x06, 0x03, 0x63, 0x7F}, // 2
    {0x3E, 0x63, 0x60, 0x3C, 0x60, 0x63, 0x63, 0x3E}, // 3
    {0x30, 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78}, // 4
    {0x7F, 0x03, 0x03, 0x3F, 0x60, 0x60, 0x63, 0x3E}, // 5
    {0x3C, 0x06, 0x03, 0x3F, 0x63, 0x63, 0x63, 0x3E}, // 6
    {0x7F, 0x63, 0x60, 0x30, 0x18, 0x0C, 0x0C, 0x0C}, // 7
    {0x3E, 0x63, 0x63, 0x3E, 0x63, 0x63, 0x63, 0x3E}, // 8
    {0x3E, 0x63, 0x63, 0x63, 0x7E, 0x60, 0x30, 0x1E}, // 9
};

static uint8_t getBitmapChar(char c, int row) {
    if (row < 0 || row >= 8) return 0;
    
    // Digits 0-9
    if (c >= '0' && c <= '9') {
        int index = c - '0'; // 0-9 maps to our font array indices 0-9
        if (index >= 0 && index < 10 && row < 8) {
            return bitmap_digits[index][row];
        }
        return 0;
    }
    
    // Letters - simple pattern for now
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        // Basic letter pattern
        if (row == 0 || row == 7) return 0x7E;
        if (row == 1 || row == 6) return 0x42;
        return 0x7E;
    }
    
    // Space
    if (c == ' ') return 0x00;
    
    // Colon
    if (c == ':') return (row == 2 || row == 5) ? 0x18 : 0x00;
    
    // Period
    if (c == '.') return (row == 7) ? 0x18 : 0x00;
    
    // Default: return space pattern
    return 0x00;
}

FramebufferRenderer::FramebufferRenderer(const std::string& fbDevice)
    : fbDevice_(fbDevice), fbFd_(-1), framebuffer_(nullptr),
      width_(0), height_(0), bytesPerPixel_(0), lineLength_(0), initialized_(false) {
}

FramebufferRenderer::~FramebufferRenderer() {
    cleanup();
}

bool FramebufferRenderer::initializeFramebuffer() {
    fbFd_ = open(fbDevice_.c_str(), O_RDWR);
    if (fbFd_ < 0) {
        std::cerr << "Cannot open framebuffer device: " << fbDevice_ << std::endl;
        return false;
    }
    
    // Get fixed screen information
    if (ioctl(fbFd_, FBIOGET_FSCREENINFO, &finfo_) == -1) {
        std::cerr << "Error reading fixed information" << std::endl;
        close(fbFd_);
        fbFd_ = -1;
        return false;
    }
    
    // Get variable screen information
    if (ioctl(fbFd_, FBIOGET_VSCREENINFO, &vinfo_) == -1) {
        std::cerr << "Error reading variable information" << std::endl;
        close(fbFd_);
        fbFd_ = -1;
        return false;
    }
    
    // Calculate bytes per pixel
    bytesPerPixel_ = vinfo_.bits_per_pixel / 8;
    if (bytesPerPixel_ == 0) bytesPerPixel_ = 1;
    
    width_ = vinfo_.xres;
    height_ = vinfo_.yres;
    lineLength_ = finfo_.line_length;
    
    // Map framebuffer to memory
    long int screensize = finfo_.smem_len;
    framebuffer_ = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbFd_, 0);
    
    if (framebuffer_ == MAP_FAILED) {
        std::cerr << "Failed to mmap framebuffer" << std::endl;
        close(fbFd_);
        fbFd_ = -1;
        return false;
    }
    
    initialized_ = true;
    std::cout << "Framebuffer initialized: " << width_ << "x" << height_ 
              << " (" << bytesPerPixel_ << " bytes/pixel)" << std::endl;
    
    return true;
}

void FramebufferRenderer::cleanup() {
    if (framebuffer_ && framebuffer_ != MAP_FAILED) {
        long int screensize = finfo_.smem_len;
        munmap(framebuffer_, screensize);
        framebuffer_ = nullptr;
    }
    
    if (fbFd_ >= 0) {
        close(fbFd_);
        fbFd_ = -1;
    }
    
    initialized_ = false;
}

bool FramebufferRenderer::createWindow(const std::string& title, int width, int height) {
    (void)title; // Unused in framebuffer mode
    (void)width; // Use actual framebuffer dimensions
    (void)height;
    
    return initializeFramebuffer();
}

void FramebufferRenderer::destroyWindow() {
    cleanup();
}

uint32_t FramebufferRenderer::convertColorToFramebufferFormat(uint32_t color) {
    uint8_t r, g, b, a;
    unpackColor(color, r, g, b, a);
    
    // Convert based on framebuffer format
    if (vinfo_.bits_per_pixel == 32) {
        // RGBA or BGRA depending on framebuffer
        if (vinfo_.red.offset == 0) {
            // RGBA
            return (r << vinfo_.red.offset) | (g << vinfo_.green.offset) | 
                   (b << vinfo_.blue.offset) | (a << vinfo_.transp.offset);
        } else {
            // BGRA (common)
            return (b << vinfo_.blue.offset) | (g << vinfo_.green.offset) | 
                   (r << vinfo_.red.offset) | (a << vinfo_.transp.offset);
        }
    } else if (vinfo_.bits_per_pixel == 16) {
        // RGB565
        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
    } else if (vinfo_.bits_per_pixel == 24) {
        // RGB
        return (r << 16) | (g << 8) | b;
    }
    
    return color; // Fallback
}

void FramebufferRenderer::setPixel(int x, int y, uint32_t color) {
    if (!framebuffer_ || x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;
    }
    
    uint32_t fbColor = convertColorToFramebufferFormat(color);
    long int location = (x + vinfo_.xoffset) * bytesPerPixel_ + 
                        (y + vinfo_.yoffset) * lineLength_;
    
    if (bytesPerPixel_ == 4) {
        *((uint32_t*)((uint8_t*)framebuffer_ + location)) = fbColor;
    } else if (bytesPerPixel_ == 2) {
        *((uint16_t*)((uint8_t*)framebuffer_ + location)) = (uint16_t)fbColor;
    } else if (bytesPerPixel_ == 3) {
        uint8_t* pixel = (uint8_t*)framebuffer_ + location;
        pixel[0] = (fbColor >> 16) & 0xFF;
        pixel[1] = (fbColor >> 8) & 0xFF;
        pixel[2] = fbColor & 0xFF;
    } else if (bytesPerPixel_ == 1) {
        *((uint8_t*)framebuffer_ + location) = (uint8_t)fbColor;
    }
}

void FramebufferRenderer::clear() {
    clear(0x000000FF); // Black
}

void FramebufferRenderer::clear(uint32_t color) {
    if (!framebuffer_) return;
    
    uint32_t fbColor = convertColorToFramebufferFormat(color);
    
    if (bytesPerPixel_ == 4) {
        uint32_t* fb = (uint32_t*)framebuffer_;
        for (int i = 0; i < width_ * height_; i++) {
            fb[i] = fbColor;
        }
    } else {
        for (int y = 0; y < height_; y++) {
            for (int x = 0; x < width_; x++) {
                setPixel(x, y, color);
            }
        }
    }
}

void FramebufferRenderer::present() {
    // No-op for framebuffer (direct writes)
    // Could sync here if needed
    msync(framebuffer_, finfo_.smem_len, MS_SYNC);
}

void FramebufferRenderer::drawRect(int x, int y, int width, int height, uint32_t color) {
    // Top and bottom lines
    drawLine(x, y, x + width - 1, y, color);
    drawLine(x, y + height - 1, x + width - 1, y + height - 1, color);
    // Left and right lines
    drawLine(x, y, x, y + height - 1, color);
    drawLine(x + width - 1, y, x + width - 1, y + height - 1, color);
}

void FramebufferRenderer::fillRect(int x, int y, int width, int height, uint32_t color) {
    for (int py = y; py < y + height; py++) {
        for (int px = x; px < x + width; px++) {
            setPixel(px, py, color);
        }
    }
}

void FramebufferRenderer::drawRect(const Rect& rect, uint32_t color) {
    drawRect(rect.x, rect.y, rect.width, rect.height, color);
}

void FramebufferRenderer::fillRect(const Rect& rect, uint32_t color) {
    fillRect(rect.x, rect.y, rect.width, rect.height, color);
}

void FramebufferRenderer::drawCircle(int centerX, int centerY, int radius, uint32_t color) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    auto drawCirclePoints = [this, centerX, centerY, color](int x, int y) {
        setPixel(centerX + x, centerY + y, color);
        setPixel(centerX - x, centerY + y, color);
        setPixel(centerX + x, centerY - y, color);
        setPixel(centerX - x, centerY - y, color);
        setPixel(centerX + y, centerY + x, color);
        setPixel(centerX - y, centerY + x, color);
        setPixel(centerX + y, centerY - x, color);
        setPixel(centerX - y, centerY - x, color);
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

void FramebufferRenderer::fillCircle(int centerX, int centerY, int radius, uint32_t color) {
    for (int y = -radius; y <= radius; y++) {
        int x = static_cast<int>(std::sqrt(radius * radius - y * y));
        drawLine(centerX - x, centerY + y, centerX + x, centerY + y, color);
    }
}

void FramebufferRenderer::drawEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color) {
    const int numPoints = 64;
    for (int i = 0; i < numPoints; i++) {
        double angle = 2.0 * M_PI * i / numPoints;
        int x = centerX + static_cast<int>(radiusX * std::cos(angle));
        int y = centerY + static_cast<int>(radiusY * std::sin(angle));
        
        if (i == 0) {
            setPixel(x, y, color);
        } else {
            double prevAngle = 2.0 * M_PI * (i - 1) / numPoints;
            int prevX = centerX + static_cast<int>(radiusX * std::cos(prevAngle));
            int prevY = centerY + static_cast<int>(radiusY * std::sin(prevAngle));
            drawLine(prevX, prevY, x, y, color);
        }
    }
}

void FramebufferRenderer::fillEllipse(int centerX, int centerY, int radiusX, int radiusY, uint32_t color) {
    for (int y = -radiusY; y <= radiusY; y++) {
        int x = static_cast<int>(radiusX * std::sqrt(1.0 - (double)(y * y) / (radiusY * radiusY)));
        drawLine(centerX - x, centerY + y, centerX + x, centerY + y, color);
    }
}

void FramebufferRenderer::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1;
    int y = y1;
    
    while (true) {
        setPixel(x, y, color);
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void FramebufferRenderer::drawLine(const Point& p1, const Point& p2, uint32_t color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void FramebufferRenderer::drawLines(const std::vector<Point>& points, uint32_t color) {
    if (points.size() < 2) return;
    
    for (size_t i = 0; i < points.size() - 1; i++) {
        drawLine(points[i], points[i+1], color);
    }
}

void FramebufferRenderer::drawPolygon(const std::vector<Point>& points, uint32_t color) {
    if (points.size() < 3) return;
    
    drawLines(points, color);
    if (points.size() > 2) {
        drawLine(points.back(), points.front(), color);
    }
}

void FramebufferRenderer::fillPolygon(const std::vector<Point>& points, uint32_t color) {
    if (points.size() < 3) return;
    
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
                drawLine(intersections[i], y, intersections[i+1], y, color);
            }
        }
    }
}

void FramebufferRenderer::renderBitmapChar(int x, int y, char c, uint32_t color, int scale) {
    for (int row = 0; row < 8; row++) {
        uint8_t bits = getBitmapChar(c, row);
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col))) {
                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        setPixel(x + col * scale + sx, y + row * scale + sy, color);
                    }
                }
            }
        }
    }
}

void FramebufferRenderer::drawText(int x, int y, const std::string& text, uint32_t color) {
    drawText(x, y, text, color, 16);
}

void FramebufferRenderer::drawText(int x, int y, const std::string& text, uint32_t color, int fontSize) {
    drawText(x, y, text, color, "", fontSize);
}

void FramebufferRenderer::drawText(int x, int y, const std::string& text, uint32_t color, const std::string& fontPath, int fontSize) {
    (void)fontPath; // Bitmap font doesn't use file path
    
    int scale = fontSize / 8;
    if (scale < 1) scale = 1;
    
    int charX = x;
    for (char c : text) {
        renderBitmapChar(charX, y, c, color, scale);
        charX += 8 * scale + 2; // Character width + spacing
    }
}

void FramebufferRenderer::getTextSize(const std::string& text, int fontSize, int& width, int& height) {
    int scale = fontSize / 8;
    if (scale < 1) scale = 1;
    
    width = text.length() * (8 * scale + 2);
    height = 8 * scale;
}

void FramebufferRenderer::fillRectGradient(int x, int y, int width, int height, 
                                           const std::vector<GradientStop>& stops, 
                                           GradientDirection direction) {
    if (stops.size() < 2) return;
    
    std::vector<GradientStop> sortedStops = stops;
    std::sort(sortedStops.begin(), sortedStops.end(), 
              [](const GradientStop& a, const GradientStop& b) {
                  return a.position < b.position;
              });
    
    if (direction == GradientDirection::Horizontal) {
        for (int px = 0; px < width; px++) {
            float t = static_cast<float>(px) / width;
            
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
            
            uint32_t color = rgba(r, g, b, a);
            drawLine(x + px, y, x + px, y + height, color);
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
            
            uint32_t color = rgba(r, g, b, a);
            drawLine(x, y + py, x + width, y + py, color);
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
                
                uint32_t color = rgba(r, g, b, a);
                setPixel(x + px, y + py, color);
            }
        }
    }
}

void FramebufferRenderer::fillRectGradient(const Rect& rect, 
                                           const std::vector<GradientStop>& stops, 
                                           GradientDirection direction) {
    fillRectGradient(rect.x, rect.y, rect.width, rect.height, stops, direction);
}

uint32_t FramebufferRenderer::rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (static_cast<uint32_t>(r) << 24) |
           (static_cast<uint32_t>(g) << 16) |
           (static_cast<uint32_t>(b) << 8) |
           static_cast<uint32_t>(a);
}

uint32_t FramebufferRenderer::rgb(uint8_t r, uint8_t g, uint8_t b) {
    return rgba(r, g, b, 255);
}

void FramebufferRenderer::unpackColor(uint32_t color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
    r = (color >> 24) & 0xFF;
    g = (color >> 16) & 0xFF;
    b = (color >> 8) & 0xFF;
    a = color & 0xFF;
}

uint32_t FramebufferRenderer::getColorFromProps(const Props& props, const std::string& key, uint32_t defaultColor) {
    if (props.has(key)) {
        try {
            if (props.getType(key) == std::type_index(typeid(uint32_t))) {
                return props.get<uint32_t>(key);
            } else if (props.getType(key) == std::type_index(typeid(std::string))) {
                std::string colorStr = props.get<std::string>(key);
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

Rect FramebufferRenderer::getRectFromProps(const Props& props, const Rect& defaultRect) {
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

void FramebufferRenderer::renderVNode(VNode::Ptr node, int offsetX, int offsetY) {
    if (!node) return;
    
    switch (node->getType()) {
        case VNodeType::Element: {
            std::string tag = node->getTag();
            
            if (tag == "Button") {
                Rect layout = getRectFromProps(node->getProps(), {offsetX, offsetY, 150, 40});
                elementLayouts_[node->getId()] = layout;
                
                uint32_t bgColor = getColorFromProps(node->getProps(), "backgroundColor", 0x4A90E2FF);
                uint32_t borderColor = getColorFromProps(node->getProps(), "borderColor", 0x357ABDFF);
                uint32_t textColor = getColorFromProps(node->getProps(), "color", 0xFFFFFFFF);
                
                fillRect(layout, bgColor);
                drawRect(layout, borderColor);
                
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
            
            if (tag == "Text") {
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
                            
                            int textW, textH;
                            getTextSize(text, fontSize, textW, textH);
                            Rect textLayout = {offsetX, offsetY, textW, textH};
                            
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
            
            Rect layout = getRectFromProps(node->getProps(), {offsetX, offsetY, width_ - offsetX, height_ - offsetY});
            if (layout.width <= 0) layout.width = width_ - offsetX;
            if (layout.height <= 0) layout.height = height_ - offsetY;
            
            if (node->getProps().has("onClick") || tag == "Button" || tag == "View") {
                elementLayouts_[node->getId()] = layout;
            }
            
            uint32_t bgColor = 0x00000000;
            if (node->getProps().has("backgroundColor")) {
                bgColor = getColorFromProps(node->getProps(), "backgroundColor", 0x00000000);
            }
            uint32_t borderColor = getColorFromProps(node->getProps(), "borderColor", 0x000000FF);
            
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
            
            int childOffsetY = offsetY;
            for (const auto& child : node->getChildren()) {
                if (child) {
                    if (child->getType() == VNodeType::Text) {
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
                            int textW, textH;
                            getTextSize(text, fontSize, textW, textH);
                            childOffsetY += textH + 5;
                        }
                    } else {
                        renderVNode(child, layout.x + 10, childOffsetY);
                        childOffsetY += 50;
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
            for (const auto& child : node->getChildren()) {
                if (child) {
                    renderVNode(child, offsetX, offsetY);
                }
            }
            break;
        }
    }
}

void FramebufferRenderer::clearLayoutCache() {
    elementLayouts_.clear();
}

void FramebufferRenderer::render(VNode::Ptr root) {
    if (!root) return;
    clearLayoutCache();
    renderVNode(root, 0, 0);
}

VNode::Ptr FramebufferRenderer::findElementAtRecursive(int x, int y, VNode::Ptr node) {
    if (!node) return nullptr;
    
    auto it = elementLayouts_.find(node->getId());
    if (it != elementLayouts_.end()) {
        const Rect& rect = it->second;
        
        if (x >= rect.x && x < rect.x + rect.width &&
            y >= rect.y && y < rect.y + rect.height) {
            
            VNode::Ptr bestChild = nullptr;
            for (const auto& child : node->getChildren()) {
                if (child) {
                    auto found = findElementAtRecursive(x, y, child);
                    if (found) {
                        if (found->getType() == VNodeType::Element) {
                            const std::string& tag = found->getTag();
                            if (tag == "Button" || found->getProps().has("onClick")) {
                                return found;
                            }
                        }
                        if (!bestChild) bestChild = found;
                    }
                }
            }
            
            if (bestChild) return bestChild;
            
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

VNode::Ptr FramebufferRenderer::findElementAt(int x, int y, VNode::Ptr root) {
    if (!root) return nullptr;
    return findElementAtRecursive(x, y, root);
}

bool FramebufferRenderer::handleClick(int x, int y, VNode::Ptr root) {
    VNode::Ptr clicked = findElementAt(x, y, root);
    if (!clicked) {
        return false;
    }
    
    if (clicked->getType() == VNodeType::Element) {
        const Props& props = clicked->getProps();
        if (props.has("onClick")) {
            try {
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

bool FramebufferRenderer::handleTouch(int x, int y, VNode::Ptr root) {
    return handleClick(x, y, root);
}

} // namespace renderer
} // namespace reactpp

