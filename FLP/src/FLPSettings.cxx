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
#include "FairMQLogger.h"

using namespace O2;
using namespace O2::FLP;

FLPSettings::FLPSettings(const boost::program_options::variables_map &settings) : Settings() {
    YAML::Node config = this->load(settings);
    if (config["restartFairRoot"]) {
        this->vRestartFairRoot = config["restartFairRoot"].as<bool>();
    } else {
        this->vRestartFairRoot = true;
    }

    if (settings["restartFairRoot"].as<std::string>() != "") {
        this->vRestartFairRoot = (settings["restartFairRoot"].as<std::string>() != "false");
    }

    if (config["sampleType"]) {
        const auto type = config["sampleType"].as<std::string>();
        if (type == "Sine") {
            LOG(INFO) << "Using sampling type of sine";
            this->sampleType = SampleType::Sine;
        } else if (type == "Random") {
            LOG(INFO) << "Using random sampling";
            this->sampleType = SampleType::Random;
        } else {
            LOG(INFO) << "Using flatline sampling";
            this->sampleType = SampleType::Equal;
        }

    }
}

SampleType FLPSettings::getSampleType() const {
    return this->sampleType;
}

bool FLPSettings::restartFairRoot() const {
    return vRestartFairRoot;
}

std::string FLPSettings::getSettingsFile() const {
    return "flp-config";
}
