// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#ifndef O2_BALANCER_DEVICES_DEVICE_MANAGER_H
#define O2_BALANCER_DEVICES_DEVICE_MANAGER_H

#include <chrono>
#include <thread>
#include <memory>
#include <csignal>
namespace O2{
    namespace Balancer{

        namespace{
            bool shouldStop;
        }
        class AbstractDevice;

        template<class T>
        class DeviceManager{
             std::unique_ptr<T> device;
        public:

            template<typename... Arguments>
            DeviceManager(Arguments... args){
                shouldStop = false;
            
                device = std::unique_ptr<T>(new T(args...));
                device->CatchSignals();
                std::signal(SIGINT,[](int sig) -> void{
                    shouldStop = true;
                });
                device->SetTransport(device->getDefaultTransport());
                device->ChangeState(T::INIT_DEVICE);
                    
                device->WaitForInitialValidation();
                device->WaitForEndOfState(T::INIT_DEVICE);
                    
                device->ChangeState(T::INIT_TASK);
                device->WaitForEndOfState(T::INIT_TASK);
                
            
            }
            void run(){
                
                device->ChangeState(T::RUN);

                while(!shouldStop){
                   std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                device->ChangeState(T::STOP);

                device->ChangeState(T::RESET_TASK);
                device->WaitForEndOfState(T::RESET_TASK);

                device->ChangeState(T::RESET_DEVICE);
                device->WaitForEndOfState(T::RESET_DEVICE);
                device->ChangeState(T::END);

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
   
}

#endif