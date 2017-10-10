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
#include <O2/Balancer/Exceptions/InitException.h>
#include <O2/Balancer/Remote/ClusterManager.h>
std::unique_ptr< O2::Balancer::DeviceManager<O2::InformationNode::InformationDevice>> deviceManager;

namespace po = boost::program_options;

int main(int argc, char** argv){
    constexpr char ACKNOWLEDGE_PORT[] = "acknowledge-port";
    constexpr char HEARTBEAT_RATE[] = "heartbeat";
    constexpr char HEARTBEAT_PORT[] = "heartbeat-port";
    O2::Balancer::ClusterManager clManager("localhost", 4000);
    clManager.close();
    try{
        reinit_logger(true, "Information", SEVERITY_MINIMUM); 
        po::options_description options("Information node options");
        options.add_options()
        (HEARTBEAT_RATE, po::value<int>()->default_value(1000000), "Heartbeat frequency")
        (ACKNOWLEDGE_PORT, po::value<int>()->default_value(5990), "Port that listens for acknowledge")
        (HEARTBEAT_PORT, po::value<int>()->default_value(5550), "Port that publishes the heartbeat");
        auto vm = O2::Balancer::AddO2Options(options, argc, argv);
        deviceManager = std::unique_ptr<O2::Balancer::DeviceManager<O2::InformationNode::InformationDevice>>(
            new O2::Balancer::DeviceManager<O2::InformationNode::InformationDevice>(
                vm["ip"].as<std::string>(),
                vm[HEARTBEAT_RATE].as<int>(),
                vm[ACKNOWLEDGE_PORT].as<int>(),
                vm[HEARTBEAT_PORT].as<int>()
            )
        );

      
        deviceManager->run();
        
    } catch(O2::Balancer::Exceptions::InitException exception){
        LOG(ERROR) << "Failed to initialize due, error :" << exception.getMessage();
      //  manager.close();
        return EXIT_FAILURE;
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << exception.getMessage();
    //    manager.close();
        return EXIT_FAILURE;
    } catch(std::exception ex){
        LOG(ERROR) << "Unkown exception occured";
        return EXIT_FAILURE;
    }
  //  manager.close();
    return EXIT_SUCCESS;
}
