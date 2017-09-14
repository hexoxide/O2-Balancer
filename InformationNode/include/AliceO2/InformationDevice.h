#ifndef ALICEO2_INFORMATION_DEVICE_H
#define ALICEO2_INFORMATION_DEVICE_H

#include <string>
#include <cstdint> // UINT64_MAX

#include <thread>
#include <atomic>
#include <chrono>

#include <FairMQDevice.h>

namespace AliceO2{
/// Stores measurment for roundtrip time of a timeframe

struct timeframeDuration{
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};

/// Publishes timeframes IDs for flpSenders (used only in test mode)

class InformationDevice : public FairMQDevice{
  public:
    /// Default constructor
    InformationDevice();

    /// Default destructor
    ~InformationDevice() override;

    /// Controls the send rate of the timeframe IDs
    void ResetEventCounter();

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
    int mEventRate; ///< Publishing rate of the timeframe IDs
    int mMaxEvents; ///< Maximum number of events to send (0 - unlimited)
    int mStoreRTTinFile; ///< Store round trip time measurements in a file.
    int mEventCounter; ///< Controls the send rate of the timeframe IDs
    uint16_t mTimeFrameId;
    std::thread mAckListener;
    std::thread mResetEventCounter;
    std::atomic<bool> mLeaving;

    std::string mAckChannelName;
    std::string mOutChannelName;
};

} 


#endif