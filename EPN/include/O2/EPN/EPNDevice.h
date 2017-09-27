#ifndef O2_EPN_EPNDEVICE_H
#define O2_EPN_EPNDEVICE_H

#include <O2/Balancer/AbstractDevice.h>
#include <unordered_map>
#include <string>
#include <chrono>
#include <unordered_set>

namespace O2{
    namespace EPN{
        class EPNDevice : public Balancer::AbstractDevice{
        protected:
            struct TFBuffer
            {
                FairMQParts parts;
                std::chrono::steady_clock::time_point start;
                std::chrono::steady_clock::time_point end;
            };
            
            public:
            EPNDevice();
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