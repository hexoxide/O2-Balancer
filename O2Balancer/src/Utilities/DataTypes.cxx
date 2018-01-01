// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/Balancer/Utilities/DataTypes.h"
#include <vector>
#include "O2/Balancer/Exceptions/InitException.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"
#include <boost/algorithm/string.hpp>

using O2::Balancer::DeviceSetting;

DeviceSetting::DeviceSetting(const int port,
                             const std::string &ip) {
    if(ip.empty()){
        throw O2::Balancer::Exceptions::UnimplementedException(
                "No device exists with an empty IP..."
        );
    }
    this->port = port;
    this->ip = ip;
}

DeviceSetting::DeviceSetting(const std::string &setting) {
    std::vector<std::string> x;
    boost::split(x, setting, boost::is_any_of(":"));
    if(x.size() != 2){
        throw O2::Balancer::Exceptions::InitException("Could not parse string");
    }
    this->ip = x[0];
    this->port = std::stoi(x[1]);

}

