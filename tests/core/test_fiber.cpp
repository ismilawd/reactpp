#include <gtest/gtest.h>
#include "reactcpp/core/FiberNode.hpp"
#include "reactcpp/core/VNode.hpp"

using namespace reactcpp;

TEST(FiberNodeTest, Create) {
    auto vnode = VNode::createElement("div");
    auto fiber = FiberNode::create(vnode);
    
    ASSERT_NE(fiber, nullptr);
    EXPECT_EQ(fiber->type, VNodeType::Element);
    EXPECT_EQ(fiber->vnode, vnode);
}

TEST(FiberNodeTest, EffectTag) {
    auto vnode = VNode::createElement("div");
    auto fiber = FiberNode::create(vnode);
    
    fiber->markForUpdate(EffectTag::Placement);
    EXPECT_EQ(fiber->effectTag, EffectTag::Placement);
}

