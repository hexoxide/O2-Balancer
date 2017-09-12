#ifndef ALICEO2_EPN_DEVICE_H
#define ALICEO2_EPN_DEVICE_H

#include <FairMQDevice.h>
#include <unordered_map>
#include <string>
#include <chrono>
#include <unordered_set>

namespace AliceO2{
    namespace EPN{

        

        class EPNDevice : public FairMQDevice{
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
            
            /// Prints the contents of the timeframe container
            void PrintBuffer(const std::unordered_map<uint16_t, TFBuffer> &buffer) const;
    
            /// Discared incomplete timeframes after \p fBufferTimeoutInMs.
            void DiscardIncompleteTimeframes();
          protected:
            virtual void InitTask() override;
            /// Overloads the Run() method of FairMQDevice
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