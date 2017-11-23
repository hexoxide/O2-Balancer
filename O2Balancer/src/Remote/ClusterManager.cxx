// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/Balancer/Remote/ClusterManager.h"
#include <boost/format.hpp> 
#include "O2/Balancer/Exceptions/ClusterTypeException.h"
#include "O2/Balancer/Utilities/DataTypes.h"
#include "O2/Balancer/Exceptions/TimeOutException.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"
#include "FairMQLogger.h"
#include <zookeeper/zookeeper.h>
#include "O2/Balancer/Globals.h"

#include <chrono>
#include <thread>
#include <regex>

using namespace O2::Balancer;
//For events
std::vector<std::string> changedPaths;

ClusterManager::ClusterManager(const std::string& zooServer,
                               const int& port) {
    
    const std::string server = zooServer + ":" + std::to_string(port);
    zoo_set_debug_level(ZOO_LOG_LEVEL_INFO);
    this->zh = zookeeper_init(server.c_str(), [](zhandle_t *zzh, int type, int state, const char *path,
        void *watcherCtx)-> void {
            if(type == ZOO_CHILD_EVENT){
                LOG(INFO) << boost::format("Child event happened at %s") % std::string(path);
                changedPaths.emplace_back(path);
            }

    }, 5000, nullptr, nullptr, 0);

    this->setupDirectories();
}

bool ClusterManager::requiresUpdate() const {
    return !changedPaths.empty();
}

std::string ClusterManager::pathThatNeedsUpdate() {
    if(!changedPaths.empty()) {
        std::string dat = changedPaths.back();
        changedPaths.pop_back();
        return dat;
    } 
    throw Exceptions::UnimplementedException("Array out of index");
}

void ClusterManager::addGlobalString(const std::string& name,
                                     const std::string& value) {

    const std::string dir = std::string(Globals::ZooKeeperTopology::VAR_ZNODE_ROOT) + "/" + name;
    int rc = zoo_create(zh,dir.c_str(),value.c_str(), value.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL,
    nullptr, 0);
    if(rc != ZOK){
        throw Exceptions::ClusterHandlerException((boost::format("Could not create a variable with name %s and value %s") % name % value).str());
    }
}

void ClusterManager::addGlobalInteger(const std::string& name,
                                      int value) {
    this->addGlobalString(name, std::to_string(value));
}
        
int ClusterManager::getGlobalInteger(const std::string& name,
                                     int timeout) {
    try {
        return std::stoi(getGlobalString(name, timeout));
    } catch(const std::invalid_argument& ex) {
        const auto msg = boost::format("Global variable %s is not an integer" ) % name;
        throw Exceptions::ClusterTypeException(msg.str());
    }
}

std::string ClusterManager::addRootIfNecessary(const std::string &name) const {
    if(name.length() > 0){
        if(name[0] != '/'){
            return "/" + name;
        }
    }
    return name;
}

std::string ClusterManager::getGlobalString(const std::string& name,
                                            int timeout) {

    const std::string directory = std::string(Globals::ZooKeeperTopology::VAR_ZNODE_ROOT) + "/" + name;
    constexpr int JUMP = 100;
    int currentWaitTime = 0;
    int done = 0;

    while(done == 0) {

        int bufferLength = 512;
        char buffer[bufferLength];
        struct Stat stat;
        done = zoo_get(this->zh, directory.c_str()  , 0, buffer, &bufferLength, &stat);

        if(done == ZOK) {
            return std::string(buffer, bufferLength);
        } else {

            if(timeout == 0 && currentWaitTime > timeout){
                throw Exceptions::TimeOutException((boost::format("Query for finding variable %s did timeout after %i milliseconds") % name % timeout).str());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(JUMP));
            currentWaitTime += JUMP;
        }
    }
    throw Exceptions::ClusterHandlerException((boost::format("Could not get variable %s") % name).str());
}

void ClusterManager::setupDirectories() {

    struct Stat v;
    if(zoo_exists(zh,Globals::ZooKeeperTopology::VAR_ZNODE_ROOT, true, &v) != ZOK) {

        int rc = zoo_create(zh, Globals::ZooKeeperTopology::VAR_ZNODE_ROOT, "", 0,  &ZOO_OPEN_ACL_UNSAFE, 0 , nullptr, 0);

        if(rc != ZOK) {
            LOG(ERROR) << "Could not create the globals directory";
        }
    }  
}


bool ClusterManager::registerConnection(const std::string& classification,
                                        const std::string& tag,
                                        const DeviceSetting& setting ) {

    std::string dir = this->addRootIfNecessary(classification);
    int rc;
    struct Stat v;

    if(zoo_exists(zh,dir.c_str(), true, &v) != ZOK) {
      
        rc = zoo_create(zh, dir.c_str(), "", 0,  &ZOO_OPEN_ACL_UNSAFE, 0 , nullptr, 0);   
        if(rc != ZOK) {
            LOG(ERROR) << boost::format("Could not create folder %s") % classification;
        }
    }

    dir += "/" + tag;
    const std::string value = setting.ip + ":" + std::to_string(setting.port); 
    rc = zoo_create(zh,dir.c_str(),value.c_str(), value.length(), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL | ZOO_SEQUENCE ,nullptr, 0);
    return rc == ZOK;
}


std::vector<DeviceSetting> ClusterManager::getRegisteredConnections(const std::string& classification,
                                                                    const std::string& tag) {
    std::vector<DeviceSetting> result;
    struct String_vector vec;
    std::string dir = this->addRootIfNecessary(classification);

    //All connections listed are sequential(just to keep generic code)
    int rc = zoo_get_children(this->zh, dir.c_str(), 1, &vec);

    if(rc == ZOK) {

        for(int32_t i = 0; i < vec.count; i++) {

            std::string tmp = dir + "/";
            tmp.append(vec.data[i]);
            //Is the tag part of the string

            if (tmp.find(tag) != std::string::npos) {
                int bufferLength = 512;
                char buffer[bufferLength];
                struct Stat stat;
                if(zoo_get(this->zh, tmp.c_str(),0, buffer, &bufferLength, &stat) == ZOK){
                    result.emplace_back(std::string(buffer,bufferLength));
                }
            }
        }
    }
    std::sort(result.begin(), result.end(), [this](DeviceSetting l, DeviceSetting r) -> bool {
        if(l.ip == r.ip) {
            return l.port > r.port;
        }
        return l.ip > r.ip;
    } );
 
    return result;
}
 

void ClusterManager::close() {
   zookeeper_close(this->zh);
}