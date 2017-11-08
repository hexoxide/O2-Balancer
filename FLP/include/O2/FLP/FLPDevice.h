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

#include <O2/Balancer/Devices/AbstractDevice.h>
#include <O2/Balancer/Remote/ClusterManager.h>
#include <queue>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include "./FLPSettings.h"

namespace O2{
    namespace FLP{

      class EPNConnection;
      class HeartbeatConnection;

        class FLPDevice : public Balancer::AbstractDevice{
          public:
            FLPDevice(std::shared_ptr<FLPSettings> settings);
            virtual ~FLPDevice();
        
          protected:
            void refreshDevice(bool inMainThread) override;
            virtual bool conditionalRun() override;
            virtual void ResetTask() override;
            virtual void preRun() override;
            virtual void Pause() override;
            virtual void postRun() override;
          private:
            std::vector<std::string> offlineEPNS;
            std::unique_ptr<HeartbeatConnection> heartBeatConnection;
            std::unique_ptr<EPNConnection> epnConnection;

            std::queue<FairMQParts> mSTFBuffer; ///< Buffer for sub-timeframes
            std::queue<std::chrono::steady_clock::time_point> mArrivalTime; ///< Stores arrival times of sub-timeframes
        
            int mNumEPNs; ///< Number of epnReceivers

            int mEventSize; ///< Size of the sub-timeframe body (only for test mode)
        
            uint16_t mTimeFrameId;

        };
    }
}

#endif