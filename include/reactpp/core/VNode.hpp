#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <atomic>
#include <functional>
#include "Props.hpp"

namespace reactpp {

enum class VNodeType {
    Element,
    Text,
    Component,
    Fragment
};

class Component;

class VNode : public std::enable_shared_from_this<VNode> {
public:
    using Ptr = std::shared_ptr<VNode>;
    using WeakPtr = std::weak_ptr<VNode>;
    
    // Factory methods
    static Ptr createElement(
        const std::string& tag,
        const Props& props = Props(),
        const std::vector<Ptr>& children = {}
    );
    
    static Ptr createText(const std::string& text);
    
    static Ptr createComponent(
        std::shared_ptr<Component> component,
        const Props& props = Props(),
        const std::vector<Ptr>& children = {}
    );
    
    static Ptr createFragment(const std::vector<Ptr>& children = {});
    
    // Getters
    VNodeType getType() const { return type_; }
    const std::string& getTag() const { return tag_; }
    const std::string& getText() const { return text_; }
    const Props& getProps() const { return props_; }
    Props& getProps() { return props_; }
    const std::vector<Ptr>& getChildren() const { return children_; }
    std::vector<Ptr>& getChildren() { return children_; }
    const std::optional<std::string>& getKey() const { return key_; }
    uint64_t getId() const { return id_; }
    WeakPtr getParent() const { return parent_; }
    std::shared_ptr<Component> getComponent() const { return component_; }
    
    // Setters
    void setKey(const std::optional<std::string>& key) { key_ = key; }
    void setParent(WeakPtr parent) { parent_ = parent; }
    
    // Tree manipulation
    void appendChild(Ptr child);
    void removeChild(Ptr child);
    void replaceChild(Ptr oldChild, Ptr newChild);
    void insertBefore(Ptr newChild, Ptr sibling);
    
    // Tree traversal
    void traversePreOrder(std::function<void(Ptr)> visitor);
    void traversePostOrder(std::function<void(Ptr)> visitor);
    void traverseLevelOrder(std::function<void(Ptr)> visitor);
    
    // Finding
    Ptr findChildByKey(const std::string& key) const;
    Ptr findById(uint64_t id) const;
    
    // Cloning
    Ptr cloneShallow() const;
    Ptr cloneDeep() const;
    
    // Serialization
    std::string serialize() const;
    
    // Equality
    bool operator==(const VNode& other) const;
    bool operator!=(const VNode& other) const;
    
    // Constructor (public to allow std::make_shared, but factory methods are preferred)
    VNode(VNodeType type);
    
private:
    
    static std::atomic<uint64_t> next_id_;
    
    VNodeType type_;
    uint64_t id_;
    std::string tag_;
    std::string text_;
    Props props_;
    std::vector<Ptr> children_;
    std::optional<std::string> key_;
    WeakPtr parent_;
    std::shared_ptr<Component> component_;
};

} // namespace reactpp

