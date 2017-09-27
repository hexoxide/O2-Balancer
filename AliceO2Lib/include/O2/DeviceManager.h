// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_DEVICE_MANAGER_H
#define O2_DEVICE_MANAGER_H

#include <memory>
#include "./SettingsManager.h"
namespace O2{
    class AbstractDevice;

    template<class T>
    class DeviceManager{
        std::unique_ptr<T> device;
    public:
        template<typename... Arguments>
        DeviceManager(Arguments... args){
            auto settings = SettingsManager::getInstance();
            //this->device = std::make_unique<T>(args...);
            this->device = std::unique_ptr<T>(new T(args...));
            device->CatchSignals();
            this->device->SetTransport(settings->getDefaultTransport());
            this->device->ChangeState(T::INIT_DEVICE);
                
            this->device->WaitForInitialValidation();
            this->device->WaitForEndOfState(T::INIT_DEVICE);
                
            this->device->ChangeState(T::INIT_TASK);
            this->device->WaitForEndOfState(T::INIT_TASK);
            

        }
        void run(){
            this->device->ChangeState(T::RUN);
            this->device->WaitForEndOfState(T::RUN);
            if (!device->CheckCurrentState(T::EXITING)){
                device->ChangeState(T::RESET_TASK);
                device->WaitForEndOfState(T::RESET_TASK);
        
                device->ChangeState(T::RESET_DEVICE);
                device->WaitForEndOfState(T::RESET_DEVICE);
        
                device->ChangeState(T::END);
            }
        }
    };
}

#endif