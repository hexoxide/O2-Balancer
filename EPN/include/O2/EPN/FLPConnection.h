// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EPN_FLPCONNECTION_H
#define O2_EPN_FLPCONNECTION_H

#include <O2/Balancer/Devices/Connection.h>

namespace O2 {
    namespace EPN {

        class EPNSettings;

        /**
         * Manages the connection between the FLP.
         * @author H.J.M van der Heijden
         */
        class FLPConnection : public Balancer::Connection {
        public:
            FLPConnection(Balancer::AbstractDevice *device,
                          std::shared_ptr<EPNSettings> settings);
        };
    }
}

#endif // O2_EPN_FLPCONNECTION_H
