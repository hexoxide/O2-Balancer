// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/FLPGlobals.h"
#include "O2/FLP/FLPDevice.h"
#include "logger/logger.h"

namespace po = boost::program_options;
using O2::Balancer::AddO2Options;
using O2::Balancer::DeviceManager;
using O2::Balancer::Exceptions::AbstractException;
using O2::FLP::FLPSettings;
using O2::FLP::FLPDevice;


int main(int argc, char **argv) {
    po::options_description options("FLP options");
    using O2::FLP::Options::RESTART_FAIR_ROOT_SETTING;

    constexpr char CONFIG_FILE[] = "flp-config";
    options.add_options()
          //  ("sample-size", po::value<int>()->default_value(1))
            (RESTART_FAIR_ROOT_SETTING, po::value<std::string>()->default_value(""))
            (CONFIG_FILE, po::value<std::string>()->default_value("./flp.yaml"), "Configuration file");

    try {
        auto vm = AddO2Options(options, argc, argv);

	fair::mq::logger::ReinitLogger(true, "FLP", fair::mq::logger::SeverityLevel::INFO);
        auto settings = std::shared_ptr<FLPSettings>(new FLPSettings(vm));
        try {
            DeviceManager<FLPDevice> manager(settings);
            manager.run();
        } catch (const AbstractException &exception) {
            LOG(ERROR) << exception.getMessage();
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    catch (po::error const& e) {
        LOG(ERROR) << e.what();
        return EXIT_FAILURE;
    }
}
