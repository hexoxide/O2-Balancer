// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_DEVICES_ABSTRACT_DEVICE_H
#define O2_BALANCER_DEVICES_ABSTRACT_DEVICE_H

#include <FairMQDevice.h>
#include <vector>
#include <memory>
#include <string>
#include "../Remote/ClusterManager.h"

namespace O2{   
    namespace Balancer{     
        class Connection;
        //class ClusterManager;
        class AbstractDevice : public FairMQDevice {
        private:
          
            //this->clusterManager = std::unique_ptr<Balancer::ClusterManager>(new Balancer::ClusterManager(ip,2181));
            std::string defaultTransport;
            std::string getProperty(const std::string& varName, const std::string& defValue); 
        protected:
            std::unique_ptr<ClusterManager> clusterManager;
            std::vector<Connection> connnections;
            virtual void PreRun() override;
            virtual void PostRun() override;

        public:
            AbstractDevice(const std::string& name);
            void addConnection(Connection connection);
            std::string getDefaultTransport() const;
        };
    }
}

#endif