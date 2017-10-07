#include "O2/Balancer/Utilities/Utilities.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "O2/Balancer/Balancer.h"
using namespace O2;


namespace po = boost::program_options;

po::variables_map O2::Balancer::AddO2Options(boost::program_options::options_description& options, int argc, char** argv){
    options.add_options()
    ("version", "Shows O2 library version")
    ("daemonize", "sets a daemon")
    ("ip", po::value<std::string>()->required())
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

void Balancer::daemonize(){
    pid_t pid,sid;
    //Fork the program
    pid = fork();
    if(pid < 0){
        std::exit(EXIT_FAILURE);
    }

    if(pid > 0){
        std::exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if(sid < 0){
        std::exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}