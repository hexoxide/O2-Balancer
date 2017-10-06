#include "O2/Balancer/ProgramOptions.h"
#include <cstdlib>
#include <iostream>
#include "O2/Balancer/Balancer.h"
#include "O2/Balancer/Utilities.h"
namespace po = boost::program_options;

po::variables_map O2::Balancer::AddO2Options(boost::program_options::options_description& options, int argc, char** argv){
    options.add_options()
    ("version", "Shows O2 library version")
    ("daemonize", "sets a daemon")
    ("help", "Produces help message");


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);   
    

    if(vm.count("version")){
        std::cout << "O2 version: " << O2::Balancer::VERSION_MAJOR << "." <<  O2::Balancer::VERSION_MINOR << "." << O2::Balancer::VERSION_PATCH << "\n" ;
        std::exit(EXIT_SUCCESS);
    }

    if(vm.count("daemonize")){
        daemonize();
    }

    return vm;
}