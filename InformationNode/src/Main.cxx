// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/*
* Main.cxx
* @author H.J.M van der Heijden
* @since 25-08-2017
*/
#include "O2/InformationDevice.h"
#include <O2/DeviceManager.h>
int main(int argc, char** argv){
    try{
        O2::DeviceManager<O2::InformationDevice> manager;
        manager.run();
    } catch(O2::Exceptions::InitException exception){
        LOG(ERROR) << "Failed to initialize due, error :" << exception.getMessage();
        return EXIT_FAILURE;
    } catch (O2::Exceptions::AbstractException exception){
        LOG(ERROR) << exception.getMessage();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
