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
#include <functional>
#include "../Remote/ClusterManager.h"

namespace O2 {
    namespace Balancer {

        class Connection;

        class Settings;

        class AbstractDevice : public FairMQDevice {
        private:
            friend Connection;
            std::thread zooThread;

            void checkZooKeeper();

            std::string defaultTransport;

            std::string getProperty(const std::string &varName, const std::string &defValue);

            bool restartOnUpdate;
            std::mutex zoolock;
            std::shared_ptr<ClusterManager> clusterManager;

            virtual bool ConditionalRun() override final;

            virtual void PreRun() override final;

            virtual void PostRun() override final;

            virtual void Run() override final;

        protected:
            std::shared_ptr<Settings> settings;
            std::atomic<bool> nRefresh;
            std::atomic<bool> nStop;

            virtual void preRun();

            virtual void postRun();

            virtual void run();

            virtual bool conditionalRun();

            virtual void refreshDevice(bool inMainThread) = 0;



        public:
            /**
             * Instantiates a device, setting all the FairMQ settings.
             * @param name The name to identify the device
             * @param settings The global settings used by each device
             * @param restartOnUpdate Does it need to restart when zookeeper has an update?
             */
            AbstractDevice(const std::string &name, std::shared_ptr<Settings> settings, bool restartOnUpdate = false);

            void useClusterManager(std::function<void(std::shared_ptr<ClusterManager>)> cl);
            /**
             * Restarts FairMQ, getting all the new paths from FairRoot and updates the channels.
             * Should only be called from the main loop!
             */
            void restartDevice();

            /**
             * If zookeeper has an update, this will be true. (when restartonupdate is true)
             * @return ZooKeeper has an update
             */
            bool needRefresh() const;


            bool needToStop() const;

            bool addHandle(const std::string &tag, const DeviceSetting &setting);

            std::string getDefaultTransport() const;

            void quit();
        };
    }
}

#endif