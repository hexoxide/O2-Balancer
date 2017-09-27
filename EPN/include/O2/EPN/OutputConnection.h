#ifndef O2_EPN_OUTPUTCONNECTION_H
#define O2_EPN_OUTPUTCONNECTION_H


#include <O2/Balancer/Connection.h>

namespace O2{
    namespace EPN{
        class OutputConnection : public Balancer::Connection{
        public:
            OutputConnection(Balancer::AbstractDevice* device);
        };
    }
}

#endif