#include "O2/FLPSettings.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
using namespace O2::FLP;

DeviceSetting::DeviceSetting(int port, std::string ip){
    this->port = port;
    this->ip = ip;
}

FLPSettings::FLPSettings(const std::string fileName){
    YAML::Node config = YAML::LoadFile(fileName);

    this->informationSettings = std::shared_ptr<DeviceSetting>( new DeviceSetting(
        config["InformationNode"]["Port"].as<int>(),
        config["InformationNode"]["Ip"].as<std::string>()
    ));

    for(size_t i = 0; i < config["EPNs"].size(); i++){
        this->epnSettings.push_back(
            std::shared_ptr<DeviceSetting>(
                new DeviceSetting(
                    config["EPNs"][i]["Port"].as<int>(),
                    config["EPNs"][i]["Ip"].as<std::string>()
                )
            )  
        );
    }

}


std::shared_ptr<DeviceSetting> FLPSettings::getInformationNodeSetting() const{
    return this->informationSettings;
}

std::vector<std::shared_ptr<DeviceSetting>> FLPSettings::getEPNSettings() const{
    return this->epnSettings;
}