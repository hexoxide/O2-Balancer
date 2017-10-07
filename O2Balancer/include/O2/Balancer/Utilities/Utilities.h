#ifndef O2_BALANCER_UTILITIES_H
#define O2_BALANCER_UTILITIES_H

#include "../Exceptions/InitException.h"
#include <boost/program_options.hpp>
#include <string>
namespace O2{
    namespace Balancer{
        /**
         *  Forks the program and runs it as a background program
         *  @author H.J.M van der Heijden
         * */
        void daemonize();

        boost::program_options::variables_map AddO2Options(boost::program_options::options_description&, int argc, char** argv);
    
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
    }
}

#endif