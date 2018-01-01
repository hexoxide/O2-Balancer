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
#include <yaml-cpp/yaml.h>
#include "FairMQLogger.h"
#include "O2/FLP/FLPGlobals.h"

using O2::FLP::FLPSettings;
using O2::FLP::SampleType;

FLPSettings::FLPSettings(
        const boost::program_options::variables_map &settings) : Settings() {

    using O2::FLP::Options::RESTART_FAIR_ROOT_SETTING;
    using O2::FLP::Options::SAMPLE_TYPE_SETTING;
    YAML::Node config = this->load(settings);
    if (config[RESTART_FAIR_ROOT_SETTING]) {
        this->vRestartFairRoot = config[RESTART_FAIR_ROOT_SETTING].as<bool>();
    } else {
        this->vRestartFairRoot = true;
    }

    if (settings[RESTART_FAIR_ROOT_SETTING].as<std::string>() != "") {
        this->vRestartFairRoot = (settings[RESTART_FAIR_ROOT_SETTING].as<std::string>() != "false");
    }

    if (config[SAMPLE_TYPE_SETTING]) {
        const auto type = config[SAMPLE_TYPE_SETTING].as<std::string>();
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
