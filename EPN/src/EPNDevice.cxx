#include "AliceO2/EPNDevice.h"
#include<memory>
#include <fstream>
#include <FairMQProgOptions.h>
using namespace AliceO2::EPN;

struct f2eHeader {
    uint16_t timeFrameId;
    int      flpIndex;
};

EPNDevice::EPNDevice(){

}

        
void EPNDevice::InitTask(){
    mNumFLPs = GetConfig()->GetValue<int>("num-flps");
    mBufferTimeoutInMs = GetConfig()->GetValue<int>("buffer-timeout");
    mTestMode = GetConfig()->GetValue<int>("test-mode");
    mInChannelName = GetConfig()->GetValue<std::string>("in-chan-name");
    mOutChannelName = GetConfig()->GetValue<std::string>("out-chan-name");
    mAckChannelName = GetConfig()->GetValue<std::string>("ack-chan-name");
}



void EPNDevice::PrintBuffer(const std::unordered_map<uint16_t, TFBuffer>& buffer) const
{
  std::string header = "===== ";

  for (int i = 1; i <= mNumFLPs; ++i) {
    std::stringstream out;
    out << i % 10;
    header += out.str();
    //i > 9 ? header += " " : header += "  ";
  }
  LOG(INFO) << header;

  for (auto& it : buffer) {
    std::string stars = "";
    for (unsigned int j = 1; j <= (it.second).parts.Size(); ++j) {
      stars += "*";
    }
    LOG(INFO) << std::setw(4) << it.first << ": " << stars;
  }
}

void EPNDevice::DiscardIncompleteTimeframes(){
  auto it = mTimeframeBuffer.begin();

  while (it != mTimeframeBuffer.end()) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - (it->second).start).count() > mBufferTimeoutInMs) {
      LOG(WARN) << "Timeframe #" << it->first << " incomplete after " << mBufferTimeoutInMs << " milliseconds, discarding";
      mDiscardedSet.insert(it->first);
      mTimeframeBuffer.erase(it++);
      LOG(WARN) << "Number of discarded timeframes: " << mDiscardedSet.size();
    } else {
      // LOG(INFO) << "Timeframe #" << it->first << " within timeout, buffering...";
      ++it;
    }
  }
}

void EPNDevice::Run(){
  // DEBUG: store receive intervals per FLP
  // vector<vector<int>> rcvIntervals(fNumFLPs, vector<int>());
  // vector<std::chrono::steady_clock::time_point> rcvTimestamp(fNumFLPs);
  // end DEBUG

  // f2eHeader* header; // holds the header of the currently arrived message.
  uint16_t id = 0; // holds the timeframe id of the currently arrived sub-timeframe.

  FairMQChannel& ackOutChannel = fChannels.at(mAckChannelName).at(0);

  while (CheckCurrentState(RUNNING)) {
    FairMQParts parts;

    if (Receive(parts, mInChannelName, 0, 100) > 0) {
      // store the received ID
      f2eHeader& header = *(static_cast<f2eHeader*>(parts.At(0)->GetData()));
      id = header.timeFrameId;
      // LOG(INFO) << "Received sub-time frame #" << id << " from FLP" << header.flpIndex;

      // DEBUG:: store receive intervals per FLP
      // if (fTestMode > 0) {
      //   int flpId = header.flpIndex;
      //   rcvIntervals.at(flpId).push_back(duration_cast<milliseconds>(steady_clock::now() - rcvTimestamp.at(flpId)).count());
      //   LOG(WARN) << rcvIntervals.at(flpId).back();
      //   rcvTimestamp.at(flpId) = steady_clock::now();
      // }
      // end DEBUG

      if (mDiscardedSet.find(id) == mDiscardedSet.end())
      {
        if (mTimeframeBuffer.find(id) == mTimeframeBuffer.end())
        {
          // if this is the first part with this ID, save the receive time.
          mTimeframeBuffer[id].start = std::chrono::steady_clock::now();
        }
        // if the received ID has not previously been discarded,
        // store the data part in the buffer
        mTimeframeBuffer[id].parts.AddPart(move(parts.At(1)));
        // PrintBuffer(fTimeframeBuffer);
      }
      else
      {
        // if received ID has been previously discarded.
        LOG(WARN) << "Received part from an already discarded timeframe with id " << id;
      }

      if (mTimeframeBuffer[id].parts.Size() == mNumFLPs) {
        if (mTestMode > 0) {
          // Send an acknowledgement back to the sampler to measure the round trip time
          std::unique_ptr<FairMQMessage> ack(NewMessage(sizeof(uint16_t)));
          memcpy(ack->GetData(), &id, sizeof(uint16_t));

          if (ackOutChannel.Send(ack, 0) <= 0) {
            LOG(ERROR) << "Could not send acknowledgement without blocking";
          }
        }
        else
        {
          // LOG(INFO) << "Collected all parts for timeframe #" << id;
          // when all parts are collected send them to the output channel
          Send(mTimeframeBuffer[id].parts, mOutChannelName);
        }

        // fTimeframeBuffer[id].end = steady_clock::now();

        mTimeframeBuffer.erase(id);
      }

      // LOG(WARN) << "Buffer size: " << fTimeframeBuffer.size();
    }

    // check if any incomplete timeframes in the buffer are older than timeout period, and discard them if they are
    DiscardIncompleteTimeframes();
  }

  
}

EPNDevice::~EPNDevice() = default;