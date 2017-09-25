// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_SETTINGS_MANAGER_H
#define O2_SETTINGS_MANAGER_H

#include <memory>
#include <string>
#include "./exceptions/InitException.h"
namespace O2{
    class SettingsManager{
        static std::shared_ptr<SettingsManager> instance;
        std::string defaultTransport;

        void variableChecksOut(std::string var, std::string tail){
            if(var != tail){
                throw Exceptions::InitException(var + std::string(" is not an allowed value"));
            }
        }
        template<typename... Arguments>
        void variableChecksOut(std::string var, std::string head, Arguments... tail){
            if(var == head){
                return;
            } else {
                variableChecksOut(var, tail...);
            }
        }

        SettingsManager();
        std::string getProperty(const std::string& varName, const std::string& defValue);
    public:
        static std::shared_ptr<SettingsManager> getInstance();
        std::string getDefaultTransport() const;

        
    };
}

#endif