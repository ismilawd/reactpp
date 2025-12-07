#pragma once

#include <functional>
#include <utility>

namespace reactpp {

// useReducer hook - to be implemented
template<typename State, typename Action>
std::pair<State, std::function<void(Action)>> useReducer(
    std::function<State(State, Action)> reducer,
    const State& initialState
);

} // namespace reactpp

