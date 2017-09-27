#include "O2/EPNConnection.h"
#include "O2/FLPDevice.h"
#include "O2/FLPSettings.h"
using namespace O2::FLP;

EPNConnection::EPNConnection(const FLPSettings& settings, AbstractDevice* device) : Connection("stf2", device){
    for(const auto& epn : settings.getEPNSettings()){
        this->addChannel(
            ConnectionType::Push,
            ConnectionMethod::Connect,
            epn->ip,
            epn->port
        );
    }

    this->updateAllSendBuffer(1);
    this->updateAllRateLogging(1);
    
}