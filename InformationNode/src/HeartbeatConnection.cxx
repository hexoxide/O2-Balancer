// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/HeartbeatConnection.h"

using namespace O2;

HeartbeatConnection::HeartbeatConnection(int port, AbstractDevice* device) : Connection("stf1",device){
    this->addChannel(
        ConnectionType::Publish,
        ConnectionMethod::Bind,
        "127.0.0.1",
        port
    );
}