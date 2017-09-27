// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/EPNSettings.h"
#include <yaml-cpp/yaml.h>

using namespace O2;
using namespace O2::EPN;

EPNSettings::EPNSettings(const std::string& configFile){
    YAML::Node config = YAML::LoadFile(configFile);
    
    this->informationSettings = std::shared_ptr<Balancer::DeviceSetting>( new Balancer::DeviceSetting(
        config["InformationNode"]["Port"].as<int>(),
        config["InformationNode"]["Ip"].as<std::string>()
    ));
}

std::shared_ptr<Balancer::DeviceSetting> EPNSettings::getInformationNodeSetting() const{
    return this->informationSettings;
}



