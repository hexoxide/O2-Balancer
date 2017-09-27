#ifndef O2_FLP_DEVICE_H
#define O2_FLP_DEVICE_H

#include <O2/AbstractDevice.h>
#include <queue>
#include <string>
#include <memory>
#include <O2/ResultManager.h>
#include "./FLPSettings.h"
namespace O2{
    namespace FLP{
        class FLPDevice : public AbstractDevice{
          public:
            FLPDevice(const FLPSettings& settings);
            virtual ~FLPDevice();
        
          protected:
            virtual void InitTask() override;
            virtual bool ConditionalRun() override;

          private:
            std::unique_ptr<ResultManager> results;
            std::queue<FairMQParts> mSTFBuffer; ///< Buffer for sub-timeframes
            std::queue<std::chrono::steady_clock::time_point> mArrivalTime; ///< Stores arrival times of sub-timeframes
        
            int mNumEPNs; ///< Number of epnReceivers

            int mEventSize; ///< Size of the sub-timeframe body (only for test mode)
        
            uint16_t mTimeFrameId;

        };
    }
}

#endif