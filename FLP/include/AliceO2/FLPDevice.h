#ifndef ALICEO2_FLP_DEVICE_H
#define ALICEO2_FLP_DEVICE_H

#include <FairMQDevice.h>
#include <queue>
#include <string>

namespace AliceO2{
    namespace FLP{
        class FLPDevice : public FairMQDevice{
          public:
            FLPDevice();
            virtual ~FLPDevice();
        
          protected:
            virtual void InitTask() override;
            virtual bool ConditionalRun() override;

          private:

            std::string* createSTF(const uint16_t& id) const;
            int balanceEPN(const FairMQMessagePtr& stf);
            
            void sendFrontData();

            std::queue<FairMQParts> mSTFBuffer; ///< Buffer for sub-timeframes
            std::queue<std::chrono::steady_clock::time_point> mArrivalTime; ///< Stores arrival times of sub-timeframes
        
            int mNumEPNs; ///< Number of epnReceivers
            unsigned int mIndex; ///< Index of the flpSender among other flpSenders
            unsigned int mSendOffset; ///< Offset for staggering output
            unsigned int mSendDelay; ///< Delay for staggering output
        
            int mEventSize; ///< Size of the sub-timeframe body (only for test mode)
            int mTestMode; ///< Run the device in test mode (only syncSampler+flpSender+epnReceiver)
            uint16_t mTimeFrameId;
        
            std::string mInChannelName;
            std::string mOutChannelName;
        };
    }
}

#endif