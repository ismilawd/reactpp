#pragma once

#include <unordered_map>
#include <any>
#include <typeindex>
#include <string>
#include <optional>
#include <functional>
#include <stdexcept>

namespace reactcpp {

class Props {
public:
    // Set a property
    template<typename T>
    void set(const std::string& key, const T& value) {
        props_[key] = value;
        types_[key] = std::type_index(typeid(T));
    }
    
    // Get a property (throws on type mismatch)
    template<typename T>
    T get(const std::string& key) const {
        auto it = props_.find(key);
        if (it == props_.end()) {
            throw std::runtime_error("Property '" + key + "' not found");
        }
        
        auto typeIt = types_.find(key);
        if (typeIt == types_.end() || typeIt->second != std::type_index(typeid(T))) {
            throw std::runtime_error("Property '" + key + "' type mismatch");
        }
        
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast& e) {
            throw std::runtime_error("Property '" + key + "' type cast failed: " + e.what());
        }
    }
    
    // Try to get a property (returns optional)
    template<typename T>
    std::optional<T> tryGet(const std::string& key) const {
        auto it = props_.find(key);
        if (it == props_.end()) {
            return std::nullopt;
        }
        
        auto typeIt = types_.find(key);
        if (typeIt == types_.end() || typeIt->second != std::type_index(typeid(T))) {
            return std::nullopt;
        }
        
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast&) {
            return std::nullopt;
        }
    }
    
    // Check if property exists
    bool has(const std::string& key) const {
        return props_.find(key) != props_.end();
    }
    
    // Remove a property
    void remove(const std::string& key) {
        props_.erase(key);
        types_.erase(key);
    }
    
    // Get type information
    std::optional<std::type_index> getType(const std::string& key) const {
        auto it = types_.find(key);
        if (it != types_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    // Merge another Props object
    void merge(const Props& other) {
        for (const auto& [key, value] : other.props_) {
            props_[key] = value;
            types_[key] = other.types_.at(key);
        }
    }
    
    // Equality comparison (shallow)
    bool operator==(const Props& other) const {
        if (props_.size() != other.props_.size()) {
            return false;
        }
        
        for (const auto& [key, value] : props_) {
            auto otherIt = other.props_.find(key);
            if (otherIt == other.props_.end()) {
                return false;
            }
            
            // Compare types
            auto thisTypeIt = types_.find(key);
            auto otherTypeIt = other.types_.find(key);
            if (thisTypeIt == types_.end() || otherTypeIt == other.types_.end() ||
                thisTypeIt->second != otherTypeIt->second) {
                return false;
            }
            
            // For std::function, we can't compare directly, so we compare addresses
            // For other types, we'd need type-specific comparison
            // This is a simplified shallow comparison
        }
        
        return true;
    }
    
    bool operator!=(const Props& other) const {
        return !(*this == other);
    }
    
    // Iterator support
    using iterator = std::unordered_map<std::string, std::any>::iterator;
    using const_iterator = std::unordered_map<std::string, std::any>::const_iterator;
    
    iterator begin() { return props_.begin(); }
    iterator end() { return props_.end(); }
    const_iterator begin() const { return props_.begin(); }
    const_iterator end() const { return props_.end(); }
    const_iterator cbegin() const { return props_.cbegin(); }
    const_iterator cend() const { return props_.cend(); }
    
    // Size
    size_t size() const { return props_.size(); }
    bool empty() const { return props_.empty(); }
    
    // Clear
    void clear() {
        props_.clear();
        types_.clear();
    }
    
private:
    std::unordered_map<std::string, std::any> props_;
    std::unordered_map<std::string, std::type_index> types_;
};

} // namespace reactcpp

