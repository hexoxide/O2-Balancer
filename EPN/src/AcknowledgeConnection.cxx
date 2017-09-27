#include "O2/EPN/AcknowledgeConnection.h"

using namespace O2::EPN;

AcknowledgeConnection::AcknowledgeConnection(Balancer::AbstractDevice* device) : Balancer::Connection("ack", device){
    this->addChannel(
        Balancer::ConnectionType::Push,
        Balancer::ConnectionMethod::Connect,
        "127.0.0.1",
        5990
    );
}