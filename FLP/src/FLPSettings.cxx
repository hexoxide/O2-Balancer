// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/FLPSettings.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
using namespace O2;
using namespace O2::FLP;

FLPSettings::FLPSettings(const boost::program_options::variables_map& settings){
    YAML::Node config = YAML::LoadFile(settings["flp-config"].as<std::string>());
    this->sampleSize = settings["sample-size"].as<int>();
    
    this->informationSettings = std::shared_ptr<Balancer::DeviceSetting>( new Balancer::DeviceSetting(
        config["InformationNode"]["Port"].as<int>(),
        config["InformationNode"]["Ip"].as<std::string>()
    ));

    for(size_t i = 0; i < config["EPNs"].size(); i++){
        this->epnSettings.push_back(
            std::shared_ptr<Balancer::DeviceSetting>(
                new Balancer::DeviceSetting(
                    config["EPNs"][i]["Port"].as<int>(),
                    config["EPNs"][i]["Ip"].as<std::string>()
                )
            )  
        );
    }

}

int FLPSettings::getSampleSize() const{
    return this->sampleSize;
}

std::shared_ptr<Balancer::DeviceSetting> FLPSettings::getInformationNodeSetting() const{
    return this->informationSettings;
}

std::vector<std::shared_ptr<Balancer::DeviceSetting>> FLPSettings::getEPNSettings() const{
    return this->epnSettings;
}