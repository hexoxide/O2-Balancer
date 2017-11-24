// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Utilities/DataTypes.h"
#include "O2/Balancer/Exceptions/InitException.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

using namespace O2::Balancer;

Settings::Settings() {

}

YAML::Node Settings::load(const boost::program_options::variables_map &settings) {
    try {
        YAML::Node config = YAML::LoadFile(settings[getSettingsFile()].as<std::string>());
        this->ipAddress = settings["ip"].as<std::string>();
        this->settingsServer = std::shared_ptr<Balancer::DeviceSetting>(
                new Balancer::DeviceSetting(
                        config["SettingsServer"]["Port"].as<int>(),
                        config["SettingsServer"]["IP"].as<std::string>()
                )
        );

        this->informationSettings = std::shared_ptr<Balancer::DeviceSetting>(new Balancer::DeviceSetting(
                config["InformationNode"]["Port"].as<int>(),
                config["InformationNode"]["Ip"].as<std::string>()
        ));
        return config;
    } catch (const YAML::BadFile &badFile) {
        throw Exceptions::InitException("Could not load settings file!");
    } catch (const YAML::TypedBadConversion<int> &badfile) {
        throw Exceptions::InitException("Could not parse settings file");
    }
}

std::shared_ptr<DeviceSetting> Settings::getSettingsServer() const {
    if (this->settingsServer == nullptr) {
        throw Exceptions::InitException("Settings server was not initialized");
    }
    return this->settingsServer;
}

std::shared_ptr<DeviceSetting> Settings::getInformationNodeSetting() const {
    if (this->informationSettings == nullptr) {
        throw Exceptions::InitException("Information node was not initialized");
    }
    return this->informationSettings;
}

std::string Settings::getIPAddress() const {
    if (this->ipAddress.empty()) {
        if (this->informationSettings == nullptr) {
            throw Exceptions::InitException("Ip address was not initialized");
        }
    }
    return this->ipAddress;
}