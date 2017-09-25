#ifndef O2_HEARTBEAT_CONNECTION_H
#define O2_HEARTBEAT_CONNECTION_H

#include <O2/Connection.h>
namespace O2{
    class HeartbeatConnection : public Connection{
    public:
        HeartbeatConnection();
    };
}

#endif