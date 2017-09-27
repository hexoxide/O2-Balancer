#include "O2/FLP/FLPSettings.h"
#include <yaml-cpp/yaml.h>
using namespace O2;
using namespace O2::FLP;

FLPSettings::FLPSettings(const std::string fileName){
    YAML::Node config = YAML::LoadFile(fileName);

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


std::shared_ptr<Balancer::DeviceSetting> FLPSettings::getInformationNodeSetting() const{
    return this->informationSettings;
}

std::vector<std::shared_ptr<Balancer::DeviceSetting>> FLPSettings::getEPNSettings() const{
    return this->epnSettings;
}