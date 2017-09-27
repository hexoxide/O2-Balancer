#include "O2/AbstractDevice.h"
#include "O2/Connection.h"
using namespace O2;

AbstractDevice::AbstractDevice(const std::string& name){
  this->fId = name;
  this->fNetworkInterface = "default";
  this->fNumIoThreads = 1;
///  this->fPortRangeMin = 22000;
 // this->fPortRangeMax = 32000;
  //this->fInitializationTimeoutInS = 1;
  
}

void AbstractDevice::addConnection(Connection connection){

    this->fChannels.insert(
        std::pair<std::string, std::vector<FairMQChannel>>(
            connection.getName(),
            std::move(connection.getChannels())
        )
    );
}