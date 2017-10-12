// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#ifndef O2_EPN_EPNACKNOWLEDGE_CONNECTION_H
#define O2_EPN_EPNACKNOWLEDGE_CONNECTION_H

#include <O2/Balancer/Devices/Connection.h>
#include <O2/Balancer/Remote/ClusterManager.h>
namespace O2{
    namespace EPN{

        class EPNSettings;
        class AcknowledgeConnection : public Balancer::Connection{
            FairMQChannel acknowledgeChannel;
        public:
            AcknowledgeConnection(Balancer::AbstractDevice* device,std::shared_ptr<EPNSettings> settings);
            void updateConnection(std::shared_ptr<Balancer::ClusterManager> clusterManager) override;
        };
    }
}

#endif