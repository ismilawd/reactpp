#include "reactcpp/core/ComponentInstance.hpp"
#include <stdexcept>

namespace reactcpp {

ComponentInstanceManager::ComponentInstanceManager()
    : nextInstanceId_(1) {
}

std::shared_ptr<Component> ComponentInstanceManager::createInstance(
    std::shared_ptr<Component> component,
    const Props& props) {
    
    if (!component) {
        throw std::runtime_error("Cannot create instance from null component");
    }
    
    uint64_t instanceId = component->getId();
    
    InstanceData data;
    data.component = component;
    data.hookManager = std::make_shared<HookManager>();
    data.hookManager->setComponentId(instanceId);
    data.mounted = false;
    
    component->setProps(props);
    component->setHookManager(data.hookManager);
    component->onCreate();
    
    instances_[instanceId] = std::move(data);
    
    return component;
}

std::shared_ptr<Component> ComponentInstanceManager::getInstance(uint64_t componentId) const {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        return nullptr;
    }
    return it->second.component;
}

void ComponentInstanceManager::updateInstance(uint64_t componentId, const Props& newProps) {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        throw std::runtime_error("Component instance not found: " + std::to_string(componentId));
    }
    
    Props oldProps = it->second.component->getProps();
    it->second.component->setProps(newProps);
    it->second.component->onUpdate(oldProps, newProps);
}

void ComponentInstanceManager::destroyInstance(uint64_t componentId) {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        return;
    }
    
    if (it->second.mounted) {
        unmountInstance(componentId);
    }
    
    instances_.erase(it);
}

std::shared_ptr<HookManager> ComponentInstanceManager::getHookManager(uint64_t componentId) {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        return nullptr;
    }
    return it->second.hookManager;
}

void ComponentInstanceManager::mountInstance(uint64_t componentId) {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        throw std::runtime_error("Component instance not found: " + std::to_string(componentId));
    }
    
    if (!it->second.mounted) {
        it->second.mounted = true;
        it->second.component->onMount();
    }
}

void ComponentInstanceManager::unmountInstance(uint64_t componentId) {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        return;
    }
    
    if (it->second.mounted) {
        it->second.mounted = false;
        it->second.component->onUnmount();
    }
}

bool ComponentInstanceManager::isMounted(uint64_t componentId) const {
    auto it = instances_.find(componentId);
    if (it == instances_.end()) {
        return false;
    }
    return it->second.mounted;
}

} // namespace reactcpp

