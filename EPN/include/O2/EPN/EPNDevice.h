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

#include <O2/Balancer/Devices/AbstractDevice.h>
#include "./EPNSettings.h"
#include <unordered_map>
#include <string>
#include <chrono>
#include <unordered_set>

namespace O2{
    namespace EPN{
        class AcknowledgeConnection;
        class FLPConnection;
        class OutputConnection;

        class EPNDevice : public Balancer::AbstractDevice{
            std::unique_ptr<AcknowledgeConnection> acknowledgeConnection;
            std::unique_ptr<FLPConnection> flpConnection;
            std::unique_ptr<OutputConnection> outputConnection;
        protected:
            struct TFBuffer
            {
                FairMQParts parts;
                std::chrono::steady_clock::time_point start;
                std::chrono::steady_clock::time_point end;
            };
            
            public:
            EPNDevice(std::shared_ptr<EPNSettings> settings);
            virtual ~EPNDevice();
            
            /// Discared incomplete timeframes after \p fBufferTimeoutInMs.
            void DiscardIncompleteTimeframes();
          protected:
            virtual void InitTask() override;
            void Run() override;
    
            std::unordered_map<uint16_t, TFBuffer> mTimeframeBuffer; ///< Stores (sub-)timeframes
            std::unordered_set<uint16_t> mDiscardedSet; ///< Set containing IDs of dropped timeframes
    
            int mNumFLPs; ///< Number of flpSenders
            int mBufferTimeoutInMs; ///< Time after which incomplete timeframes are dropped
            int mTestMode; ///< Run the device in test mode (only syncSampler+flpSender+epnReceiver)
    
            std::string mInChannelName;
            std::string mOutChannelName;
            std::string mAckChannelName;
        };
    }
}

#endif