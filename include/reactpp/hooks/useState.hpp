#pragma once

#include "HookManager.hpp"
#include <functional>
#include <utility>

namespace reactpp {

// useState hook - to be implemented
template<typename T>
std::pair<T, std::function<void(T)>> useState(const T& initialValue);

template<typename T>
std::pair<T, std::function<void(T)>> useState(std::function<T()> initializer);

} // namespace reactpp

