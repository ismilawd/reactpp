#include "reactcpp/core/VNode.hpp"
#include "reactcpp/core/Component.hpp"
#include <sstream>
#include <queue>
#include <algorithm>

namespace reactcpp {

std::atomic<uint64_t> VNode::next_id_{1};

VNode::VNode(VNodeType type) 
    : type_(type), id_(next_id_.fetch_add(1, std::memory_order_relaxed)) {
}

VNode::Ptr VNode::createElement(
    const std::string& tag,
    const Props& props,
    const std::vector<Ptr>& children) {
    
    auto node = std::make_shared<VNode>(VNodeType::Element);
    node->tag_ = tag;
    node->props_ = props;
    node->children_ = children;
    
    // Set parent for children
    for (auto& child : node->children_) {
        if (child) {
            child->parent_ = node;
        }
    }
    
    return node;
}

VNode::Ptr VNode::createText(const std::string& text) {
    auto node = std::make_shared<VNode>(VNodeType::Text);
    node->text_ = text;
    return node;
}

VNode::Ptr VNode::createComponent(
    std::shared_ptr<Component> component,
    const Props& props,
    const std::vector<Ptr>& children) {
    
    auto node = std::make_shared<VNode>(VNodeType::Component);
    node->component_ = component;
    node->props_ = props;
    node->children_ = children;
    
    for (auto& child : node->children_) {
        if (child) {
            child->parent_ = node;
        }
    }
    
    return node;
}

VNode::Ptr VNode::createFragment(const std::vector<Ptr>& children) {
    auto node = std::make_shared<VNode>(VNodeType::Fragment);
    node->children_ = children;
    
    for (auto& child : node->children_) {
        if (child) {
            child->parent_ = node;
        }
    }
    
    return node;
}

void VNode::appendChild(Ptr child) {
    if (!child) return;
    
    children_.push_back(child);
    child->parent_ = shared_from_this();
}

void VNode::removeChild(Ptr child) {
    if (!child) return;
    
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->parent_.reset();
        children_.erase(it);
    }
}

void VNode::replaceChild(Ptr oldChild, Ptr newChild) {
    if (!oldChild || !newChild) return;
    
    auto it = std::find(children_.begin(), children_.end(), oldChild);
    if (it != children_.end()) {
        oldChild->parent_.reset();
        *it = newChild;
        newChild->parent_ = shared_from_this();
    }
}

void VNode::insertBefore(Ptr newChild, Ptr sibling) {
    if (!newChild) return;
    
    if (!sibling) {
        appendChild(newChild);
        return;
    }
    
    auto it = std::find(children_.begin(), children_.end(), sibling);
    if (it != children_.end()) {
        children_.insert(it, newChild);
        newChild->parent_ = shared_from_this();
    } else {
        appendChild(newChild);
    }
}

void VNode::traversePreOrder(std::function<void(Ptr)> visitor) {
    visitor(shared_from_this());
    for (auto& child : children_) {
        if (child) {
            child->traversePreOrder(visitor);
        }
    }
}

void VNode::traversePostOrder(std::function<void(Ptr)> visitor) {
    for (auto& child : children_) {
        if (child) {
            child->traversePostOrder(visitor);
        }
    }
    visitor(shared_from_this());
}

void VNode::traverseLevelOrder(std::function<void(Ptr)> visitor) {
    std::queue<Ptr> queue;
    queue.push(shared_from_this());
    
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        
        visitor(current);
        
        for (auto& child : current->children_) {
            if (child) {
                queue.push(child);
            }
        }
    }
}

VNode::Ptr VNode::findChildByKey(const std::string& key) const {
    for (const auto& child : children_) {
        if (child && child->key_ && *child->key_ == key) {
            return child;
        }
        if (child) {
            auto found = child->findChildByKey(key);
            if (found) return found;
        }
    }
    return nullptr;
}

VNode::Ptr VNode::findById(uint64_t id) const {
    if (id_ == id) {
        return const_cast<VNode*>(this)->shared_from_this();
    }
    
    for (const auto& child : children_) {
        if (child) {
            auto found = child->findById(id);
            if (found) return found;
        }
    }
    return nullptr;
}

VNode::Ptr VNode::cloneShallow() const {
    auto cloned = std::make_shared<VNode>(type_);
    cloned->tag_ = tag_;
    cloned->text_ = text_;
    cloned->props_ = props_;
    cloned->key_ = key_;
    cloned->component_ = component_;
    // Don't copy children or parent
    return cloned;
}

VNode::Ptr VNode::cloneDeep() const {
    auto cloned = cloneShallow();
    
    for (const auto& child : children_) {
        if (child) {
            auto clonedChild = child->cloneDeep();
            cloned->appendChild(clonedChild);
        }
    }
    
    return cloned;
}

std::string VNode::serialize() const {
    std::ostringstream oss;
    
    switch (type_) {
        case VNodeType::Element:
            oss << "<" << tag_;
            if (key_) {
                oss << " key=\"" << *key_ << "\"";
            }
            oss << " id=\"" << id_ << "\"";
            oss << ">";
            for (const auto& child : children_) {
                if (child) {
                    oss << child->serialize();
                }
            }
            oss << "</" << tag_ << ">";
            break;
            
        case VNodeType::Text:
            oss << text_;
            break;
            
        case VNodeType::Component:
            oss << "<Component id=\"" << id_ << "\">";
            for (const auto& child : children_) {
                if (child) {
                    oss << child->serialize();
                }
            }
            oss << "</Component>";
            break;
            
        case VNodeType::Fragment:
            oss << "<Fragment>";
            for (const auto& child : children_) {
                if (child) {
                    oss << child->serialize();
                }
            }
            oss << "</Fragment>";
            break;
    }
    
    return oss.str();
}

bool VNode::operator==(const VNode& other) const {
    if (type_ != other.type_) return false;
    if (tag_ != other.tag_) return false;
    if (text_ != other.text_) return false;
    if (key_ != other.key_) return false;
    if (props_ != other.props_) return false;
    if (children_.size() != other.children_.size()) return false;
    
    for (size_t i = 0; i < children_.size(); ++i) {
        if (children_[i] && other.children_[i]) {
            if (*children_[i] != *other.children_[i]) return false;
        } else if (children_[i] != other.children_[i]) {
            return false;
        }
    }
    
    return true;
}

bool VNode::operator!=(const VNode& other) const {
    return !(*this == other);
}

} // namespace reactcpp

