#include "AliceO2/FLPDevice.h"
#include <cstdint> // UINT64_MAX
#include <cassert>
#include <chrono>
#include <utility>
#include <FairMQLogger.h>
#include <FairMQMessage.h>
#include <FairMQProgOptions.h>
#include <iostream>
#include <ctime>
#include <cstring>
//#include <options/FairMQProgOptions.h>



struct f2eHeader {
  uint16_t timeFrameId;
  int      flpIndex;
};

using namespace AliceO2::FLP;

FLPDevice::FLPDevice(){

}

        
void FLPDevice::InitTask(){
    this->mIndex = GetConfig()->GetValue<int>("flp-index");
    this->mEventSize = GetConfig()->GetValue<int>("event-size");
    this->mNumEPNs = GetConfig()->GetValue<int>("num-epns");
    this->mTestMode = GetConfig()->GetValue<int>("test-mode");
    this->mSendOffset = GetConfig()->GetValue<int>("send-offset");
    this->mSendDelay = GetConfig()->GetValue<int>("send-delay");
    this->mInChannelName = GetConfig()->GetValue<std::string>("in-chan-name");
    this->mOutChannelName = GetConfig()->GetValue<std::string>("out-chan-name");
    LOG(INFO) << this->mOutChannelName;
}



bool FLPDevice::ConditionalRun(){
 // base buffer, to be copied from for every timeframe body (zero-copy)
  FairMQChannel& dataInChannel = fChannels.at(mInChannelName).at(0);
  std::fstream fstream("/dev/null",  std::ifstream::binary | std::ios::in);
  //while (CheckCurrentState(RUNNING)) {
    if(fstream.good()){
      FairMQParts parts;
      const int size = this->mEventSize * 1024 * 1024;
      //constexpr int size = 90;
      char* buffer = new char[size];  
      fstream.read(buffer,size);

      parts.AddPart(NewMessage());
      parts.AddPart(NewMessage(
        buffer,
        size,
        [](void* /*data*/, void* object) {delete[] static_cast<char*> (object); },
        buffer
      ));
      if (dataInChannel.Receive(parts.At(0)) >= 0) {
        uint16_t currentTimeFrameid = *(static_cast<uint16_t*>(parts.At(0)->GetData()));
        LOG(INFO) << "Current id " << currentTimeFrameid;
        int direction = currentTimeFrameid % mNumEPNs;
        if (Send(parts, mOutChannelName, direction, 0) < 0) {
           LOG(ERROR) << "booom";
        }
      } else {
        // if nothing was received, try again
       // continue;
      }
     

    }


    //Send(baseMsg, "test");
 
 // }
  fstream.close();

  return true;


   
  
/*
 FairMQMessagePtr baseMsg(NewMessage(mEventSize));
   // store the channel reference to avoid traversing the map on every loop iteration
   FairMQChannel& dataInChannel = fChannels.at(mInChannelName).at(0);
 
   while (CheckCurrentState(RUNNING)) {
     // initialize f2e header
     auto* header = new f2eHeader;
     if (mTestMode > 0) {
       // test-mode: receive and store id part in the buffer.
       FairMQMessagePtr id(NewMessage());
       if (dataInChannel.Receive(id) > 0) {
         header->timeFrameId = *(static_cast<uint16_t*>(id->GetData()));
         header->flpIndex = mIndex;
       } else {
         // if nothing was received, try again
         delete header;
         continue;
       }
     } else {
       // regular mode: use the id generated locally
       header->timeFrameId = mTimeFrameId;
       header->flpIndex = mIndex;
 
       if (++mTimeFrameId == UINT16_MAX - 1) {
         mTimeFrameId = 0;
       }
     }
 
     FairMQParts parts;
     parts.AddPart(NewMessage(header, sizeof(f2eHeader), [](void* data, void* hint){ delete static_cast<f2eHeader*>(hint); }, header));
     parts.AddPart(NewMessage());

     mArrivalTime.push(std::chrono::steady_clock::now());
 
     if (mTestMode > 0) {
       // test-mode: initialize and store data part in the buffer.
       parts.At(1)->Copy(baseMsg);
       mSTFBuffer.push(std::move(parts));
     } else {
       // regular mode: receive data part from input
       if (dataInChannel.Receive(parts.At(1)) >= 0) {
         mSTFBuffer.push(std::move(parts));
       } else {
         // if nothing was received, try again
         continue;
       }
     }
 
     // if offset is 0 - send data out without staggering.
     if (mSendOffset == 0 && mSTFBuffer.size() > 0) {
       sendFrontData();
     } else if (mSTFBuffer.size() > 0) {
       if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mArrivalTime.front()).count() >= (mSendDelay * mSendOffset)) {
         sendFrontData();
       } else {
         // LOG(INFO) << "buffering...";
       }
     }
 }
 */
}


inline void FLPDevice::sendFrontData()
{
  /*
  f2eHeader header = *(static_cast<f2eHeader*>(mSTFBuffer.front().At(0)->GetData()));
  uint16_t currentTimeframeId = header.timeFrameId;

  // for which EPN is the message?
  int direction = currentTimeframeId % mNumEPNs;
  // LOG(INFO) << "Sending event " << currentTimeframeId << " to EPN#" << direction << "...";

  if (Send(mSTFBuffer.front(), mOutChannelName, direction, 0) < 0) {
    LOG(ERROR) << "Failed to queue sub-timeframe #" << currentTimeframeId << " to EPN[" << direction << "]";
  }
  mSTFBuffer.pop();
  mArrivalTime.pop();*/
}


FLPDevice::~FLPDevice() = default;