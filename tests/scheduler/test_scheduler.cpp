#include <gtest/gtest.h>
#include "reactcpp/scheduler/UpdateScheduler.hpp"

using namespace reactcpp;

TEST(UpdateSchedulerTest, Placeholder) {
    auto scheduler = std::make_shared<UpdateScheduler>();
    EXPECT_NE(scheduler, nullptr);
}

