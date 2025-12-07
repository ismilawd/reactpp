#pragma once

#include <functional>
#include <vector>
#include <any>

namespace reactcpp {

// useMemo hook - to be implemented
template<typename T>
T useMemo(std::function<T()> computation, const std::vector<std::any>& dependencies);

} // namespace reactcpp

