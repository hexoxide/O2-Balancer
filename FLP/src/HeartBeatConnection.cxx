// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/HeartBeatConnection.h"
#include "O2/FLP/FLPDevice.h"

using namespace O2::FLP;

HeartbeatConnection::HeartbeatConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice *device)
        : Balancer::Connection("stf1", device) {
    this->addInputChannel(
            Balancer::ConnectionType::Subscribe,
            Balancer::ConnectionMethod::Connect,
            settings->getInformationNodeSetting()->ip,
            settings->getInformationNodeSetting()->port
    );

    this->updateAllRateLogging(1);
    this->updateAllReceiveBuffer(100000);
}
