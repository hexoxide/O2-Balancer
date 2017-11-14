// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/EPNConnection.h"
#include "O2/FLP/FLPDevice.h"
#include "O2/FLP/FLPSettings.h"
#include <chrono>
#include <boost/format.hpp>
#include <thread>
using namespace O2::FLP;

EPNConnection::EPNConnection(std::shared_ptr<FLPSettings> settings, Balancer::AbstractDevice* device) : Balancer::Connection("stf2", device){
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void{
        auto dev = manager->getRegisteredConnections("EPN", "stf2");
        while(dev.empty()){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            dev = manager->getRegisteredConnections("EPN", "stf2");
        }
    
        for(auto& epn : dev){
            LOG(INFO) << "sending data to : " << epn.ip << ":" << epn.port;
            this->addInputChannel(
                Balancer::ConnectionType::Push,
                Balancer::ConnectionMethod::Connect,
                epn.ip,
                epn.port
            );
        }
        
        this->updateAllSendBuffer(100000);
        this->updateAllRateLogging(1);

    });
}

void EPNConnection::updateBlacklist(){
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        const std::string classification = manager->pathThatNeedsUpdate();
        const std::string name =  this->getName();
        std::vector<std::string> offline = this->getOfflineDevices(manager->getRegisteredConnections(classification,name));
        this->offlineEPNS.swap(offline);
    });
}

size_t EPNConnection::balance(O2::Balancer::heartbeatID id){
    size_t direction = 0;
    O2::Balancer::heartbeatID dirbalancer = id + this->incrementer;
    this->incrementer = this->incrementer % this->amountOfEpns();    
    do {
        direction = dirbalancer % (this->amountOfEpns() - this->offlineEPNS.size());
        FairMQChannel& dataOutChannel = this->getChannels().at(direction);
        bool invalid = false;
        for(const std::string& ip : this->offlineEPNS){
            if(ip == dataOutChannel.GetAddress()){
                dirbalancer += 1;
                LOG(WARN) << "skipping machine " << ip;
                invalid=true;
                break;
            }
        }
        if(invalid){
            continue;
        }
        incrementer += 1;

        break;
    } while(true);
    return direction;

}

void EPNConnection::updateConnection(){
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        const std::string tmp = manager->pathThatNeedsUpdate();//only the epn path needs update
        this->updateChannels(manager->getRegisteredConnections(tmp, this->getName()));
        this->updateAllSendBuffer(100000);
        this->updateAllRateLogging(1);
    });
}

size_t EPNConnection::amountOfEpns() const{
    return this->channelSize();
}