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
#include "O2/EPN/EPNGlobals.h"

//using namespace O2::EPN;
using O2::EPN::AcknowledgeConnection;
using O2::EPN::EPNSettings;
using O2::Balancer::AbstractDevice;
using O2::EPN::DEVICE_NAME;
using O2::EPN::ACK_CHANNEL_STR;


AcknowledgeConnection::AcknowledgeConnection(AbstractDevice *device,
                                             std::shared_ptr<EPNSettings>) : Connection(ACK_CHANNEL_STR, device) {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        auto dev = manager->getRegisteredConnections(DEVICE_NAME, ACK_CHANNEL_STR);
        while (dev.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            dev = manager->getRegisteredConnections(DEVICE_NAME, ACK_CHANNEL_STR);
        }
        this->acknowledgeChannel = this->addInputChannel(
                Balancer::ConnectionType::Push,
                Balancer::ConnectionMethod::Connect,
                dev[0].ip,
                dev[0].port);
    });
    this->updateAllSendBuffer(10000);
}
