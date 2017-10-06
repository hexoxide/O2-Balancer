#include "O2/Balancer/ClusterNodes.h"

using namespace O2::Balancer;

ClusterNode::ClusterNode(ClusterNode* parent, const std::string& name){
    this->parent = parent;
    this->name = name;
}

std::string ClusterNode::getName() const{
    return this->name;
}
ClusterNode* ClusterNode::getParent() const{
    return this->parent;
}