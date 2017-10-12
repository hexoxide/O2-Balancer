#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Remote/ClusterManager.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Globals.h"
#include "O2/Balancer/Utilities/DeviceSetting.h"
#include <cstdlib>

#include "O2/Balancer/Exceptions/InitException.h"

using namespace O2::Balancer;



AbstractDevice::AbstractDevice(const std::string& name, std::shared_ptr<Settings> settings){
    this->fId = name;
    this->settings = settings;
    this->clusterManager = std::shared_ptr<ClusterManager>(new ClusterManager(
        settings->getSettingsServer()->ip,
        settings->getSettingsServer()->port
    ));
    this->fNetworkInterface = "default";
    this->fNumIoThreads = 1;
    this->fPortRangeMin = 22000;
    this->fPortRangeMax = 32000;
    this->fInitializationTimeoutInS = 1;
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
}

void AbstractDevice::PreRun(){


}

void AbstractDevice::addHandle(const std::string& tag, const DeviceSetting& setting){
    this->clusterManager->registerConnection(
        this->fId,
        tag,
        setting
    );
}

void AbstractDevice::PostRun(){
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}

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

void AbstractDevice::update(){
    for(auto i : this->connnections){
        i->updateConnection(this->clusterManager);
    }
}

void AbstractDevice::addConnection(std::shared_ptr<Connection> connection){
    this->connnections.push_back(connection);
    this->fChannels.insert(
        std::pair<std::string, std::vector<FairMQChannel>>(
            connection->getName(),
            std::move(connection->getChannels())
        )
    );
}