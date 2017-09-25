// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EXCEPTIONS_INIT_EXCEPTION_H
#define O2_EXCEPTIONS_INIT_EXCEPTION_H

#include "./AbstractException.h"

namespace O2{
    namespace Exceptions{
        class InitException : public AbstractException{
        public:
            InitException(const std::string& msg);
        };
    }
}

#endif