// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_FLP_FLP_SETTINGS_H
#define O2_FLP_FLP_SETTINGS_H

#include <memory>
#include <vector>
#include <O2/Balancer/Utilities/DeviceSetting.h>
#include <boost/program_options.hpp>


namespace O2{
    namespace FLP{
        class FLPSettings{
            std::shared_ptr<Balancer::DeviceSetting> informationSettings;
            std::vector<std::shared_ptr<Balancer::DeviceSetting>> epnSettings;
            int sampleSize;
            std::string ip;
        public:
            FLPSettings(const boost::program_options::variables_map& settings);
            std::shared_ptr<Balancer::DeviceSetting> getInformationNodeSetting() const;
            std::vector<std::shared_ptr<Balancer::DeviceSetting>> getEPNSettings() const;
            std::string getIpAddress() const;
            int getSampleSize() const;
        };
    }
}

#endif