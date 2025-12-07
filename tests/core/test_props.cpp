#include <gtest/gtest.h>
#include "reactpp/core/Props.hpp"
#include <string>
#include <functional>

using namespace reactpp;

TEST(PropsTest, SetAndGet) {
    Props props;
    props.set("name", std::string("test"));
    props.set("value", 42);
    
    EXPECT_EQ(props.get<std::string>("name"), "test");
    EXPECT_EQ(props.get<int>("value"), 42);
}

TEST(PropsTest, HasProperty) {
    Props props;
    props.set("test", 1);
    
    EXPECT_TRUE(props.has("test"));
    EXPECT_FALSE(props.has("nonexistent"));
}

TEST(PropsTest, TryGet) {
    Props props;
    props.set("value", 42);
    
    auto result = props.tryGet<int>("value");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 42);
    
    auto missing = props.tryGet<int>("missing");
    EXPECT_FALSE(missing.has_value());
}

TEST(PropsTest, TypeMismatch) {
    Props props;
    props.set("value", std::string("test"));
    
    EXPECT_THROW(props.get<int>("value"), std::runtime_error);
}

TEST(PropsTest, Merge) {
    Props props1;
    props1.set("a", 1);
    props1.set("b", 2);
    
    Props props2;
    props2.set("b", 3);
    props2.set("c", 4);
    
    props1.merge(props2);
    
    EXPECT_EQ(props1.get<int>("a"), 1);
    EXPECT_EQ(props1.get<int>("b"), 3); // Overwritten
    EXPECT_EQ(props1.get<int>("c"), 4);
}

TEST(PropsTest, Equality) {
    Props props1;
    props1.set("a", 1);
    props1.set("b", std::string("test"));
    
    Props props2;
    props2.set("a", 1);
    props2.set("b", std::string("test"));
    
    EXPECT_EQ(props1, props2);
    
    props2.set("c", 3);
    EXPECT_NE(props1, props2);
}

