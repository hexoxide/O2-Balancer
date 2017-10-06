#ifndef O2_BALANCER_CLUSTER_MANAGER_H
#define O2_BALANCER_CLUSTER_MANAGER_H

#include <string>
#include <vector>
#include <memory>
//forward declaration
typedef struct _zhandle zhandle_t;


namespace O2{
    namespace Balancer{

        class ClusterNode;
        class ClusterManager{
            zhandle_t *zh;
            std::string getOwnIpAddress() const;
            std::shared_ptr<ClusterNode> root;
            std::string clusterType;
        public:
            ClusterManager(const std::string& zooServer, const int& port);
            void registerCluster(const std::string& type);
            void close();
        };
    }
}

#endif