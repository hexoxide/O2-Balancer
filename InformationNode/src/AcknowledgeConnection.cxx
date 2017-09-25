#include "O2/AcknowledgeConnection.h"

using namespace O2;

AcknowledgeConnection::AcknowledgeConnection() : Connection("ack"){
    this->addChannel(ConnectionType::Pull, ConnectionMethod::Connect, 5990 );
}