#ifndef O2_BALANCER_REMOTE_CLUSTER_MANAGER_H
#define O2_BALANCER_REMOTE_CLUSTER_MANAGER_H

#include <string>
#include <vector>
#include <memory>
//forward declaration
typedef struct _zhandle zhandle_t;


namespace O2{
    namespace Balancer{

        
        class DeviceSetting;

        class ClusterManager{
            zhandle_t *zh;
            void setupDirectories();
        public:
            ClusterManager(const std::string& zooServer, const int& port);
            void addGlobalVariable(const std::string& name, const std::string& value);
            std::string getGlobalVariable(const std::string& name);
            void registerConnection(const std::string& classification, const std::string& tag, const DeviceSetting& setting );
            void close();
        };
    }
}

#endif