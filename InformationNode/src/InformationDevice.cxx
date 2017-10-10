// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
* InformationDevice.cxx
* @author H.J.M van der Heijden
* @since 25-08-2017
*/
#include <fstream>
#include <ctime>

#include <FairMQLogger.h>
#include <FairMQProgOptions.h>
#include "O2/InformationNode/InformationDevice.h"
#include "O2/InformationNode/AcknowledgeConnection.h"
#include "O2/InformationNode/HeartbeatConnection.h"


using namespace O2;
using namespace O2::InformationNode;

InformationDevice::InformationDevice(std::string ip, int heartbeat, int acknowledgePort, int heartbeatPort) : Balancer::AbstractDevice("Information"){
  this->timeFrameId = 0;
  this->heartbeat = heartbeat;
  this->addConnection(HeartbeatConnection(ip,heartbeatPort, this));
  this->addConnection(AcknowledgeConnection(ip,acknowledgePort,this));
}



InformationDevice::~InformationDevice()
= default;

void InformationDevice::InitTask(){
  mAckChannelName = "ack";
  mOutChannelName = "stf1";

}

void InformationDevice::PreRun(){
    AbstractDevice::PreRun(); 
    mLeaving = false;
    mAckListener = std::thread(&InformationDevice::ListenForAcknowledgement, this);
    this->clusterManager->addGlobalVariable("sampleSize", "10");
}

bool InformationDevice::ConditionalRun(){
  FairMQMessagePtr msg(NewSimpleMessage(timeFrameId));

  if (fChannels.at(mOutChannelName).at(0).Send(msg) >= 0) {
    mTimeframeRTT[timeFrameId].start = std::chrono::steady_clock::now();

    this->timeFrameId = (timeFrameId == UINT16_MAX - 1)? 0 : timeFrameId + 1;
  } else {
    LOG(ERROR) << "Could not send :(";
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(heartbeat));

  return true;
}

void InformationDevice::PostRun(){

    mLeaving = true;
    mAckListener.join();
    AbstractDevice::PostRun();
}

void InformationDevice::ListenForAcknowledgement(){
  
  uint16_t id = 0;


  while (!mLeaving) {
    FairMQMessagePtr idMsg(NewMessage());

    if (Receive(idMsg, mAckChannelName, 0, 1000) >= 0) {
      id = *(static_cast<uint16_t*>(idMsg->GetData()));
      mTimeframeRTT.at(id).end = std::chrono::steady_clock::now();
      // store values in a file
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(mTimeframeRTT.at(id).end - mTimeframeRTT.at(id).start);

      

      LOG(INFO) << "Timeframe #" << id << " acknowledged after " << elapsed.count() << " μs.";
    }
  }

  LOG(INFO) << "Exiting Ack listener";
}