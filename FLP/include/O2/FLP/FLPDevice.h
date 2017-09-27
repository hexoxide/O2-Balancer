// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_FLP_DEVICE_H
#define O2_FLP_DEVICE_H

#include <O2/Balancer/AbstractDevice.h>
#include <queue>
#include <string>
#include <memory>
#include <O2/Balancer/ResultManager.h>
#include "./FLPSettings.h"

namespace O2{
    namespace FLP{
        class FLPDevice : public Balancer::AbstractDevice{
          public:
            FLPDevice(const FLPSettings& settings);
            virtual ~FLPDevice();
        
          protected:
            virtual void InitTask() override;
            virtual bool ConditionalRun() override;

          private:
            std::unique_ptr<Balancer::ResultManager> results;
            std::queue<FairMQParts> mSTFBuffer; ///< Buffer for sub-timeframes
            std::queue<std::chrono::steady_clock::time_point> mArrivalTime; ///< Stores arrival times of sub-timeframes
        
            int mNumEPNs; ///< Number of epnReceivers

            int mEventSize; ///< Size of the sub-timeframe body (only for test mode)
        
            uint16_t mTimeFrameId;

        };
    }
}

#endif