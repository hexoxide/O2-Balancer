#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Utilities/DeviceSetting.h"
#include "O2/Balancer/Exceptions/InitException.h"
#include "FairVersion.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

using namespace O2::Balancer;

Settings::Settings(){

}

YAML::Node Settings::load(const boost::program_options::variables_map& settings){
    try{
        YAML::Node config = YAML::LoadFile(settings[getSettingsFile()].as<std::string>());
        this->ipAddress = settings["ip"].as<std::string>();
        this->settingsServer = std::shared_ptr<Balancer::DeviceSetting>(
            new Balancer::DeviceSetting(
                config["SettingsServer"]["Port"].as<int>(),
                config["SettingsServer"]["IP"].as<std::string>()
            )
        );

        this->informationSettings = std::shared_ptr<Balancer::DeviceSetting>( new Balancer::DeviceSetting(
            config["InformationNode"]["Port"].as<int>(),
            config["InformationNode"]["Ip"].as<std::string>()
        ));
        return config;
    } catch(const YAML::BadFile& badFile){
        throw Exceptions::InitException("Could not load settings file!");
    } catch(const YAML::TypedBadConversion<int>& badfile){
        throw Exceptions::InitException("Could not parse settings file");
    }
}

std::shared_ptr<DeviceSetting> Settings::getSettingsServer() const{
    if(this->settingsServer == nullptr){
        throw Exceptions::InitException("Settings server was not initialized");
    }
    return this->settingsServer;
}

std::shared_ptr<DeviceSetting> Settings::getInformationNodeSetting() const{
    if(this->informationSettings == nullptr){
        throw Exceptions::InitException("Information node was not initialized");
    }
    return this->informationSettings;
}

std::string Settings::getIPAddress() const{
    if(this->ipAddress == ""){
        if(this->informationSettings == nullptr){
            throw Exceptions::InitException("Ip address was not initialized");
        }
    }
    return this->ipAddress;
}