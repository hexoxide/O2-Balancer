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
#include <O2/Balancer/Exceptions/UnimplementedException.h>
#include <O2/Balancer/Utilities/DataTypes.h>
#include <vector>
#include <string>


namespace O2 {
    namespace FLP {
        class FLPSettings;

        class DeviceSetting;

        /**
         *  The class that manages the connection between the EPN and the FLP.
         *  Also responsible of load balancing.
         *  @author H.J.M van der Heijden
         */
        class EPNConnection : public Balancer::Connection {
            std::vector<std::string> offlineEPNS;
        public:
            EPNConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice *device);

            /**
             * Get the amount of EPNs registered by FairMQ
             * @return Amount of epns
             */
            size_t amountOfEpns() const;

            /**
             * Method for spreading the sub time frames equally while sending it to the same EPN as other FLPs.
             * @param id The heartbeat required to calculate the round robin
             * @return Target index to send the STF to
             */
            size_t balance(O2::Balancer::heartbeatID id);

            /**
             * Updates the channels from FairMQ.
             * If called while FairMQ is running, behaviour is undefined.
             * @warning Don't call this while fairMQ is running.
             */
            void updateConnection();

            /**
             * Updates the blacklist from zookeeper.
             */
            void updateBlacklist();
        };
    } // namespace FLP
} // namespace O2

#endif // O2_FLP_EPN_CONNECTION_H
