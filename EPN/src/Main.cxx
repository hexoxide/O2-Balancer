// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/EPNDevice.h"
#include "O2/EPN/EPNGlobals.h"

using O2::Balancer::AddO2Options;
using O2::EPN::EPNSettings;
using O2::EPN::EPNDevice;
using O2::Balancer::DeviceManager;
using O2::Balancer::Exceptions::AbstractException;
namespace po = boost::program_options;

int main(int argc, char **argv) {
    po::options_description options("EPN options");

    using O2::EPN::AMOUNT_OF_FLPS_SETTING;
    using O2::EPN::CONFIG_FILE_SETTING;
    using O2::EPN::CONFIG_FILE_DEFAULT;
    using O2::EPN::AMOUNT_OF_FLPS_DEFAULT;
    using O2::EPN::FLP_PORT_SETTING;

    try {
        options.add_options()
                (CONFIG_FILE_SETTING, po::value<std::string>()->default_value(CONFIG_FILE_DEFAULT), "Configuration file")
                (AMOUNT_OF_FLPS_SETTING, po::value<int>()->default_value(AMOUNT_OF_FLPS_DEFAULT))
                (FLP_PORT_SETTING, po::value<int>()->default_value(0), "Port that the FLPs can use to connect");
        auto vm = AddO2Options(options, argc, argv);

        auto settings = std::shared_ptr<EPNSettings>(new EPNSettings(vm));
        reinit_logger(true, "EPN", SEVERITY_MINIMUM);
        DeviceManager<EPNDevice> device(settings);
        device.run();
    } catch (const AbstractException &ex) {
        LOG(ERROR) << "Could not start due exception " << ex.getMessage();
        return EXIT_FAILURE;
    } catch (po::error const& e) {
        LOG(ERROR) << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
