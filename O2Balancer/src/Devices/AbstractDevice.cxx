#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Remote/ClusterManager.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Globals.h"
#include "O2/Balancer/Utilities/DeviceSetting.h"
#include <cstdlib>
#include <boost/format.hpp>
#include "O2/Balancer/Exceptions/InitException.h"

using namespace O2::Balancer;

AbstractDevice::AbstractDevice(const std::string& name, std::shared_ptr<Settings> settings){
    this->fId = name;
    this->settings = settings;
    this->nStop = false;
    this->nRefresh = false;
    this->clusterManager = std::shared_ptr<ClusterManager>(new ClusterManager(
        settings->getSettingsServer()->ip,
        settings->getSettingsServer()->port
    ));
    this->zooThread = std::thread(&AbstractDevice::checkZooKeeper,this );
   // this->fNetworkInterface = "default";
 //   this->fNumIoThreads = 1;
   // this->fPortRangeMin = 22000;
   // this->fPortRangeMax = 32000;
   // this->fInitializationTimeoutInS = 1;
    this->defaultTransport = this->getProperty(
        Globals::EnvironmentVariables::O2_TRANSPORT_VAR,
        Globals::FairMessageOptions::ZERO_MQ
    );
    variableChecksOut(
        this->defaultTransport, 
        Globals::FairMessageOptions::ZERO_MQ,
        Globals::FairMessageOptions::NANO_MSG,
        Globals::FairMessageOptions::SHARED_MEMORY
    );

    if(this->defaultTransport != Globals::FairMessageOptions::ZERO_MQ){
        LOG(WARN) << "ZeroMQ is the recommended transport, using " << this->defaultTransport << " might contain bugs";
    }
}

void AbstractDevice::checkZooKeeper(){
    while(!this->nStop){
        std::unique_lock<std::mutex> lck (this->zoolock);
        this->nRefresh = this->clusterManager->requiresUpdate();
    }
}

bool AbstractDevice::addHandle(const std::string& tag, const DeviceSetting& setting){
    std::unique_lock<std::mutex> lck (this->zoolock);
    const std::string name = (boost::format("%s:%s:%d") % tag % setting.ip % setting.port).str();
    return this->clusterManager->registerConnection(
        this->fId,
        name,
        setting
    );
}

bool AbstractDevice::needRefresh() const{
    return this->nRefresh;
}

     
bool AbstractDevice::needToStop() const{
    return this->nStop;
}

/*void AbstractDevice::PostRun(){
    LOG(INFO) << "Closing device : " << fId;
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}*/

std::string AbstractDevice::getDefaultTransport() const{
    return this->defaultTransport;
}

std::string AbstractDevice::getProperty(const std::string& varName, const std::string& defValue) {
    auto result = std::getenv(varName.c_str());

    if(result == nullptr && defValue != ""){
        return defValue;
    }
    if(result == nullptr){
        throw Exceptions::InitException("Could not load " + varName + "property");
    }
    return result;
}

std::shared_ptr<ClusterManager> AbstractDevice::getClusterManager() const{
    return this->clusterManager;
}

void AbstractDevice::quit(){
    this->nStop = true;
    LOG(INFO) << "closing thread";
    this->zooThread.join();
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}