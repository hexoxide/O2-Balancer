#include "O2/FLP/EPNConnection.h"
#include "O2/FLP/FLPDevice.h"
#include "O2/FLP/FLPSettings.h"
using namespace O2::FLP;

EPNConnection::EPNConnection(const FLPSettings& settings, Balancer::AbstractDevice* device) : Balancer::Connection("stf2", device){
    for(const auto& epn : settings.getEPNSettings()){
        this->addChannel(
            Balancer::ConnectionType::Push,
            Balancer::ConnectionMethod::Connect,
            epn->ip,
            epn->port
        );
    }

    this->updateAllSendBuffer(1);
    this->updateAllRateLogging(1);
    
}