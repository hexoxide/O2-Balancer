// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EXCEPTIONS_UNIMPLEMENTED_EXCEPTION_H
#define O2_EXCEPTIONS_UNIMPLEMENTED_EXCEPTION_H

#include "./AbstractException.h"

namespace O2{

    namespace Balancer{
        namespace Exceptions{
            /**
             * An exception that should be thrown when the program is meeting a condition that defaults to undefined behaviour.
             * For example when a switch defaults to an unsupported operation.
             * Ideally, this exception should never be thrown.
             * @author H.J.M van der Heijden
             * @date 10 October 2017
             * */
            class UnimplementedException : public AbstractException{
            public:
                UnimplementedException(const std::string& msg);
            };
        }
    }

}

#endif