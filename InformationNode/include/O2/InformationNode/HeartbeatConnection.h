#ifndef O2_INFORMATIONNODE_HEARTBEAT_CONNECTION_H
#define O2_INFORMATIONNODE_HEARTBEAT_CONNECTION_H

#include <O2/Balancer/Connection.h>
namespace O2{
    namespace InformationNode{
        class HeartbeatConnection : public Balancer::Connection{
        public:
            HeartbeatConnection(int port, Balancer::AbstractDevice* device);
        };
    }

}

#endif