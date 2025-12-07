#pragma once

#include <memory>

namespace reactpp {

template<typename T>
class Ref {
public:
    T current;
    
    Ref(const T& initialValue) : current(initialValue) {}
};

// useRef hook - to be implemented
template<typename T>
std::shared_ptr<Ref<T>> useRef(const T& initialValue);

} // namespace reactpp

