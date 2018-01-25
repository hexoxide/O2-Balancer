// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/Balancer/Utilities/Utilities.h"
#include <boost/format.hpp>
#include <sys/stat.h>
#include <FairVersion.h>
#include "FairMQLogger.h"
#include <zookeeper/zookeeper_version.h>
#include <yaml-cpp/yaml.h>
#include "O2/Balancer/Balancer.h"
#include <thread>
#include <sstream>
#include <iostream>
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

std::string getCompilerInformation(){
    std::stringstream dat;
#ifdef __GNUC__
    dat << "g++ "<< __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;

#elif __clang__
    dat << "Clang++ " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
#else
    dat << "Unknown";
#endif
    return dat.str();
}


namespace po = boost::program_options;

po::variables_map O2::Balancer::AddO2Options(boost::program_options::options_description& options, int argc, char** argv) {
    try {
        options.add_options()
        ("version", "Shows O2 library version")
        ("daemonize", "sets a daemon")
        ("ip", po::value<std::string>()->required());



        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, options), vm);
     
        
        if(vm.count("version")) {
            std::cout << "O2 Balancer prototype version: " << O2::Balancer::VERSION_MAJOR << "." <<  O2::Balancer::VERSION_MINOR << "." << O2::Balancer::VERSION_PATCH << "\n" ;
            std::cout << "Using FairRoot version : " << TOSTRING(FAIRROOT_VERSION) << "\n";
            std::cout << "Using ZooKeeper version : " << ZOO_MAJOR_VERSION << "." << ZOO_MINOR_VERSION << "." << ZOO_PATCH_VERSION << "\n";

            std::cout << "Using Boost : "
            << BOOST_VERSION / 100000     << "."  // major version
            << BOOST_VERSION / 100 % 1000 << "."  // minor version
            << BOOST_VERSION % 100                // patch level
            << std::endl;
            std::cout << "Compiled on : " << __DATE__ << "\n";
            std::cout << "Compiled by : " << getCompilerInformation() << "\n";
            std::exit(EXIT_SUCCESS);
        }
        po::notify(vm);   

        if(vm.count("daemonize")) {
            daemonize();
        }

        return vm;
    } catch(const boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::required_option> >& exc) {
        throw O2::Balancer::Exceptions::InitException(exc.what());
    }

}

void O2::Balancer::daemonize() {
    pid_t pid,sid;
    //Fork the program
    pid = fork();
    if(pid < 0) {
        std::exit(EXIT_FAILURE);
    }

    if(pid > 0) {
        std::exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if(sid < 0) {
        std::exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}



void O2::Balancer::crashAfterAmountOfBeats(int heartrate,
                                       int amount,
                                       bool terminate) {
    LOG(ERROR) << boost::format("Initializing crash in %i ms") % (heartrate * amount);
    static std::thread crashThread;
    crashThread = std::thread([](int rate, int amo, bool terminate) -> void {
        int current = 0;
        for(;;) {
            if(current >= amo) {

                if(terminate) {
                    LOG(ERROR) << "Crashing application with a terminate";
                    std::terminate();
                } else {
                    LOG(ERROR) << "Crashing application with an abort";
                    std::abort();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rate));
            current++;

        }

    }, heartrate, amount, terminate);
}

