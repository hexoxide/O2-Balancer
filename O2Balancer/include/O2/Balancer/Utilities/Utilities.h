// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_UTILITIES_H
#define O2_BALANCER_UTILITIES_H

#include "../Exceptions/InitException.h"
#include <boost/program_options.hpp>
#include <string>
namespace O2{

    namespace Balancer{
        /**
         * Forks the program and runs it as a background program
         * This also closes stdin and stdout.
         * */
        void daemonize();



        /**
         * Adds the start up variables that are similair to all the derived programs from the O2 Balancer.
         * Such as versioning, daemonizing and ip addresses.
         * @param options   The custom options required for functioning
         * @param int argc  The argument count that is passed from the main function
         * @param char** argv  The argument array that is passed from the main function
         * @return the custom options. 
         * */
        boost::program_options::variables_map AddO2Options(boost::program_options::options_description& options, int argc, char** argv);
    
        /**
         *  Checks recursively if a variable is within an list.
         * */
        inline void variableChecksOut(std::string var, std::string tail){
            if(var != tail){
                throw O2::Balancer::Exceptions::InitException(var + std::string(" is not an allowed value"));

            }
        }
        template<typename... Arguments>
        inline void variableChecksOut(std::string var, std::string head, Arguments... tail){
            if(var == head){
                return;
            } else {
                variableChecksOut(var, tail...);
            }
        }


        /**
         * Crash after amount of heartrates.
         * This crashes the program when the amount of heartrates occurs.
         * 
         * */
         void crashAfterAmountOfBeats(int heartrate, int amount, bool terminate);
    }
}

#endif