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

#include <string>
#include <vector>
#include <FairMQChannel.h>

namespace O2{
    namespace Balancer{
        enum class ConnectionType{
            Publish,
            Pull,
            Push,
            Subscribe
        };
    
        enum class ConnectionMethod{
            Bind,
            Connect
        };
    
        class AbstractDevice;
    
        /**
        *   Class for maintaining 1 connection
        *   @author H.J.M van der Heijden
        *   @since 25-08-2017
        */
        class Connection{
            /*
            *   FairMQ dynamicly creates connections.
            *   This function allows us to make it more staticly.
            */
            std::string typeToString(ConnectionType type) const;
            std::string methodToString(ConnectionMethod method) const;
            std::string name;
            AbstractDevice* device;
            std::vector<FairMQChannel> channels;
        public:
            void updateAllRateLogging(const int& logg);
            void updateAllReceiveBuffer(const int& buffer);
            void updateAllSendBuffer(const int& buffer);
            void updateAllSendKernelSize(const int& size);
            void updateAllReceiveKernelSize(const int& size);
            void addChannel(ConnectionType type, ConnectionMethod method, const std::string& ip, int port);
            Connection(const std::string& name, AbstractDevice* device);
            std::string getName() const;
            std::vector<FairMQChannel> getChannels() const;
        };
    }
  
}


#endif