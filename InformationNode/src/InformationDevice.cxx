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

InformationDevice::InformationDevice(int heartbeat, int acknowledgePort, int heartbeatPort) : Balancer::AbstractDevice("Information"){
  
  this->heartbeat = heartbeat;
  this->addConnection(HeartbeatConnection(heartbeatPort, this));
  this->addConnection(AcknowledgeConnection(acknowledgePort,this));

}

InformationDevice::~InformationDevice()
= default;

void InformationDevice::InitTask(){
  mMaxEvents = 100;//GetConfig()->GetValue<int>("max-events");
  mStoreRTTinFile = 100;//GetConfig()->GetValue<int>("store-rtt-in-file");
  mAckChannelName = "ack";//GetConfig()->GetValue<std::string>("ack-chan-name");
  mOutChannelName = "stf1";//GetConfig()->GetValue<std::string>("out-chan-name");
}

void InformationDevice::PreRun(){
    mLeaving = false;
    mAckListener = std::thread(&InformationDevice::ListenForAcknowledgement, this);
    
  }

bool InformationDevice::ConditionalRun(){
  FairMQMessagePtr msg(NewSimpleMessage(mTimeFrameId));

  if (fChannels.at(mOutChannelName).at(0).Send(msg) >= 0) {
    mTimeframeRTT[mTimeFrameId].start = std::chrono::steady_clock::now();

    if (++mTimeFrameId == UINT16_MAX - 1) {
      mTimeFrameId = 0;
    }
  } else {
    LOG(ERROR) << "Could not send :(";
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(heartbeat));

  if (mMaxEvents > 0 && mTimeFrameId >= mMaxEvents) {
    LOG(INFO) << "Reached configured maximum number of events (" << mMaxEvents << "). Exiting Run().";
    return false;
  }

  return true;
}

void InformationDevice::PostRun(){

    mLeaving = true;
    mAckListener.join();
}

void InformationDevice::ListenForAcknowledgement(){
  
   uint16_t id = 0;

  std::ofstream ofsFrames;
  std::ofstream ofsTimes;

  // store round trip time measurements in a file
  if (mStoreRTTinFile > 0) {
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm utc = *gmtime(&t);
    std::stringstream s;
    s << utc.tm_year + 1900 << "-" << utc.tm_mon + 1 << "-" << utc.tm_mday << "-" << utc.tm_hour << "-" << utc.tm_min << "-" << utc.tm_sec;
    std::string name = s.str();
    ofsFrames.open(name + "-frames.log");
    ofsTimes.open(name + "-times.log");
  }

  while (!mLeaving) {
    FairMQMessagePtr idMsg(NewMessage());

    if (Receive(idMsg, mAckChannelName, 0, 1000) >= 0) {
      id = *(static_cast<uint16_t*>(idMsg->GetData()));
      mTimeframeRTT.at(id).end = std::chrono::steady_clock::now();
      // store values in a file
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(mTimeframeRTT.at(id).end - mTimeframeRTT.at(id).start);

      if (mStoreRTTinFile > 0) {
        ofsFrames << id << "\n";
        ofsTimes  << elapsed.count() << "\n";
      }

      LOG(INFO) << "Timeframe #" << id << " acknowledged after " << elapsed.count() << " μs.";
    }
  }

  // store round trip time measurements in a file
  if (mStoreRTTinFile > 0) {
    ofsFrames.close();
    ofsTimes.close();
  }
  LOG(INFO) << "Exiting Ack listener";
}
