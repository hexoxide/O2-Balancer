// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/EPNConnection.h"
#include "O2/FLP/FLPDevice.h"
#include "O2/FLP/FLPSettings.h"

using namespace O2::FLP;

EPNConnection::EPNConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice* device) : Balancer::Connection("stf2", device){
    
    
    for(const auto& epn : settings->getEPNSettings()){
        this->addInputChannel(
            Balancer::ConnectionType::Push,
            Balancer::ConnectionMethod::Connect,
            epn->ip,
            epn->port
        );
    }
    this->updateAllSendBuffer(100000);
    this->updateAllRateLogging(1);
    
}