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
#include "O2/EPN/EPNSettings.h"
#include <O2/Balancer/DeviceManager.h>
#include <O2/Balancer/ProgramOptions.h>
#include <O2/Balancer/ClusterManager.h>

namespace po = boost::program_options;

int main(int argc, char** argv){
    using namespace O2;
    using namespace O2::EPN;
    
    po::options_description options("EPN options");
    constexpr char CONFIG_FILE[] = "epn-config";

    


    options.add_options()
    (CONFIG_FILE, po::value<std::string>()->default_value("./epn.yaml"), "Configuration file")
    ("amount-flps", po::value<int>()->default_value(2))
    ("flp-port", po::value<int>()->default_value(0), "Port that the FLPs can use to connect");
    auto vm = Balancer::AddO2Options(options, argc, argv);

    EPNSettings settings(vm);

  //  O2::Balancer::ClusterManager manager("localhost",2181);
   // manager.registerCluster("EPN");
    Balancer::DeviceManager<EPNDevice> device(settings);
    device.run();

   // manager.close();
    return EXIT_SUCCESS;
}

/*#include <runFairMQDevice.h>

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options){
    options.add_options()
        ("buffer-timeout", bpo::value<int>()->default_value(1000), "Buffer timeout in milliseconds")
        ("num-flps", bpo::value<int>()->required(), "Number of FLPs")
        ("test-mode", bpo::value<int>()->default_value(0), "Run in test mode")
        ("in-chan-name", bpo::value<std::string>()->default_value("stf2"), "Name of the input channel (sub-time frames)")
        ("out-chan-name", bpo::value<std::string>()->default_value("tf"), "Name of the output channel (time frames)")
        ("ack-chan-name", bpo::value<std::string>()->default_value("ack"), "Name of the acknowledgement channel");
}

FairMQDevicePtr getDevice(const FairMQProgOptions&){
    return new AliceO2::EPN::EPNDevice();
}
*/