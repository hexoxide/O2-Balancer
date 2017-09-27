#ifndef O2_EPN_EPNACKNOWLEDGE_CONNECTION_H
#define O2_EPN_EPNACKNOWLEDGE_CONNECTION_H

#include <O2/Balancer/Connection.h>

namespace O2{
    namespace EPN{
        class AcknowledgeConnection : public Balancer::Connection{
        public:
            AcknowledgeConnection(Balancer::AbstractDevice* device);
        };
    }
}

#endif