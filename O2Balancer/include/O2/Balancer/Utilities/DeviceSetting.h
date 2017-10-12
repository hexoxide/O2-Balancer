// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_DEVICE_SETTING_H
#define O2_BALANCER_DEVICE_SETTING_H

#include <string>

namespace O2{

    namespace Balancer{
        /**
        *   Small classes to interchange data.
        *   @author H.J.M van der Heijden
        *   @date 11 October 2017
        **/
        struct DeviceSetting{
            int port;
            std::string ip;
            DeviceSetting(int port, std::string ip);
            DeviceSetting(const std::string& setting);
            
        };
    }
}


#endif