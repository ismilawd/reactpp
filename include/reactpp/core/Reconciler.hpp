#pragma once

#include "VNode.hpp"
#include "FiberNode.hpp"

namespace reactpp {

class Reconciler {
public:
    Reconciler();
    ~Reconciler();
    
    // Reconciliation methods - to be implemented
    FiberNode::Ptr reconcile(VNode::Ptr current, VNode::Ptr next);
    
private:
    // Implementation details
};

} // namespace reactpp

