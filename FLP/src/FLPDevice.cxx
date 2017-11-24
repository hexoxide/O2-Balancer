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

#include "O2/FLP/Utils.h"


using namespace O2::FLP;

FLPDevice::FLPDevice(std::shared_ptr<FLPSettings> settings) : Balancer::AbstractDevice(
        O2::Balancer::Globals::DeviceNames::FLP_NAME, settings, settings->restartFairRoot()) {

    this->heartBeatConnection = std::unique_ptr<HeartbeatConnection>(
            new HeartbeatConnection(settings, this)
    );
    this->epnConnection = std::unique_ptr<EPNConnection>(
            new EPNConnection(settings, this)
    );

    this->averageSampleSize = 0;
    this->sampleType = settings->getSampleType();
    this->lastHeartbeat = 0;
}


void FLPDevice::preRun() {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        try {
            this->averageSampleSize = manager->getGlobalInteger("sampleSize", 1000);
        } catch (const O2::Balancer::Exceptions::ClusterHandlerException &ex) {
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


bool FLPDevice::conditionalRun() {
    FairMQChannel &dataInChannel = fChannels.at(this->heartBeatConnection->getName()).at(0);
    std::fstream fstream("/dev/null", std::ifstream::binary | std::ios::in);
    if (fstream.good()) {
        FairMQParts parts;
        int generatedSize;
        switch (this->sampleType) {
            case SampleType::Random: {
                generatedSize = generateRandomSize(this->averageSampleSize);
                break;
            }
            case SampleType::Sine: {
                generatedSize = generateSineSize(this->averageSampleSize, lastHeartbeat);
                LOG(INFO) << generatedSize;
                break;
            }
            default: {
                generatedSize = this->averageSampleSize;
                break;
            }
        }
        //const int generatedSize = generateRandomSize(this->averageSampleSize);//generateSineSize(this->averageSampleSize, 1);

        const int size = generatedSize * 1024 * 1024;
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