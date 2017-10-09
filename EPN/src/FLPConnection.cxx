// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/FLPConnection.h"
#include "O2/EPN/EPNSettings.h"

using namespace O2::EPN;

FLPConnection::FLPConnection(Balancer::AbstractDevice* device, const EPNSettings& settings) : Balancer::Connection("stf2", device){

    this->addChannel(
        Balancer::ConnectionType::Pull,
        Balancer::ConnectionMethod::Bind,
        settings.getIPAddress(),
        settings.FLPConnectionPort()
    );

    this->updateAllReceiveBuffer(100000);

}