// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
* Main.cxx
* @author H.J.M van der Heijden
* @since 25-08-2017
*/
#include "O2/InformationNode/InformationDevice.h"
#include <O2/Balancer/Devices/DeviceManager.h>
#include <O2/Balancer/Utilities/Utilities.h>
#include "O2/InformationNode/InfoSettings.h"

std::unique_ptr< O2::Balancer::DeviceManager<O2::InformationNode::InformationDevice>> deviceManager;

namespace po = boost::program_options;

int main(int argc, char** argv){
    using namespace O2::InformationNode;
    using namespace O2::Balancer;

    try{
        reinit_logger(true, "Information", SEVERITY_MINIMUM); 
        po::options_description options("Information node options");
        options.add_options()
        ("sample-size", po::value<int>()->default_value(-1), "Sample size of all the flp")
        ("info-config", po::value<std::string>()->default_value("./information.yaml"), "Configuration file");
        auto vm = AddO2Options(options, argc, argv);
        
        auto settings = std::shared_ptr<InfoSettings>(new InfoSettings(vm));
        
        deviceManager = std::unique_ptr<DeviceManager<InformationDevice>>(
            new DeviceManager<InformationDevice>(
                settings
            )
        );

      

        deviceManager->run();
        
    } catch(const O2::Balancer::Exceptions::InitException& exception){
        LOG(ERROR) << "Failed to initialize due, error :" << exception.getMessage();

        return EXIT_FAILURE;
    } catch (const O2::Balancer::Exceptions::AbstractException& exception){
        LOG(ERROR) << exception.getMessage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
