#pragma once

#include <functional>
#include <vector>
#include <any>

namespace reactcpp {

// useCallback hook - to be implemented
template<typename F>
F useCallback(F callback, const std::vector<std::any>& dependencies);

} // namespace reactcpp

