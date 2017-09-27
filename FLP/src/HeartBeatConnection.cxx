#include "O2/HeartBeatConnection.h"
#include "O2/FLPDevice.h"
#include "O2/FLPSettings.h"
using namespace O2::FLP;

HeartbeatConnection::HeartbeatConnection(const FLPSettings& settings, AbstractDevice* device) : Connection("stf1", device){
    this->addChannel(
        ConnectionType::Subscribe,
        ConnectionMethod::Connect,
        settings.getInformationNodeSetting()->ip,
        settings.getInformationNodeSetting()->port
    );

    this->updateAllRateLogging(1);
    this->updateAllReceiveBuffer(10);



}
