#pragma once

#include <atomic>
#include <memory>

namespace reactpp {

template<typename T>
class Context {
public:
    using Ptr = std::shared_ptr<Context<T>>;
    
    static Ptr create(const T& defaultValue);
    
    T getDefaultValue() const { return defaultValue_; }
    uint64_t getId() const { return id_; }
    
private:
    Context(const T& defaultValue);
    
    static std::atomic<uint64_t> next_id_;
    uint64_t id_;
    T defaultValue_;
};

template<typename T>
std::atomic<uint64_t> Context<T>::next_id_{1};

template<typename T>
Context<T>::Context(const T& defaultValue)
    : id_(next_id_.fetch_add(1, std::memory_order_relaxed)),
      defaultValue_(defaultValue) {
}

template<typename T>
typename Context<T>::Ptr Context<T>::create(const T& defaultValue) {
    return std::make_shared<Context<T>>(defaultValue);
}

} // namespace reactpp

