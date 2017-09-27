#include "O2/EPN/FLPConnection.h"

using namespace O2::EPN;

FLPConnection::FLPConnection(Balancer::AbstractDevice* device) : Balancer::Connection("stf2", device){
    this->addChannel(
        Balancer::ConnectionType::Pull,
        Balancer::ConnectionMethod::Bind,
        "127.0.0.1",
        5561
    );

    this->updateAllReceiveBuffer(10);

}