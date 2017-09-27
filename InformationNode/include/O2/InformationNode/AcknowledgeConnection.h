#ifndef O2_INFORMATIONNODE_ACKNOWLEDGE_CONNECTION_H
#define O2_INFORMATIONNODE_ACKNOWLEDGE_CONNECTION_H

#include <O2/Balancer/Connection.h>

namespace O2{
    namespace InformationNode{
        class AcknowledgeConnection : public Balancer::Connection{
        public:
            AcknowledgeConnection(int port, Balancer::AbstractDevice* device);
        };
    }

}

#endif