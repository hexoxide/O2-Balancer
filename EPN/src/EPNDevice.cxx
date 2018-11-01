// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "O2/EPN/EPNDevice.h"
#include <boost/smart_ptr/make_unique.hpp>
#include "O2/EPN/FLPConnection.h"
#include "O2/EPN/AcknowledgeConnection.h"
#include "O2/EPN/OutputConnection.h"

using O2::EPN::EPNDevice;
using O2::EPN::EPNSettings;
using O2::EPN::FLPConnection;
using O2::EPN::AcknowledgeConnection;
using O2::EPN::OutputConnection;


EPNDevice::EPNDevice(std::shared_ptr<EPNSettings> settings) : Balancer::AbstractDevice(
        O2::Balancer::Globals::DeviceNames::EPN_NAME, settings) {
    this->flpConnection = boost::make_unique<FLPConnection>(this,settings);
    this->acknowledgeConnection = boost::make_unique<AcknowledgeConnection>(this,settings);
    this->outputConnection = boost::make_unique<OutputConnection>(this,settings);
    this->mNumFLPs = settings->getAmountOfFLPs();
    this->mBufferTimeoutInMs = 900000;
}


void EPNDevice::DiscardIncompleteTimeframes() {
    auto it = mTimeframeBuffer.begin();
    while (it != mTimeframeBuffer.end()) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - (it->second).start).count() > mBufferTimeoutInMs) {
            LOG(WARN) << "Timeframe #" << it->first << " incomplete after " << mBufferTimeoutInMs
                      << " milliseconds, discarding";
            mDiscardedSet.insert(it->first);
            mTimeframeBuffer.erase(it++);
            LOG(WARN) << "Number of discarded timeframes: " << mDiscardedSet.size();
        } else {
            ++it;
        }
    }
}

void EPNDevice::refreshDevice(bool) {
    this->useClusterManager([this](std::shared_ptr<O2::Balancer::ClusterManager> manager) -> void {
        const std::string tmp = manager->pathThatNeedsUpdate();
        LOG(WARN) << boost::format("Refresh called on path %s, yet not fully supported on EPNs.") % tmp;
    });
}

void EPNDevice::run() {
    const std::string IP = this->settings->getIPAddress();
    O2::Balancer::heartbeatID id = 0;
    FairMQChannel &ackOutChannel = fChannels.at(this->acknowledgeConnection->getName()).at(0);
    auto settings = dynamic_cast<EPNSettings *>(this->settings.get());

    const bool isGoat = (IP == settings->getGoatIP());
    bool crashing = false;

    while (CheckCurrentState(RUNNING)) {
        FairMQParts parts;

        if (Receive(parts, this->flpConnection->getName(), 0, 1000) > 0) {
            // store the received ID
            O2::Balancer::f2eHeader &header = *(static_cast<O2::Balancer::f2eHeader *>(parts.At(0)->GetData()));
            id = header.timeFrameId;

            if (mDiscardedSet.find(id) == mDiscardedSet.end()) {
                if (mTimeframeBuffer.find(id) == mTimeframeBuffer.end()) {
                    // if this is the first part with this ID, save the receive time.
                    mTimeframeBuffer[id].start = std::chrono::steady_clock::now();
                }
                // if the received ID has not previously been discarded,
                // store the data part in the buffer
                mTimeframeBuffer[id].parts.AddPart(move(parts.At(1)));
            } else {
                // if received ID has been previously discarded.
                LOG(WARN) << "Received part from an already discarded timeframe with id " << id;
            }

            if (mTimeframeBuffer[id].parts.Size() == mNumFLPs) {
                size_t total = 0;
                for (int i = 0; i < mTimeframeBuffer[id].parts.Size(); i++) {
                    total += mTimeframeBuffer[id].parts.At(i)->GetSize();
                }
                LOG(INFO) << "Received " << (total / 1024 / 1024) << " Mega Bytes";
                // Send an acknowledgement back to the sampler to measure the round trip time
                std::string result = IP + "#" + std::to_string(id);
                std::unique_ptr<FairMQMessage> ack(NewMessage(sizeof(char) * result.length()));
                std::memcpy(ack->GetData(), result.c_str(), sizeof(char) * result.length());
                if (ackOutChannel.Send(ack, 0) <= 0) {
                    LOG(ERROR) << "Could not send acknowledgement without blocking";
                }

                if (isGoat) {
                    if (id >= settings->getAmountBeforeCrash()) {
                        if (!crashing) {
                            Balancer::crashAfterAmountOfBeats(
                                    settings->getHeartrate(),
                                    settings->getAmountAfterSignal(),
                                    true);
                            crashing = true;
                        }
                    }
                }
                mTimeframeBuffer.erase(id);
            }
        }

        // check if any incomplete timeframes in the buffer are older than timeout period, and discard them if they are
        DiscardIncompleteTimeframes();
    }
}

EPNDevice::~EPNDevice() = default;
