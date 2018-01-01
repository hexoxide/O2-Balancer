// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EPN_EPN_GLOBALS_H
#define O2_EPN_EPN_GLOBALS_H

namespace O2 {
    namespace EPN {

        // General information
        constexpr char DEVICE_NAME[] = "InformationNode";

        constexpr char ACK_CHANNEL_STR[] = "ack";

        // Settings
        constexpr char CONFIG_FILE_SETTING[] = "epn-config";

        constexpr char CONFIG_FILE_DEFAULT[] = "./epn.yaml";

        constexpr char AMOUNT_OF_FLPS_SETTING[] = "amount-flps";

        constexpr int AMOUNT_OF_FLPS_DEFAULT = 2;

        constexpr char FLP_PORT_SETTING[] ="flp-port";


    }
}

#endif // O2_EPN_EPN_GLOBALS_H