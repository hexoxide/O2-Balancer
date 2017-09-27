#ifndef O2_BALANCER_DEVICE_SETTING_H
#define O2_BALANCER_DEVICE_SETTING_H

#include <string>

namespace O2{
    namespace Balancer{
        struct DeviceSetting{
            int port;
            std::string ip;
            DeviceSetting(int port, std::string ip);
        };
    }
}

#endif