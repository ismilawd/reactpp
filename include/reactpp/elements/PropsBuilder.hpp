#pragma once

#include "reactpp/core/Props.hpp"
#include <functional>
#include <string>

namespace reactpp {
namespace elements {

// React-like props builder for inline prop setting
class PropsBuilder {
public:
    PropsBuilder() {}
    
    // Build the props object
    Props build() const { return props_; }
    
    // Chainable setters for common props
    PropsBuilder& onClick(std::function<void()> handler) {
        props_.set("onClick", handler);
        return *this;
    }
    
    PropsBuilder& x(int value) {
        props_.set("x", value);
        return *this;
    }
    
    PropsBuilder& y(int value) {
        props_.set("y", value);
        return *this;
    }
    
    PropsBuilder& width(int value) {
        props_.set("width", value);
        return *this;
    }
    
    PropsBuilder& height(int value) {
        props_.set("height", value);
        return *this;
    }
    
    PropsBuilder& backgroundColor(uint32_t color) {
        props_.set("backgroundColor", color);
        return *this;
    }
    
    PropsBuilder& borderColor(uint32_t color) {
        props_.set("borderColor", color);
        return *this;
    }
    
    PropsBuilder& color(uint32_t color) {
        props_.set("color", color);
        return *this;
    }
    
    PropsBuilder& fontSize(int size) {
        props_.set("fontSize", size);
        return *this;
    }
    
    PropsBuilder& borderWidth(int width) {
        props_.set("borderWidth", width);
        return *this;
    }
    
    // Generic setter for any prop
    template<typename T>
    PropsBuilder& set(const std::string& key, const T& value) {
        props_.set(key, value);
        return *this;
    }
    
    // Implicit conversion to Props
    operator Props() const { return props_; }

private:
    Props props_;
};

// Helper function to create props builder
inline PropsBuilder props() {
    return PropsBuilder();
}

} // namespace elements
} // namespace reactpp

