#pragma once

#include <functional>
#include <vector>
#include <any>

namespace reactcpp {

// useEffect hook - to be implemented
void useEffect(std::function<void()> effect, const std::vector<std::any>& dependencies = {});

} // namespace reactcpp

