#include <gtest/gtest.h>
#include "reactpp/scheduler/UpdateScheduler.hpp"

using namespace reactpp;

TEST(UpdateSchedulerTest, Placeholder) {
    auto scheduler = std::make_shared<UpdateScheduler>();
    EXPECT_NE(scheduler, nullptr);
}

