// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/FLP/FLPDevice.h"

#include "O2/FLP/HeartBeatConnection.h"
#include "O2/FLP/EPNConnection.h"
#include <boost/make_unique.hpp>

#include "O2/FLP/Utils.h"


using O2::FLP::FLPDevice;
using O2::FLP::FLPSettings;
using O2::FLP::HeartbeatConnection;
using O2::FLP::EPNConnection;
using O2::Balancer::AbstractDevice;
using O2::Balancer::ClusterManager;
using O2::Balancer::Exceptions::ClusterHandlerException;

FLPDevice::FLPDevice(std::shared_ptr<FLPSettings> settings) : AbstractDevice(
        O2::Balancer::Globals::DeviceNames::FLP_NAME, settings, settings->restartFairRoot()) {

    this->heartBeatConnection = boost::make_unique<HeartbeatConnection>(settings,this);
    this->epnConnection = boost::make_unique<EPNConnection>(settings,this);

    this->averageSampleSize = 0;
    this->sampleType = settings->getSampleType();
    this->lastHeartbeat = 0;
}


void FLPDevice::preRun() {
 //   constexpr char SAMPLE_GLOBAL_NAME[] = "sampleSize";
 //   constexpr int MAXIMUM_TIME_OUT = 1000;
    this->useClusterManager([this](std::shared_ptr<ClusterManager> manager) -> void {
            constexpr int MAXIMUM_TIME_OUT = 1000;
	    constexpr char SAMPLE_GLOBAL_NAME[] = "sampleSize";
	try {
            this->averageSampleSize = manager->getGlobalInteger(SAMPLE_GLOBAL_NAME,
                                                                MAXIMUM_TIME_OUT);
        } catch (const ClusterHandlerException &ex) {
            LOG(ERROR) << ex.getMessage();
        }
    });
}


void FLPDevice::refreshDevice(bool inMainThread) {

    if (inMainThread) {
        this->epnConnection->updateConnection();
    } else {
        LOG(INFO) << "Refreshing blacklist";
        this->epnConnection->updateBlacklist();
    }
}

int FLPDevice::getSampleSize() const{
    switch (this->sampleType) {
        case SampleType::Random: {
            return generateRandomSize(this->averageSampleSize);
            break;
        }
        case SampleType::Sine: {
            return generateSineSize(this->averageSampleSize, lastHeartbeat);
            break;
        }
        default: {
            return this->averageSampleSize;

        }
    }
}

bool FLPDevice::conditionalRun() {
    FairMQChannel &dataInChannel = fChannels.at(this->heartBeatConnection->getName()).at(0);
    std::fstream fstream("/dev/null", std::ifstream::binary | std::ios::in);
    if (fstream.good()) {
        FairMQParts parts;
        const int generatedSize = getSampleSize();
        const int size = generatedSize * 1024;
        auto buffer = new char[size];
        fstream.read(buffer, size);

        parts.AddPart(NewMessage());
        parts.AddPart(NewMessage(
                buffer,
                size,
                [](void * /*data*/, void *object) { delete[] static_cast<char *> (object); },
                buffer
        ));

        if (dataInChannel.Receive(parts.At(0)) >= 0) {
            lastHeartbeat = *(static_cast<O2::Balancer::heartbeatID *>(parts.At(0)->GetData()));
            if (this->epnConnection->amountOfEpns() == 0) {
                LOG(WARN) << boost::format("Timeframe %i discarded, all the EPNs are offline") % lastHeartbeat;
                return true;
            }

            int direction = this->epnConnection->balance(lastHeartbeat);

            LOG(INFO) << boost::format("Direction: %i, amount of EPNs: %i, heartbeat: %i") % direction %
                         this->epnConnection->amountOfEpns() % lastHeartbeat;
            if (Send(parts, this->epnConnection->getName(), direction, 0) < 0) {
                LOG(ERROR) << boost::format("could not send to EPN %i") % direction;
            }
        }
    }
    fstream.close();
    return true;
}


FLPDevice::~FLPDevice() {
    LOG(INFO) << "closing";
}
