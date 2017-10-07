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

EPNSettings::EPNSettings(const boost::program_options::variables_map& settings){
    YAML::Node config = YAML::LoadFile(settings["epn-config"].as<std::string>());
    this->ip = settings["ip"].as<std::string>();
    this->informationSettings = std::shared_ptr<Balancer::DeviceSetting>( new Balancer::DeviceSetting(
        config["InformationNode"]["Port"].as<int>(),
        config["InformationNode"]["Ip"].as<std::string>()
    ));
    this->amountOfFLPs = settings["amount-flps"].as<int>();
    this->flpConnectionPort = config["FLPConnectionPort"].as<int>();
    this->outputConnectionPort = config["OutputPort"].as<int>();

    if(settings["flp-port"].as<int>() != 0){
        this->flpConnectionPort = settings["flp-port"].as<int>();
    }
    


}

std::shared_ptr<Balancer::DeviceSetting> EPNSettings::getInformationNodeSetting() const{
    return this->informationSettings;
}

std::string EPNSettings::getIpAddress() const{
    return this->ip;
}

int EPNSettings::FLPConnectionPort() const{
    return this->flpConnectionPort;
}
int EPNSettings::OutputConnectionPort() const{
    return this->outputConnectionPort;    
}

int EPNSettings::getAmountOfFLPs() const{
    return this->amountOfFLPs;
}