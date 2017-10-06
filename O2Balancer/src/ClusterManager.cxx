#include "O2/Balancer/ClusterManager.h"
#include "O2/Balancer/ClusterNodes.h"
#include <zookeeper/zookeeper.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <iostream>
#include <arpa/inet.h>
using namespace O2::Balancer;

ClusterManager::ClusterManager(const std::string& zooServer, const int& port){
    const std::string server = zooServer + ":" + std::to_string(port);
    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    this->zh = zookeeper_init(server.c_str(), [](zhandle_t *zzh, int type, int state, const char *path,
        void *watcherCtx)-> void {
            std::cout << std::string(path) << "\n";

    }, 10000, 0, 0, 0);
}

void ClusterManager::registerCluster(const std::string& type){
    this->clusterType = type;
    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_CREATE, ZOO_AUTH_IDS}};
    const std::string ip = this->getOwnIpAddress();
    const std::string dir = "/" + type + "/" + ip; 
    char buffer[512];
    int rc = zoo_create(zh,dir.c_str(),ip.c_str(), ip.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,
                        nullptr, 0);
    if(rc != ZOK){
       
    }
}

std::string ClusterManager::getOwnIpAddress() const{
    struct ifaddrs *ifAddrStruct = nullptr;
    struct ifaddrs *ifa = nullptr;
    void* tmpAddrPtr = nullptr;

    getifaddrs(&ifAddrStruct);
    std::string result;
    for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            result = std::string(addressBuffer);
        } 
    }
    if (ifAddrStruct!=nullptr) freeifaddrs(ifAddrStruct);

    return result;
}

void ClusterManager::close(){
    zookeeper_close(this->zh);
}