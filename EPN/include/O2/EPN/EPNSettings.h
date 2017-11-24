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

#include <O2/Balancer/Utilities/DataTypes.h>
#include <O2/Balancer/Utilities/Settings.h>
#include <vector>
#include <string>


namespace O2 {
    namespace EPN {

        /**
         * Loads, determines and collects all settings required to run the EPN.
         * The EPN can be a goat. Which means that it call an abort on itself after a certain amount of heartbeats
         * @author H.J.M van der Heijden.
         */
        class EPNSettings : public Balancer::Settings {
            int flpConnectionPort;
            int outputConnectionPort;
            int amountOfFLPs;
            std::string goatIP;
            int heartrate;
            int amountAfterSignal;
            int amountBeforeCrash;


        protected:
            std::string getSettingsFile() const override;

        public:
            /**
             * Only constructor, with boost command line arguments
             * @param settings Containing the preparsed command line arguments from boost.
             */
            explicit EPNSettings(const boost::program_options::variables_map &settings);

            /**
             * Gets port that the FLPs can use
             * @return The flp port
             */
            int FLPConnectionPort() const;


            int OutputConnectionPort() const;

            /**
             * Get the amount of flps currently sending data to this EPN.
             * This is required, otherwise there is no possibility to know if a full time frame is accepted
             * @return The amount of online flps.
             */
            int getAmountOfFLPs() const;

            /**
             * The IP that will be sacreficed and crashes.
             * When the ip matches this machines ip. It will crash
             * @return IP address of the EPN that will crash
             */
            std::string getGoatIP() const;

            /**
             * Gets the heartrate to be mimiced for the EPN crash when this is the Goat.
             * @return Mimicked heartrate
             */
            int getHeartrate() const;

            /**
             * How long should the goat wait before it mimicks a crash
             * @return Heartbeats to wait
             */
            int getAmountAfterSignal() const;

            /**
             * How long should it wait after the signal to crash?
             * @return Heartbeats to wait
             */
            int getAmountBeforeCrash() const;
        };
    } // namespace EPN
} // namespace O2

#endif // O2_EPN_EPNSETTINGS_H
