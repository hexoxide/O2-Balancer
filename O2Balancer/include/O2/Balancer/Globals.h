// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2BALANCER_GLOBALS_H
#define O2BALANCER_GLOBALS_H


namespace O2 {
    namespace Balancer {
        /**
         * Namespace containing various magic numbers and variables used throughout the library and the derived programs.
         * @author H.J.M van der Heijden
         * @date 10 October 2017
         * */
        namespace Globals {

            /**
             * The names and ids for all the derived programs to be used in ZooKeeper and FairRoot.
             * */
            namespace DeviceNames {
                constexpr char INFORMATION_NAME[] = "InformationNode";
                constexpr char FLP_NAME[] = "FLP";
                constexpr char EPN_NAME[] = "EPN";
            }



            /**
             * Namespace containing all the default options for message queuing in fairroot.
             * */
            namespace FairMessageOptions {
                constexpr char ZERO_MQ[] = "zeromq";
                constexpr char NANO_MSG[] = "nanomsg";
                constexpr char SHARED_MEMORY[] = "shmem";
            }

            /**
             *  Variables that can be set within the environment.
             * */
            namespace EnvironmentVariables {
                constexpr char O2_TRANSPORT_VAR[] =  "O2Transport";
            }

            /**
             * The layout and names used within Zookeeper.
             * */
            namespace ZooKeeperTopology {
                constexpr char VAR_ZNODE_ROOT[] = "/globals";
            }

        }
    }
}

#endif