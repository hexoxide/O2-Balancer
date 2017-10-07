#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include <cstdlib>

#include "O2/Balancer/Exceptions/InitException.h"

using namespace O2::Balancer;



AbstractDevice::AbstractDevice(const std::string& name){
  this->fId = name;
  this->fNetworkInterface = "default";
  this->fNumIoThreads = 1;
  this->fPortRangeMin = 22000;
  this->fPortRangeMax = 32000;
  this->fInitializationTimeoutInS = 1;
  this->defaultTransport = this->getProperty("O2Transport", "zeromq");
  variableChecksOut(this->defaultTransport, "zeromq", "nanomsg","shmem");
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

void AbstractDevice::addConnection(Connection connection){
    this->connnections.push_back(connection);
    this->fChannels.insert(
        std::pair<std::string, std::vector<FairMQChannel>>(
            connection.getName(),
            std::move(connection.getChannels())
        )
    );
}