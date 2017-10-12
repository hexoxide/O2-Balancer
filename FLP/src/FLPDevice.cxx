// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/FLPDevice.h"
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
#include "O2/FLP/HeartBeatConnection.h"
#include "O2/FLP/EPNConnection.h"
#include <O2/Balancer/Globals.h>
#include <O2/Balancer/Exceptions/ClusterHandlerException.h>


struct f2eHeader {
  uint16_t timeFrameId;
  int      flpIndex;
};

using namespace O2::FLP;

FLPDevice::FLPDevice(std::shared_ptr<FLPSettings> settings) : Balancer::AbstractDevice(O2::Balancer::Globals::DeviceNames::FLP_NAME, settings){
  this->mNumEPNs = settings->getEPNSettings().size();
  this->addConnection(HeartbeatConnection(settings, this));
  this->addConnection(EPNConnection(settings,this));
  this->mEventSize = settings->getSampleSize();


  //while(  this->clusterManager->addGlobalVariable("sampleSize", "10");)

}

        

void FLPDevice::PreRun(){
    AbstractDevice::PreRun();
    try{
      this->mEventSize = this->clusterManager->getGlobalInteger("sampleSize", 1000);

    } catch (const O2::Balancer::Exceptions::ClusterHandlerException& ex){
        LOG(ERROR) << ex.getMessage();
    }
}


bool FLPDevice::ConditionalRun(){

  
  FairMQChannel& dataInChannel = fChannels.at("stf1").at(0);
  std::fstream fstream("/dev/null",  std::ifstream::binary | std::ios::in);

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

}