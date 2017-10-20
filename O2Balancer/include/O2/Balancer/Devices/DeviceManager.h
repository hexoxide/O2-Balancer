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
        /**
         *  Global static variable, to be changed after the program quits.
         *  Required for signaling.
         * */
        namespace{
            bool shouldStop;
        }
        class AbstractDevice;

        /**
         *  Class for maintaining the state of a device specified by FairRoot.
         *  When called the device will catch the signals.
         *  @author H.J.M van der Heijden
         *  @date 10 October 2017
         * */
        template<class T>
        class DeviceManager{
             std::unique_ptr<T> device;
        public:
            /**
             *  Creates the device and initializes FairRoot.
             *  When called, the signals will be catched.
             *  @param args     The constructor arguments for the purposed device
             * */
            template<typename... Arguments>
            DeviceManager(Arguments... args){
                shouldStop = false;
            
                device = std::unique_ptr<T>(new T(args...));
                device->CatchSignals();
                std::signal(SIGINT,[](int sig) -> void{
                    shouldStop = true;
                    LOG(INFO) << "Stopping device!";
                });
                device->SetTransport(device->getDefaultTransport());
                device->ChangeState(T::INIT_DEVICE);
                    
                device->WaitForInitialValidation();
                device->WaitForEndOfState(T::INIT_DEVICE);
                    
                device->ChangeState(T::INIT_TASK);
                device->WaitForEndOfState(T::INIT_TASK);
                
            
            }
            /**
             *  Starts the state of the device.
             *  Will return when the abort signall is called.
             * */
            void run(){
                
                device->ChangeState(T::RUN);

                while(!device->needToStop() && !shouldStop){
                    if(device->needRefresh()){
                        //Refresh the device, stopping everything and setup the new stuff
                        device->ChangeState(T::STOP);
                        device->WaitForEndOfState(T::STOP);

                        device->ChangeState(T::RESET_TASK);
                        device->WaitForEndOfState(T::RESET_TASK);

                        device->ChangeState(T::RESET_DEVICE);
                        device->WaitForEndOfState(T::RESET_DEVICE);
                        device->refreshDevice();
                        device->ChangeState(T::INIT_DEVICE);
                        
                        device->WaitForInitialValidation();
                        device->WaitForEndOfState(T::INIT_DEVICE);
                            
                        device->ChangeState(T::INIT_TASK);
                        device->WaitForEndOfState(T::INIT_TASK);

                        device->ChangeState(T::RUN);
                    }
                }
                
                LOG(INFO) << "Stopping";
             
                device->ChangeState(T::STOP);

                device->ChangeState(T::RESET_TASK);
                device->WaitForEndOfState(T::RESET_TASK);

                device->ChangeState(T::RESET_DEVICE);
                device->WaitForEndOfState(T::RESET_DEVICE);
                device->ChangeState(T::END);
                /*
                if (!device->CheckCurrentState(T::EXITING)){

                    device->ChangeState(T::RESET_TASK);
                    device->WaitForEndOfState(T::RESET_TASK);
            
                    device->ChangeState(T::RESET_DEVICE);
                    device->WaitForEndOfState(T::RESET_DEVICE);
            
                    device->ChangeState(T::END);
                }*/
                device->quit();
            }
        };
    }
   
}

#endif