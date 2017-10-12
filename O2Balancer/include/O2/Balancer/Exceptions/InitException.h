// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_EXCEPTIONS_INIT_EXCEPTION_H
#define O2_BALANCER_EXCEPTIONS_INIT_EXCEPTION_H

#include "./AbstractException.h"

namespace O2{

    namespace Balancer{
        namespace Exceptions{
            /**
             * The exception that halted the initialization of the library. 
             * When a setting is invalid, this exception should be thrown.
             * @author H.J.M van der Heijden
             * @date 10 October 2017
             * */
            class InitException : public AbstractException{
            public:
                InitException(const std::string& msg);
            };
        }
    }

}

#endif