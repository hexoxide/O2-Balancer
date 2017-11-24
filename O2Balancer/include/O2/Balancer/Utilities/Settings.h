// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_UTILITIES_SETTINGS_H
#define O2_BALANCER_UTILITIES_SETTINGS_H

#include <boost/program_options.hpp>
#include <memory>
#include <string>
#include <vector>

//forward declaration
namespace YAML {
    class Node;
}

namespace O2 {
    namespace Balancer {
        class DeviceSetting;

        /**
         *  Utilities for loading configuration files for the derived programs of O2 Balancer.
         *  Since the FairRoot json loader was quite hard to adapt for custom purposes, a new library has been adapted.
         *  The library used is yaml-cpp. Using the YAML (Yaml aint a markup language) markup language.
         *  @author H.J.M van der Heijden
         *  @date 10 October 2017
         * */
        class Settings {
        private:
            std::shared_ptr<DeviceSetting> informationSettings;
            std::shared_ptr<DeviceSetting> settingsServer;
            std::string ipAddress;
        protected:
            /**
             * This class must be derived by the child. 
             * When this parent class requires the settings file.
             * */
            virtual std::string getSettingsFile() const = 0;

            /**
             *  Load the settings file, and sets up the general required options.
             *  Such as own IP address and the informationnode. 
             * */
            YAML::Node load(const boost::program_options::variables_map &settings);

        public:
            std::shared_ptr<DeviceSetting> getSettingsServer() const;

            std::shared_ptr<DeviceSetting> getInformationNodeSetting() const;

            std::string getIPAddress() const;

            Settings();

        };
    }
}

#endif