#include <gtest/gtest.h>
#include "reactpp/core/Component.hpp"
#include "reactpp/core/VNode.hpp"
#include "reactpp/core/Props.hpp"

using namespace reactpp;

class TestComponent : public Component {
public:
    bool onCreateCalled = false;
    bool onMountCalled = false;
    bool onUpdateCalled = false;
    bool onUnmountCalled = false;
    
    void onCreate() override {
        onCreateCalled = true;
    }
    
    void onMount() override {
        onMountCalled = true;
    }
    
    void onUpdate(const Props& oldProps, const Props& newProps) override {
        onUpdateCalled = true;
    }
    
    void onUnmount() override {
        onUnmountCalled = true;
    }
    
    VNode::Ptr render() override {
        return VNode::createElement("div");
    }
};

TEST(ComponentTest, Lifecycle) {
    auto component = std::make_shared<TestComponent>();
    
    component->onCreate();
    EXPECT_TRUE(component->onCreateCalled);
    
    component->onMount();
    EXPECT_TRUE(component->onMountCalled);
    
    Props oldProps, newProps;
    component->onUpdate(oldProps, newProps);
    EXPECT_TRUE(component->onUpdateCalled);
    
    component->onUnmount();
    EXPECT_TRUE(component->onUnmountCalled);
}

TEST(ComponentTest, Props) {
    auto component = std::make_shared<TestComponent>();
    
    Props props;
    props.set("test", std::string("value"));
    
    component->setProps(props);
    EXPECT_EQ(component->getProps().get<std::string>("test"), "value");
}

TEST(ComponentTest, ComponentId) {
    auto component1 = std::make_shared<TestComponent>();
    auto component2 = std::make_shared<TestComponent>();
    
    EXPECT_NE(component1->getId(), component2->getId());
}

