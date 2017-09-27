#include "O2/EPN/OutputConnection.h"

using namespace O2::EPN;

OutputConnection::OutputConnection(Balancer::AbstractDevice* device) : Balancer::Connection("tf", device){
    this->addChannel(
        Balancer::ConnectionType::Publish,
        Balancer::ConnectionMethod::Bind,
        "127.0.0.1",
        5591
    );
}