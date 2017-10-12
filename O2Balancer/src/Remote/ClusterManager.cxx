#include "O2/Balancer/Remote/ClusterManager.h"
#include <boost/format.hpp> 
#include "O2/Balancer/Exceptions/ClusterTypeException.h"
#include "O2/Balancer/Exceptions/TimeOutException.h"
#include "FairMQLogger.h"
#include <zookeeper/zookeeper.h>
#include "O2/Balancer/Globals.h"
#include <chrono>
#include <thread>

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

void ClusterManager::addGlobalString(const std::string& name, const std::string& value){
    //struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL, ZOO_AUTH_IDS}};
    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL, ZOO_AUTH_IDS}};
    struct ACL_vector CREATE_ONLY = {1, CREATE_ONLY_ACL};
    const std::string dir = std::string(Globals::ZooKeeperTopology::VAR_ZNODE_ROOT) + "/" + name;
    //"/globals/" + name;
    char buffer[512];


    int rc = zoo_create(zh,dir.c_str(),value.c_str(), value.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,
    nullptr, 0);
    if(rc != ZOK){
       // LOG(ERROR) << "Could not create variable";
        throw Exceptions::ClusterHandlerException((boost::format("Could not create a variable with name %s and value %s") % name % value).str());
    }

}

void ClusterManager::addGlobalInteger(const std::string& name, int value){
    this->addGlobalString(name, std::to_string(value));
}

            
int ClusterManager::getGlobalInteger(const std::string& name, int timeout){
    try{
        return std::stoi(getGlobalString(name, timeout));
    } catch(const std::invalid_argument& ex){
        const auto msg = boost::format("Global variable %s is not an integer" ) % name;
        throw Exceptions::ClusterTypeException(msg.str());
    }
}

std::string ClusterManager::getGlobalString(const std::string& name, int timeout){

    const std::string directory = std::string(
        Globals::ZooKeeperTopology::VAR_ZNODE_ROOT) + "/" + name;
    //"/globals/" + name;
    constexpr int JUMP = 100;
    int currentWaitTime = 0;
    int done = 0;
    while(done == 0){
        int buflen = 512;
        char buffer[buflen]; 
        struct Stat stat;
        done = zoo_get(this->zh, directory.c_str()  , 0, buffer, &buflen, &stat);
        if(done == ZOK){
            return std::string(buffer, buflen);
        } else {
            if(timeout == 0 && currentWaitTime > timeout){
                throw Exceptions::TimeOutException((boost::format("Query for finding variable %s did timeout after %i milliseconds") % name % timeout).str());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(JUMP));
            currentWaitTime += JUMP;
        }
    }
    throw Exceptions::ClusterHandlerException((boost::format("Could not get variable %s") % name).str());
   // return std::string("");
}

void ClusterManager::setupDirectories(){
//    struct ACL CREATE_ONLY_ACL[] = {{ZOO_PERM_ALL}};
    int rc = zoo_create(zh, Globals::ZooKeeperTopology::VAR_ZNODE_ROOT, "", 0,  &ZOO_OPEN_ACL_UNSAFE, 0 , nullptr, 0);

    if(rc != ZOK){
        LOG(ERROR) << "Could not create the globals directory";
    }
}


void ClusterManager::registerConnection(const std::string& classification, const std::string& tag, const DeviceSetting& setting ){

}




void ClusterManager::close(){
   zookeeper_close(this->zh);
}