#include "O2/Balancer/Utilities/Utilities.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <FairVersion.h>
#include <zookeeper/zookeeper_version.h>
#include <iostream>
#include "O2/Balancer/Balancer.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

using namespace O2;


namespace po = boost::program_options;

po::variables_map O2::Balancer::AddO2Options(boost::program_options::options_description& options, int argc, char** argv){
    try{
        options.add_options()
        ("version", "Shows O2 library version")
        ("daemonize", "sets a daemon")
        ("ip", po::value<std::string>()->required());



        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, options), vm);
     
        
        if(vm.count("version")){
            std::cout << "O2 Balancer prototype version: " << O2::Balancer::VERSION_MAJOR << "." <<  O2::Balancer::VERSION_MINOR << "." << O2::Balancer::VERSION_PATCH << "\n" ;
            std::cout << "Using FairRoot version : " << TOSTRING(FAIRROOT_VERSION) << "\n";
            std::cout << "Using ZooKeeper version : " << ZOO_MAJOR_VERSION << "." << ZOO_MINOR_VERSION << "." << ZOO_PATCH_VERSION << "\n";
            std::cout << "Using Boost : "     
            << BOOST_VERSION / 100000     << "."  // major version
            << BOOST_VERSION / 100 % 1000 << "."  // minor version
            << BOOST_VERSION % 100                // patch level
            << std::endl;
            std::cout << "Compiled on : " << __DATE__ << "\n";
            std::exit(EXIT_SUCCESS);
        }
        po::notify(vm);   

        if(vm.count("daemonize")){
            daemonize();
        }

        return vm;
    } catch(const boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::required_option> >& exc){
        throw O2::Balancer::Exceptions::InitException(exc.what());
    }

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