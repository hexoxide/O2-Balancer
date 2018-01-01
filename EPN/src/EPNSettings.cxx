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
#include "O2/EPN/EPNGlobals.h"

using O2::EPN::EPNSettings;


EPNSettings::EPNSettings(const boost::program_options::variables_map &settings) : Settings() {
    using O2::EPN::AMOUNT_OF_FLPS_SETTING;
    using O2::EPN::FLP_PORT_SETTING;
    YAML::Node config = this->load(settings);

    constexpr char FLP_CONNECTION_PORT_SETTING[] = "FLPConnectionPort";
    constexpr char OUTPUT_PORT_SETTING[] = "OutputPort";

    this->amountOfFLPs = settings[AMOUNT_OF_FLPS_SETTING].as<int>();
    this->flpConnectionPort = config[FLP_CONNECTION_PORT_SETTING].as<int>();
    this->outputConnectionPort = config[OUTPUT_PORT_SETTING].as<int>();
    if (settings[FLP_PORT_SETTING].as<int>() != 0) {
        this->flpConnectionPort = settings[FLP_PORT_SETTING].as<int>();
    }

    // YAML file variables
    constexpr char GOAT_NODE_NAME[] = "Goat";
    constexpr char GOAT_IP_SETTING[] = "IP";
    constexpr char GOAT_AMOUNT_AFTER_SETTING[] = "amount_after_signal";
    constexpr char GOAT_AMOUNT_BEFORE_SETTING[] = "amount_before_signal";
    constexpr char GOAT_HEARTBEAT_SETTING[] = "heartbeat";

    if (config[GOAT_NODE_NAME]) {
        this->goatIP = config[GOAT_NODE_NAME][GOAT_IP_SETTING].as<std::string>();
        this->amountAfterSignal = config[GOAT_NODE_NAME][GOAT_AMOUNT_AFTER_SETTING].as<int>();
        this->amountBeforeCrash = config[GOAT_NODE_NAME][GOAT_AMOUNT_BEFORE_SETTING].as<int>();
        this->heartrate = config[GOAT_NODE_NAME][GOAT_HEARTBEAT_SETTING].as<int>();
    }
}


std::string EPNSettings::getGoatIP() const {
    return goatIP;
}

int EPNSettings::getHeartrate() const {
    return heartrate;
}

int EPNSettings::getAmountAfterSignal() const {
    return amountAfterSignal;
}

int EPNSettings::getAmountBeforeCrash() const {
    return amountBeforeCrash;
}

std::string EPNSettings::getSettingsFile() const {
    return "epn-config";
}

int EPNSettings::FLPConnectionPort() const {
    return this->flpConnectionPort;
}

int EPNSettings::OutputConnectionPort() const {
    return this->outputConnectionPort;
}

int EPNSettings::getAmountOfFLPs() const {
    if (this->amountOfFLPs == 0) {
        throw Balancer::Exceptions::InitException(
                "There need to be at least one FLP defined"
        );
    }
    return this->amountOfFLPs;
}
