// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/AcknowledgeConnection.h"
#include <O2/Balancer/Devices/AbstractDevice.h>
#include "O2/EPN/EPNSettings.h"
#include "FairMQLogger.h"
using namespace O2::EPN;

AcknowledgeConnection::AcknowledgeConnection(Balancer::AbstractDevice* device, std::shared_ptr<EPNSettings> settings) : Balancer::Connection("ack", device){
    /*this->acknowledgeChannel = this->addChannel(
        Balancer::ConnectionType::Push,
        Balancer::ConnectionMethod::Connect,
        settings->getInformationNodeSetting()->ip,
        settings->getInformationNodeSetting()->port
    );*/

    auto dev = device->getClusterManager()->getRegisteredConnection("InformationNode", "ack");
    LOG(INFO) << "Connected with " << dev.ip << " " << dev.port;
    this->acknowledgeChannel = this->addChannel(
        Balancer::ConnectionType::Push,
        Balancer::ConnectionMethod::Connect,
        dev.ip,
        dev.port
    );
}

void AcknowledgeConnection::updateConnection(std::shared_ptr<Balancer::ClusterManager> clusterManager){
    auto dev = clusterManager->getRegisteredConnection("InformationNode", "ack");
    this->acknowledgeChannel.UpdateAddress(dev.ip);
    LOG(INFO) << "Connected with " << dev.ip << " " << dev.port;
}