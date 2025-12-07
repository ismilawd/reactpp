#pragma once

#include "reactpp/core/Component.hpp"
#include "reactpp/core/VNode.hpp"
#include "reactpp/context/Context.hpp"

namespace reactpp {

template<typename T>
class ContextProvider : public Component {
public:
    ContextProvider(std::shared_ptr<Context<T>> context, const T& value)
        : context_(context), value_(value) {}
    
    VNode::Ptr render() override {
        // Return children
        return VNode::createFragment({});
    }
    
    T getValue() const { return value_; }
    void setValue(const T& value) { value_ = value; }
    
private:
    std::shared_ptr<Context<T>> context_;
    T value_;
};

} // namespace reactpp

