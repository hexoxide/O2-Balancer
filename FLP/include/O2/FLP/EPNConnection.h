#ifndef O2_FLP_EPN_CONNECTION_H
#define O2_FLP_EPN_CONNECTION_H

#include <O2/Balancer/Connection.h>

namespace O2{
    namespace FLP{
        class FLPSettings;
        class EPNConnection : public Balancer::Connection{
        public:
            EPNConnection(const FLPSettings& settings, Balancer::AbstractDevice* device);
        };
    }
}

#endif