#include <gtest/gtest.h>
#include "reactpp/events/SyntheticEvent.hpp"

using namespace reactpp::events;

TEST(SyntheticEventTest, Create) {
    SyntheticEvent event("click");
    EXPECT_EQ(event.type, "click");
    EXPECT_FALSE(event.defaultPrevented);
    EXPECT_FALSE(event.propagationStopped);
}

TEST(SyntheticEventTest, PreventDefault) {
    SyntheticEvent event("click");
    event.preventDefault();
    EXPECT_TRUE(event.defaultPrevented);
}

