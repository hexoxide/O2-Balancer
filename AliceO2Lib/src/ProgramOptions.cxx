#include "O2/ProgramOptions.h"
#include <cstdlib>
#include <iostream>
#include "O2/O2.h"

namespace po = boost::program_options;

po::variables_map O2::AddO2Options(boost::program_options::options_description& options, int argc, char** argv){
    options.add_options()
    ("version", "Shows O2 library version")
    ("help", "Produces help message");


    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, options), vm);
    po::notify(vm);   
    
    if(vm.count("version")){
        std::cout << "O2 version: " << O2::VERSION_MAJOR << "." <<  O2::VERSION_MINOR << "." << O2::VERSION_PATCH << "\n" ;
        std::exit(EXIT_SUCCESS);
    }

    return vm;
}