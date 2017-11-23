// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_FLP_EPN_CONNECTION_H
#define O2_FLP_EPN_CONNECTION_H

#include <O2/Balancer/Devices/Connection.h>
#include <O2/Balancer/Utilities/DataTypes.h>
#include <vector>
#include <string>


namespace O2{
    namespace FLP{
        class FLPSettings;
        class DeviceSetting;
        class EPNConnection : public Balancer::Connection{
            std::vector<std::string> offlineEPNS;
            size_t incrementer = 0;
        public:
            EPNConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice* device);
            size_t amountOfEpns() const;
            size_t balance(O2::Balancer::heartbeatID id);
            void updateConnection();
            void updateBlacklist();
        };
    }
}

#endif