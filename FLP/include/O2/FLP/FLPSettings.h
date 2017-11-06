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

#include <vector>
#include <O2/Balancer/Utilities/DataTypes.h>
#include <O2/Balancer/Utilities/Settings.h>

namespace O2{
    namespace FLP{
        class FLPSettings : public Balancer::Settings{
        protected:
            std::string getSettingsFile() const override;
        public:
            FLPSettings(const boost::program_options::variables_map& settings);
        };
    }
}

#endif