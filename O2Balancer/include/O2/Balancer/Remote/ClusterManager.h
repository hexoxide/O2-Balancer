// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_REMOTE_CLUSTER_MANAGER_H
#define O2_BALANCER_REMOTE_CLUSTER_MANAGER_H

#include <string>
#include <vector>
#include <memory>

//forward declaration, containing ZooKeeper specification
typedef struct _zhandle zhandle_t;


namespace O2{

    namespace Balancer{

        
        class DeviceSetting;
        /**
         * The file that interacts and shares data among the other machines on the cluster.
         * For this, ZooKeeper C library is used. 
         * @author H.J.M van der Heijden
         * @date 10 October 2017
         * */
        class ClusterManager{
            zhandle_t *zh;
            /**
             *  Setups the topology used within ZooKeeper.
             * */
            void setupDirectories();
        public:
            /**
             *  Constructor that connects to the specified IP and Port used by the ZooKeeper server.
             *  @param zooServer    The IP address. (example "localhost")
             *  @param port         The port of ZooKeeper. (example 2181)
             * */
            ClusterManager(const std::string& zooServer, const int& port);

            /**
             *  Shares an variable with all connected machines on the cluster.
             *  However when the machine disconnects, the variable is lost.
             *  @name   The identifier of the variable.
             *  @value  The value of the variable
             * */
            void addGlobalString(const std::string& name, const std::string& value);
            
            void addGlobalInteger(const std::string& name,  int value);
            /**
             *  Gets a variable that is shared from an other machine.
             *  Although time out is not really required, Sometimes machines are not started in the same order thus requiring to wait a while.
             *  The time out is thus for how long you wish to wait until giving up. (zero is wait indefinitly)
             *  @value name,    the identifier of the variable
             *  @value timeout, The amount of time you wish to wait until giving up.
             * */
            std::string getGlobalString(const std::string& name, int timeout = 0);
            
            int getGlobalInteger(const std::string& name, int timeout);

            /**
             *  Not yet implemented
             * */
            void registerConnection(const std::string& classification, const std::string& tag, const DeviceSetting& setting );
            
            /**
             *  Closes the connection, making the class unusable after calling this.
             * */
            void close();
        };
    }
}

#endif