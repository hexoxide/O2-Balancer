// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EPN_EPNDEVICE_H
#define O2_EPN_EPNDEVICE_H
#include <boost/format.hpp>
#include <future>
#include <queue>
#include <unordered_map>
#include <string>
#include <chrono>
#include <unordered_set>
#include <O2/Balancer/Devices/AbstractDevice.h>
#include <O2/Balancer/Globals.h>
#include <O2/Balancer/Utilities/Utilities.h>
#include <O2/Balancer/Devices/DeviceManager.h>
#include <FairMQProgOptions.h>
#include "./EPNSettings.h"



namespace O2 {
    namespace EPN {
        class AcknowledgeConnection;

        class FLPConnection;

        class OutputConnection;

        /**
         * Central class that manages all the connections of the epn.
         * Contains the central logic of the EPN.
         * @author H.J.M van der Heijden
         */
        class EPNDevice : public Balancer::AbstractDevice {
            std::unique_ptr<AcknowledgeConnection> acknowledgeConnection;
            std::unique_ptr<FLPConnection> flpConnection;
            std::unique_ptr<OutputConnection> outputConnection;
        protected:
            struct TFBuffer {
                FairMQParts parts;
                std::chrono::steady_clock::time_point start;
                std::chrono::steady_clock::time_point end;
            };

        public:
            explicit EPNDevice(std::shared_ptr<EPNSettings> settings);

            ~EPNDevice() override;

            /// Discared incomplete timeframes after \p fBufferTimeoutInMs.
            void DiscardIncompleteTimeframes();

        protected:
            void run() override;

            void refreshDevice(bool inMainThread) override;

            std::unordered_map<O2::Balancer::heartbeatID, TFBuffer> mTimeframeBuffer; ///< Stores (sub-)timeframes
            std::unordered_set<O2::Balancer::heartbeatID> mDiscardedSet; ///< Set containing IDs of dropped timeframes

            int mNumFLPs; ///< Number of flpSenders
            int mBufferTimeoutInMs; ///< Time after which incomplete timeframes are dropped
        };
    } // namespace EPN
} // namespace O2

#endif