// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.


#ifndef O2_BALANCER_DEVICES_CONNECTION_H
#define O2_BALANCER_DEVICES_CONNECTION_H

#include <fairmq/FairMQChannel.h>
#include <functional>
#include <string>
#include <vector>

namespace O2 {
    namespace Balancer {

        /**
         *  Statically typed wrapper for FairRoot for all the connection types.
         *  This is to prevent typos.
         * */
        enum class ConnectionType {
            Publish,
            Pull,
            Push,
            Subscribe
        };


        /**
         *  Statically typed wrapper for FairRoot for all the ConnectionMethods.
         *  Primarily used to prevent typos.
         * */
        enum class ConnectionMethod {
            Bind,
            Connect
        };

        class AbstractDevice;

        class ClusterManager;

        struct DeviceSetting;

        /**
        *   Class for maintaining 1 connection
        *   @author H.J.M van der Heijden
        *   @since 25-08-2017
        */
        class Connection {
            /*
            *   FairMQ dynamically creates connections.
            *   This function allows us to make it more statically.
            */
            std::string typeToString(ConnectionType type) const;

            std::string methodToString(ConnectionMethod method) const;

            std::string name;
            AbstractDevice *device;
        protected:
            /**
             * Allows you to use the clustermanager without worrying about mutexes.
             * @param cl
             */
            void useClusterManager(std::function<void(std::shared_ptr<ClusterManager>)> cl);

        public:
            /**
             *  A generic connection class for maintaining connections.
             *  @param name, The identifier of the connection used in Zookeeper
             *  @param device,  The fairroot device that requires handling. 
             * */
            Connection(const std::string &name, AbstractDevice *device);

            std::vector<std::string> getOfflineDevices(std::vector<DeviceSetting> nChannels);

            /**
             *  Gets all the FairMQChannels and updates all the log rates.
             *  @param logRate, The logging variable
             * */
            void updateAllRateLogging(const int &logRate);

            std::vector<FairMQChannel> &getChannels() const;

            size_t channelSize() const;

            void updateChannels(std::vector<DeviceSetting> nChannels);

            /**
             *  Gets all the FairMQChannels and updates all the receive buffers. 
             *  @param buffer, the updated receive buffer
             * */
            void updateAllReceiveBuffer(const int &buffer);

            /**
             *  Updates all the associated FairMQChannels and updates the send buffer.
             *  @param buffer,  The new send buffer. 
             * */
            void updateAllSendBuffer(const int &buffer);

            /**
             *  Updates all the send kernel sizes of the associated FairMQChannels.
             *  @param size,    The new size
             * */
            void updateAllSendKernelSize(const int &size);

            /**
             *  Updates all the receive kernel sizes of the associated FairMQChannels.
             *  @param size, the new size 
             * */
            void updateAllReceiveKernelSize(const int &size);

            /**
             * Adds an output channel that sends (or receives) data through FairRoot, this can be called multipletimes.
             * Will not register to ZooKeeper and is invisible.
             * All the connections are using TCP
             * @param type, The connectionType for patterns used in FairRoot.
             * @param method, The connectionMethod used by FairRoot
             * @param ip.   The ip address to bind the connection
             * @param port, The connection port.
             * */
            std::shared_ptr<DeviceSetting>
            addOutputChannel(ConnectionType type, ConnectionMethod method, const std::string &ip, int port);

            /**
             * Adds an input channel that receives data through FairRoot, calling this multiple times is not a good idea.
             * Will register an interface at zookeeper to become visible
             * All the connections are using TCP
             * @param type, The connectionType for patterns used in FairRoot.
             * @param method, The connectionMethod used by FairRoot
             * @param ip.   The ip address to bind the connection
             * @param port, The connection port.
             * */
            std::shared_ptr<DeviceSetting>
            addInputChannel(ConnectionType type, ConnectionMethod method, const std::string &ip, int port);

            std::string getName() const;
        };
    }

}


#endif
