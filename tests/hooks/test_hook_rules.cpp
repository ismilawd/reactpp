#include <gtest/gtest.h>
#include "reactpp/hooks/HookManager.hpp"

using namespace reactpp;

TEST(HookManagerTest, Reset) {
    HookManager manager;
    manager.reset();
    EXPECT_EQ(manager.getCurrentIndex(), 0);
}

TEST(HookManagerTest, HookState) {
    HookManager manager;
    manager.setHookState(0, std::make_any<int>(42));
    
    auto& state = manager.getHookState(0);
    EXPECT_EQ(std::any_cast<int>(state), 42);
}

