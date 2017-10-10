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
#include <O2/Balancer/Exceptions/InitException.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

using namespace O2;
using namespace O2::FLP;

FLPSettings::FLPSettings(const boost::program_options::variables_map& settings) : Settings(){
    YAML::Node config = this->load(settings);
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

    this->sampleSize = settings["sample-size"].as<int>();

}

std::string FLPSettings::getSettingsFile() const{
    return "flp-config";
}

int FLPSettings::getSampleSize() const{
    return this->sampleSize;
}


std::vector<std::shared_ptr<Balancer::DeviceSetting>> FLPSettings::getEPNSettings() const{
    if(this->epnSettings.size() == 0){
        throw Balancer::Exceptions::InitException("No EPNs are defined!");
    }
    return this->epnSettings;
}

