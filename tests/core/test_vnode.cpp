#include <gtest/gtest.h>
#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"

using namespace reactpp;

TEST(VNodeTest, CreateElement) {
    auto node = VNode::createElement("div");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), VNodeType::Element);
    EXPECT_EQ(node->getTag(), "div");
}

TEST(VNodeTest, CreateText) {
    auto node = VNode::createText("Hello");
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getType(), VNodeType::Text);
    EXPECT_EQ(node->getText(), "Hello");
}

TEST(VNodeTest, AppendChild) {
    auto parent = VNode::createElement("div");
    auto child = VNode::createText("Child");
    
    parent->appendChild(child);
    
    EXPECT_EQ(parent->getChildren().size(), 1);
    EXPECT_EQ(parent->getChildren()[0], child);
}

TEST(VNodeTest, TreeTraversal) {
    auto root = VNode::createElement("root");
    auto child1 = VNode::createElement("child1");
    auto child2 = VNode::createElement("child2");
    
    root->appendChild(child1);
    root->appendChild(child2);
    
    int count = 0;
    root->traversePreOrder([&count](VNode::Ptr) { count++; });
    
    EXPECT_EQ(count, 3); // root + 2 children
}

TEST(VNodeTest, CloneDeep) {
    auto original = VNode::createElement("div");
    original->setKey("test-key");
    auto child = VNode::createText("child");
    original->appendChild(child);
    
    auto cloned = original->cloneDeep();
    
    EXPECT_NE(cloned, original);
    EXPECT_EQ(cloned->getTag(), original->getTag());
    EXPECT_EQ(cloned->getKey(), original->getKey());
    EXPECT_EQ(cloned->getChildren().size(), 1);
    EXPECT_NE(cloned->getChildren()[0], original->getChildren()[0]);
}

TEST(VNodeTest, FindByKey) {
    auto root = VNode::createElement("root");
    auto child1 = VNode::createElement("child1");
    child1->setKey("key1");
    auto child2 = VNode::createElement("child2");
    child2->setKey("key2");
    
    root->appendChild(child1);
    root->appendChild(child2);
    
    auto found = root->findChildByKey("key2");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getKey().value(), "key2");
}

