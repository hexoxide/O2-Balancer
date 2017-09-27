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



