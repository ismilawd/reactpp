#include <gtest/gtest.h>
#include "reactcpp/context/Context.hpp"

using namespace reactcpp;

TEST(ContextTest, Create) {
    auto context = Context<int>::create(42);
    ASSERT_NE(context, nullptr);
    EXPECT_EQ(context->getDefaultValue(), 42);
}

