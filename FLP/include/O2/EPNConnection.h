#ifndef O2_FLP_EPN_CONNECTION_H
#define O2_FLP_EPN_CONNECTION_H

#include <O2/Connection.h>

namespace O2{
    namespace FLP{

        class FLPSettings;
        class EPNConnection : public Connection{
        public:
            EPNConnection(const FLPSettings& settings,AbstractDevice* device);
        };
    }
}

#endif