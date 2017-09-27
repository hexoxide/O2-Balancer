// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_INFORMATIONNODE_INFORMATION_DEVICE_H
#define O2INFORMATIONNODE_INFORMATION_DEVICE_H

#include <string>
#include <cstdint> // UINT64_MAX

#include <thread>
#include <atomic>
#include <chrono>
#include <O2/Balancer/AbstractDevice.h>

namespace O2{

  namespace InformationNode{

    struct timeframeDuration{
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::time_point end;
    };
    
    /// Publishes timeframes IDs for flpSenders (used only in test mode)
    
    class InformationDevice : public Balancer::AbstractDevice{
      public:
        /// Default constructor
        InformationDevice(int heartbeat, int acknowledgePort, int heartbeatPort);
    
        /// Default destructor
        ~InformationDevice() override;
    
    
        /// Listens for acknowledgements from the epnReceivers when they collected full timeframe
        void ListenForAcknowledgement();
    
      protected:
        /// Overloads the InitTask() method of FairMQDevice
        void InitTask() override;
    
        /// Overloads the Run() method of FairMQDevice
        bool ConditionalRun() override;
        void PreRun() override;
        void PostRun() override;
    
        std::array<timeframeDuration, UINT16_MAX> mTimeframeRTT; ///< Container for the roundtrip values per timeframe ID
        int mMaxEvents; ///< Maximum number of events to send (0 - unlimited)
        int mStoreRTTinFile; ///< Store round trip time measurements in a file.
        int mEventCounter; ///< Controls the send rate of the timeframe IDs
        int heartbeat;
        uint16_t mTimeFrameId;
        std::thread mAckListener;
        std::atomic<bool> mLeaving;
    
        std::string mAckChannelName;
        std::string mOutChannelName;
    };
  }


} 


#endif