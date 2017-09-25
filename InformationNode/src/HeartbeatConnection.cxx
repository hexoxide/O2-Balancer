#include "O2/HeartbeatConnection.h"

using namespace O2;

HeartbeatConnection::HeartbeatConnection() : Connection("stf1"){
    this->addChannel(ConnectionType::Publish, ConnectionMethod::Bind, 5550 );
}