#ifndef O2_EPN_FLPCONNECTION_H
#define O2_EPN_FLPCONNECTION_H

#include <O2/Balancer/Connection.h>

namespace O2{
    namespace EPN{
        class FLPConnection : public Balancer::Connection{
        public:
            FLPConnection(Balancer::AbstractDevice* device);
        };
    }
}

#endif