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
#include <atomic>
#include <mutex>
#include <thread>

#include "../Remote/ClusterManager.h"

namespace O2{   
    namespace Balancer{
             
        class Connection;

        class Settings;

        class AbstractDevice : public FairMQDevice {
        private:
            friend Connection;
            std::thread zooThread;
            void checkZooKeeper();
            std::string defaultTransport;
            std::string getProperty(const std::string& varName, const std::string& defValue); 
        protected:
            std::mutex zoolock;
            std::shared_ptr<Settings> settings;
            std::shared_ptr<ClusterManager> clusterManager;
            std::vector<std::shared_ptr<Connection>> connnections;
            std::atomic<bool> nRefresh;
            std::atomic<bool> nStop;
        public:
            AbstractDevice(const std::string& name, std::shared_ptr<Settings> settings);
            std::shared_ptr<ClusterManager> getClusterManager() const;
            bool needRefresh() const;
            virtual void refreshDevice() = 0;
            bool needToStop() const;
            bool addHandle(const std::string& tag, const DeviceSetting& setting);
            std::string getDefaultTransport() const;
            void quit();
        };
    }
}

#endif