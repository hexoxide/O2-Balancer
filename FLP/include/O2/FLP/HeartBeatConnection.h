#ifndef O2_FLP_HEARTBEAT_CONNECTION_H
#define O2_FLP_HEARTBEAT_CONNECTION_H

#include <O2/Balancer/Connection.h>

namespace O2{
    namespace FLP{
        class FLPSettings;

        class HeartbeatConnection : public Balancer::Connection{
        public:
            HeartbeatConnection(const FLPSettings& settings, Balancer::AbstractDevice* device);
        };
    }
}

#endif