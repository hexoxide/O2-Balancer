// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/FLPDevice.h"
#include <O2/Balancer/DeviceManager.h>
#include <O2/Balancer/ProgramOptions.h>
#include "O2/FLP/FLPSettings.h"
#include <O2/Balancer/ClusterManager.h>
namespace po = boost::program_options;


int main(int argc, char** argv){
    using namespace O2;
    using namespace O2::FLP;
    po::options_description options("FLP options");
    constexpr char CONFIG_FILE[] = "flp-config";
    options.add_options()
    ("sample-size",  po::value<int>()->default_value(1))
    (CONFIG_FILE, po::value<std::string>()->default_value("./flp.yaml"), "Configuration file");
    
    auto vm = Balancer::AddO2Options(options, argc, argv);


    //O2::Balancer::ClusterManager manager("localhost",2181);
    //manager.registerCluster("FLP");
    
// vm[CONFIG_FILE].as<std::string>()
    //Load the settings
    FLPSettings settings(vm);
    try{
        Balancer::DeviceManager<FLPDevice> manager(
            settings
        );
        manager.run();    
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << exception.getMessage();
       // manager.close();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}

