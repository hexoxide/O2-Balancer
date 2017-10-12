// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/OutputConnection.h"
#include "O2/EPN/EPNSettings.h"
using namespace O2::EPN;

OutputConnection::OutputConnection(Balancer::AbstractDevice* device, std::shared_ptr<EPNSettings> settings) : Balancer::Connection("tf", device){
    this->addChannel(
        Balancer::ConnectionType::Publish,
        Balancer::ConnectionMethod::Bind,
        settings->getIPAddress(),
        settings->OutputConnectionPort()
    );
    this->updateAllSendBuffer(10000);
}