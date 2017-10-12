// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_FLP_HEARTBEAT_CONNECTION_H
#define O2_FLP_HEARTBEAT_CONNECTION_H

#include <O2/Balancer/Devices/Connection.h>

namespace O2{
    namespace FLP{
        class FLPSettings;

        class HeartbeatConnection : public Balancer::Connection{
        public:
            HeartbeatConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice* device);
        };
    }
}

#endif