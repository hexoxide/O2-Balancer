#include "O2/Balancer/Remote/ClusterManager.h"
#include "FairMQLogger.h"
#include <zookeeper/zookeeper.h>

using namespace O2::Balancer;

ClusterManager::ClusterManager(const std::string& zooServer, const int& port){
    const std::string server = zooServer + ":" + std::to_string(port);
    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
    this->zh = zookeeper_init(server.c_str(), [](zhandle_t *zzh, int type, int state, const char *path,
        void *watcherCtx)-> void {
            LOG(INFO) << std::string(path) << "\n";

    }, 10000, 0, 0, 0);

    this->setupDirectories();
}

void ClusterManager::addGlobalVariable(const std::string& name, const std::string& value){
    //struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL, ZOO_AUTH_IDS}};
    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL, ZOO_AUTH_IDS}};
    struct ACL_vector CREATE_ONLY = {1, CREATE_ONLY_ACL};
    const std::string dir = "/globals/" + name;
    char buffer[512];


    int rc = zoo_create(zh,dir.c_str(),value.c_str(), value.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,
    nullptr, 0);
    if(rc != ZOK){
        LOG(ERROR) << "Could not create variable";
    }

}

std::string ClusterManager::getGlobalVariable(const std::string& name){
    int buflen = 512;
    char buffer[buflen]; 
  
    struct Stat stat;
    const std::string directory = "/globals/" + name;
    int rc = zoo_get(this->zh, directory.c_str()  , 0, buffer, &buflen, &stat);
    if(rc != ZOK){
        return std::string("");
    }

    return std::string(buffer, buflen);
}

void ClusterManager::setupDirectories(){
//    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL}};
    int rc = zoo_create(zh, "/globals", "", 0,  &ZOO_OPEN_ACL_UNSAFE, 0 , nullptr, 0);

    if(rc != ZOK){
        LOG(ERROR) << "Could not create the globals directory";
    }
}

void ClusterManager::registerConnection(const std::string& classification, const std::string& tag, const DeviceSetting& setting ){

}




void ClusterManager::close(){
   zookeeper_close(this->zh);
}