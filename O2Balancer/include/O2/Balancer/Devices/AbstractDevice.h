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

#include "O2/Balancer/Remote/ClusterManager.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Globals.h"
#include "O2/Balancer/Utilities/DataTypes.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"

#include <FairMQDevice.h>
#include <vector>
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>

namespace O2 {
    namespace Balancer {

        class Connection;

        class Settings;

        /**
         * A wrapper around FairMQDevice.
         * Making sure that Zookeeper can be used and exceptions specific to O2 are handled.
         * @author H.J.M van der Heijden
         */
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

            /**
             * Called when FairRoot calls PreRun.
             * Difference is that this catches exceptions.
             */
            virtual void preRun();

            /**
             * Called when FairRoot calls PostRun.
             * Difference is that this closes correctly in case of an exception.
             */
            virtual void postRun();

            /**
             * Called when FairRoot calls Run.
             * Difference is that this correctly closes in case of an exception.
             */
            virtual void run();

            /**
             * Called when FairRoot calls ConditionalRun.
             * Difference is that this wraps up exceptions.
             * @return
             */
            virtual bool conditionalRun();

            /**
             * When Zookeeper detects an fail-over.
             * This function is called to make sure the device is doing their stuff.
             * @param inMainThread
             */
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
