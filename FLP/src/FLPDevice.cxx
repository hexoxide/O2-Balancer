#include "O2/FLPDevice.h"
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
#include "O2/HeartBeatConnection.h"
#include "O2/EPNConnection.h"




struct f2eHeader {
  uint16_t timeFrameId;
  int      flpIndex;
};

using namespace O2::FLP;

FLPDevice::FLPDevice(const FLPSettings& settings) : AbstractDevice("flpSender1"){

  
  
  this->results = std::unique_ptr<O2::ResultManager>(new O2::ResultManager("flp.csv"));
  this->mNumEPNs = settings.getEPNSettings().size();
  this->addConnection(HeartbeatConnection(settings, this));
  this->addConnection(EPNConnection(settings,this));
}

        
void FLPDevice::InitTask(){
    this->mEventSize = 1;

}



bool FLPDevice::ConditionalRun(){


  FairMQChannel& dataInChannel = fChannels.at("stf1").at(0);
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
        this->results->addTimeFrame(currentTimeFrameid);
        LOG(INFO) << "Current id " << currentTimeFrameid;
        int direction = currentTimeFrameid % mNumEPNs;
        LOG(INFO) << "Direction" << direction;
        if (Send(parts, "stf2", direction, 0) < 0) {
           LOG(ERROR) << "Could not send to EPN " << direction;
        }

      }
  }
  fstream.close();
  return true;

}


FLPDevice::~FLPDevice(){
  LOG(INFO) << "closing";
  this->results.reset();
}