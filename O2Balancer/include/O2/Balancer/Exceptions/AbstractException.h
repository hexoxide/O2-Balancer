// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_EXCEPTIONS_ABSTRACT_EXCEPTION_H
#define O2_BALANCER_EXCEPTIONS_ABSTRACT_EXCEPTION_H

#include <string>
namespace O2{

    namespace Balancer{
        namespace Exceptions{
            /**
             * Base class for all exceptions used in the O2 Balancer library and derived programs.
             * Although not abstract, it's preferably not directly throw this exception. Instead use exceptions derived from this.
             * @author H.J.M van der Heijden
             * @date 10 October 2017
             * */
            class AbstractException : public std::exception{
                std::string msg;
            public:
                /**
                 * Constructor with message.
                 * */
                AbstractException(const std::string& msg);
                /**
                 * Gets the error message that caused the crash.
                 * */
                std::string getMessage() const;
                
            };
        }
    }
    
}

#endif