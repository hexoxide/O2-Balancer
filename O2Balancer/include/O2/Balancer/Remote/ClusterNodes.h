#ifndef O2_BALANCER_REMOTE_CLUSTER_NODES_H
#define O2_BALANCER_REMOTE_CLUSTER_NODES_H

#include <vector>
#include <memory>
#include <string>
namespace O2{
    namespace Balancer{
        class ClusterNode{
        protected:
            ClusterNode* parent;
            std::string name;
        public:
            ClusterNode(ClusterNode* parent, const std::string& name);
            std::string getName() const;
            ClusterNode* getParent() const;
            //std::shared_ptr<ClusterNode> children;
        };

        class ClusterNodeContainer : public ClusterNode{
            std::vector<std::shared_ptr<ClusterNode>> children;
        };

        class ClusterNodeData : public ClusterNode{
            std::string text;
        public:
            std::string getText() const;
        };

    }
}

#endif