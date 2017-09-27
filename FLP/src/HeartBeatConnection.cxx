#include "O2/FLP/HeartBeatConnection.h"
#include "O2/FLP/FLPDevice.h"
#include "O2/FLP/FLPSettings.h"
using namespace O2::FLP;

HeartbeatConnection::HeartbeatConnection(const FLPSettings& settings, Balancer::AbstractDevice* device) : Balancer::Connection("stf1", device){
    this->addChannel(
        Balancer::ConnectionType::Subscribe,
        Balancer::ConnectionMethod::Connect,
        settings.getInformationNodeSetting()->ip,
        settings.getInformationNodeSetting()->port
    );

    this->updateAllRateLogging(1);
    this->updateAllReceiveBuffer(10);



}
