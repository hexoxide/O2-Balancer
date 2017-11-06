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
#include <vector>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include <FairMQLogger.h>
#include <FairMQProgOptions.h>
#include <O2/Balancer/Globals.h>
#include <O2/Balancer/Exceptions/AbstractException.h>
#include "O2/InformationNode/InformationDevice.h"
#include "O2/InformationNode/InfoSettings.h"

#include "O2/InformationNode/AcknowledgeConnection.h"

using namespace O2;
using namespace O2::InformationNode;

InformationDevice::InformationDevice(std::shared_ptr<InfoSettings> settings) :  Balancer::AbstractDevice(O2::Balancer::Globals::DeviceNames::INFORMATION_NAME, settings){
  this->timeFrameId = 0;
  this->heartbeat = settings->getHeartRate();
  this->useClusterManager([this, settings](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
      try{
        manager->addGlobalInteger("sampleSize", settings->getSampleSize());
      } catch (const O2::Balancer::Exceptions::AbstractException& exc){
          LOG(ERROR) << exc.getMessage(); 
      }
  });


  this->heartbeatConnection = std::unique_ptr<HeartbeatConnection>(new HeartbeatConnection(
    settings->getIPAddress(), settings->getHeartBeatPort(), this
  )); 

  this->acknowledgeConnection = std::unique_ptr<AcknowledgeConnection>(new AcknowledgeConnection(
    settings->getIPAddress(),settings->getAcknowledgePort(),this
  ));
}



InformationDevice::~InformationDevice()
= default;

void InformationDevice::InitTask(){

}

void InformationDevice::refreshDevice(){

}

void InformationDevice::preRun(){

    mLeaving = false;
    mAckListener = std::thread(&InformationDevice::ListenForAcknowledgement, this);


}

bool InformationDevice::conditionalRun(){
  FairMQMessagePtr msg(NewSimpleMessage(timeFrameId));
  
  if (fChannels.at(heartbeatConnection->getName()).at(0).Send(msg) >= 0) {

    mTimeframeRTT[timeFrameId].start = std::chrono::steady_clock::now();

    this->timeFrameId = (timeFrameId == UINT16_MAX - 1)? 0 : timeFrameId + 1;
  } else {
    LOG(ERROR) << "Could not send :(";
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(heartbeat));

  return true;
}

void InformationDevice::postRun(){
    LOG(INFO) << "Stopping";
    mLeaving = true;
    mAckListener.join();
}


void InformationDevice::ListenForAcknowledgement(){
  
  uint16_t id = 0;


  while (!mLeaving) {
    FairMQMessagePtr idMsg(NewMessage());

    if (Receive(idMsg, this->acknowledgeConnection->getName(), 0, 1000) >= 0) {
      //id = *(static_cast<uint16_t*>(idMsg->GetData()));
      std::string dat = std::string(static_cast<char*>(idMsg->GetData()), idMsg->GetSize());
      std::vector<std::string> x;
      boost::split(x, dat, boost::is_any_of("#"));
      id = std::atoi(x[1].c_str());
      mTimeframeRTT.at(id).end = std::chrono::steady_clock::now();
      // store values in a file
      auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(mTimeframeRTT.at(id).end - mTimeframeRTT.at(id).start);
      LOG(INFO) << "Timeframe #" << id << " received from " << x[0] << "  acknowledged after " << elapsed.count() << " μs.";
    }
  }
  
  LOG(INFO) << "Exiting Ack listener";
}