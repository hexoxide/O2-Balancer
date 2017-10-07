// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_EPN_EPNSETTINGS_H
#define O2_EPN_EPNSETTINGS_H

#include <string>
#include <memory>
#include <vector>
#include <O2/Balancer/Utilities/DeviceSetting.h>
#include <boost/program_options.hpp>

namespace O2{
    namespace EPN{
        class EPNSettings{
            std::shared_ptr<Balancer::DeviceSetting> informationSettings;
            int flpConnectionPort;
            int outputConnectionPort;
            int amountOfFLPs;
            std::string ip;
        public:
            EPNSettings(const boost::program_options::variables_map& settings);
            std::shared_ptr<Balancer::DeviceSetting> getInformationNodeSetting() const;
            int FLPConnectionPort() const;
            int OutputConnectionPort() const;
            std::string getIpAddress() const;
            int getAmountOfFLPs() const;
        };
    }
}

#endif