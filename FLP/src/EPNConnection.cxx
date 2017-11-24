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
#include <O2/Balancer/Exceptions/UnimplementedException.h>

using namespace O2::FLP;

constexpr char EPN_TAG[] = "EPN";
constexpr char EPN_CHANNEL[] = "stf2";

EPNConnection::EPNConnection(std::shared_ptr<FLPSettings>, Balancer::AbstractDevice *device) : Balancer::Connection(
        "stf2", device) {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        auto dev = manager->getRegisteredConnections(EPN_TAG, EPN_CHANNEL);
        while (dev.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            dev = manager->getRegisteredConnections(EPN_TAG, EPN_CHANNEL);
        }

        for (auto &epn : dev) {
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

void EPNConnection::updateBlacklist() {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        const std::string classification = manager->pathThatNeedsUpdate();
        const std::string name = this->getName();
        std::vector<std::string> offline = this->getOfflineDevices(
                manager->getRegisteredConnections(classification, name));
        this->offlineEPNS.swap(offline);
    });
}

size_t EPNConnection::balance(O2::Balancer::heartbeatID id) {

    if (this->offlineEPNS.empty()) {
        return id % this->amountOfEpns();
    } else if (this->offlineEPNS.size() == this->getChannels().size()) {
        LOG(WARN) << "No EPN is online... ";
        return id % this->amountOfEpns();
    } else {
        auto channels = this->getChannels();

        // store all the online epns in a list
        std::vector<std::string> filteredList;
        for (const auto &i : channels) {
            bool isOffline = false;
            for (const std::string &offline : this->offlineEPNS) {
                if (offline == i.GetAddress()) {
                    isOffline = true;
                    break;
                }
            }
            if (!isOffline) {
                filteredList.emplace_back(i.GetAddress());
            }
        }

        //do the round robin on the filtered list
        const size_t storedList = id % filteredList.size();

        // Detect to which channel it points
        for (size_t i = 0; i < channels.size(); i++) {
            if (channels.at(i).GetAddress() == filteredList.at(storedList)) {
                return i;
            }
        }
        // This shouldn't be called.
        // But just in case throw an error
        throw Balancer::Exceptions::UnimplementedException("Could choose a index");

    }
}

void EPNConnection::updateConnection() {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        const std::string tmp = manager->pathThatNeedsUpdate();//only the epn path needs update
        this->updateChannels(manager->getRegisteredConnections(tmp, this->getName()));
        this->updateAllSendBuffer(100000);
        this->updateAllRateLogging(1);
    });
}

size_t EPNConnection::amountOfEpns() const {
    return this->channelSize();
}