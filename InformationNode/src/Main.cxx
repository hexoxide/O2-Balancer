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
#include "O2/InformationDevice.h"
#include <O2/DeviceManager.h>
#include <O2/ProgramOptions.h>

namespace po = boost::program_options;

int main(int argc, char** argv){
    constexpr char ACKNOWLEDGE_PORT[] = "acknowledge-port";
    constexpr char HEARTBEAT_RATE[] = "heartbeat";
    constexpr char HEARTBEAT_PORT[] = "heartbeat-port";

    po::options_description options("Information node options");
    options.add_options()
    (HEARTBEAT_RATE, po::value<int>()->default_value(100), "Heartbeat frequency")
    (ACKNOWLEDGE_PORT, po::value<int>()->default_value(5990), "Port that listens for acknowledge")
    (HEARTBEAT_PORT, po::value<int>()->default_value(5550), "Port that publishes the heartbeat");
    auto vm = O2::AddO2Options(options, argc, argv);
    
   
    try{
        O2::DeviceManager<O2::InformationDevice> manager(
            vm[HEARTBEAT_RATE].as<int>(),
            vm[ACKNOWLEDGE_PORT].as<int>(),
            vm[HEARTBEAT_PORT].as<int>()
        );
        manager.run();
    } catch(O2::Exceptions::InitException exception){
        LOG(ERROR) << "Failed to initialize due, error :" << exception.getMessage();
        return EXIT_FAILURE;
    } catch (O2::Exceptions::AbstractException exception){
        LOG(ERROR) << exception.getMessage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
