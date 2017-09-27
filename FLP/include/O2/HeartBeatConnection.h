#ifndef O2_FLP_HEARTBEAT_CONNECTION_H
#define O2_FLP_HEARTBEAT_CONNECTION_H

#include <O2/Connection.h>

namespace O2{
    namespace FLP{

        class FLPSettings;

        class HeartbeatConnection : public Connection{
        public:
            HeartbeatConnection(const FLPSettings& settings, AbstractDevice* device);
        };
    }
}

#endif