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

using namespace O2::EPN;

OutputConnection::OutputConnection(Balancer::AbstractDevice* device) : Balancer::Connection("tf", device){
    this->addChannel(
        Balancer::ConnectionType::Publish,
        Balancer::ConnectionMethod::Bind,
        "127.0.0.1",
        5591
    );
}