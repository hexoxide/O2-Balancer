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
#include <O2/Balancer/Exceptions/InitException.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

using namespace O2;
using namespace O2::EPN;

EPNSettings::EPNSettings(const boost::program_options::variables_map& settings) : Settings(){
    YAML::Node config = this->load(settings);
    this->amountOfFLPs = settings["amount-flps"].as<int>();
    this->flpConnectionPort = config["FLPConnectionPort"].as<int>();
    this->outputConnectionPort = config["OutputPort"].as<int>();
    if(settings["flp-port"].as<int>() != 0){
        this->flpConnectionPort = settings["flp-port"].as<int>();
    }
    


}

std::string EPNSettings::getSettingsFile() const{
    return "epn-config";
}

int EPNSettings::FLPConnectionPort() const{
    
    return this->flpConnectionPort;
}
int EPNSettings::OutputConnectionPort() const{
    return this->outputConnectionPort;    
}

int EPNSettings::getAmountOfFLPs() const{
    if(this->amountOfFLPs == 0){
        throw Balancer::Exceptions::InitException("There need to be at least one FLP defined");
    }
    return this->amountOfFLPs;
}