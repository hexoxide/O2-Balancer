// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Globals.h"
#include "O2/Balancer/Utilities/DataTypes.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"



using O2::Balancer::AbstractDevice;
using O2::Balancer::Settings;
using O2::Balancer::ClusterManager;
using O2::Balancer::Exceptions::AbstractException;

constexpr char EXCEPTION_MESSAGE[] = "The following exception occurred ";

AbstractDevice::AbstractDevice(const std::string &name,
                               std::shared_ptr<Settings> settings,
                               bool restartOnUpdate) {
    this->fId = name;
    this->settings = settings;
    this->nStop = false;
    this->nRefresh = false;
    this->restartOnUpdate = restartOnUpdate;


    this->clusterManager = std::shared_ptr<ClusterManager>(new ClusterManager(
            settings->getSettingsServer()->ip,
            settings->getSettingsServer()->port
    ));

    this->zooThread = std::thread(&AbstractDevice::checkZooKeeper, this);

    // this->fNetworkInterface = "default";
    //   this->fNumIoThreads = 1;
    // this->fPortRangeMin = 22000;
    // this->fPortRangeMax = 32000;
    // this->fInitializationTimeoutInS = 1;
    this->defaultTransport = this->getProperty(
            O2::Balancer::Globals::EnvironmentVariables::O2_TRANSPORT_VAR,
            O2::Balancer::Globals::FairMessageOptions::ZERO_MQ
    );
    variableChecksOut(
            this->defaultTransport,
            O2::Balancer::Globals::FairMessageOptions::ZERO_MQ,
            O2::Balancer::Globals::FairMessageOptions::NANO_MSG,
            O2::Balancer::Globals::FairMessageOptions::SHARED_MEMORY
    );

    if (this->defaultTransport != O2::Balancer::Globals::FairMessageOptions::ZERO_MQ) {
        LOG(WARN) << "ZeroMQ is the recommended transport, using " << this->defaultTransport << " might contain bugs";
    }
}


void AbstractDevice::useClusterManager(std::function<void(std::shared_ptr<ClusterManager>)> cl) {
    std::unique_lock<std::mutex> lck(this->zoolock);
    cl(this->clusterManager);
}

bool AbstractDevice::ConditionalRun() {
    try {
        return this->conditionalRun();
    } catch (const AbstractException &exception) {
        LOG(ERROR) << EXCEPTION_MESSAGE << exception.getMessage();
        return false;
    } catch (...) { // Catch all other exceptions.
        LOG(ERROR) << "Crashed with unknown exception in a conditional run";
        std::exit(EXIT_FAILURE);

    }
}

void AbstractDevice::PreRun() {
    try {
        this->preRun();
    } catch (const AbstractException &exception) {
        LOG(ERROR) << EXCEPTION_MESSAGE << exception.getMessage();
    } catch (...) { // Catch all the other exceptions.
        LOG(ERROR) << "Crashed with unknown exception in a PreRun";
        std::exit(EXIT_FAILURE);
    }
}

void AbstractDevice::Run() {
    try {
        this->run();
    } catch (const AbstractException &exception) {
        LOG(ERROR) << EXCEPTION_MESSAGE << exception.getMessage();
    } catch(...) { // Catch all the other exceptions.
        LOG(ERROR) << "Crashed with unknown exception in a Run";
        std::exit(EXIT_FAILURE);
    }
}

void AbstractDevice::PostRun() {
    try {
        this->postRun();
    } catch (const AbstractException &exception) {
        LOG(ERROR) << EXCEPTION_MESSAGE << exception.getMessage();
    } catch(...) { // Catch all the other exceptions.
        LOG(ERROR) << "Crashed with unknown exception in a post run";
        std::exit(EXIT_FAILURE);
    }

}

void AbstractDevice::preRun() {
    FairMQDevice::PreRun();
}

void AbstractDevice::run() {
    FairMQDevice::Run();
}

void AbstractDevice::postRun() {
    FairMQDevice::PostRun();
}

bool AbstractDevice::conditionalRun() {
    FairMQDevice::ConditionalRun();
}


void AbstractDevice::checkZooKeeper() {
    while (!this->nStop) {
        std::unique_lock<std::mutex> lck(this->zoolock);
        this->nRefresh = this->clusterManager->requiresUpdate() && this->restartOnUpdate;
        lck.unlock();
        if (this->clusterManager->requiresUpdate() && !this->restartOnUpdate) {
            this->refreshDevice(false);
        }
    }
}

void AbstractDevice::restartDevice() {

    // Refresh the device, stopping everything and setup the new stuff
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds{};
    start = std::chrono::system_clock::now();

    ChangeState(AbstractDevice::STOP);
    WaitForEndOfState(AbstractDevice::STOP);

    ChangeState(AbstractDevice::RESET_TASK);
    WaitForEndOfState(AbstractDevice::RESET_TASK);

    ChangeState(AbstractDevice::RESET_DEVICE);
    WaitForEndOfState(AbstractDevice::RESET_DEVICE);
    this->refreshDevice(true);
    ChangeState(AbstractDevice::INIT_DEVICE);

    WaitForInitialValidation();
    WaitForEndOfState(AbstractDevice::INIT_DEVICE);

    ChangeState(AbstractDevice::INIT_TASK);
    WaitForEndOfState(AbstractDevice::INIT_TASK);

    ChangeState(AbstractDevice::RUN);
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    LOG(INFO) << "Reset time: " << elapsed_seconds.count() << " seconds";
}

bool AbstractDevice::addHandle(const std::string &tag, const DeviceSetting &setting) {
    std::unique_lock<std::mutex> lck(this->zoolock);

    return this->clusterManager->registerConnection(
            this->fId,
            tag,
            setting
    );
}

bool AbstractDevice::needRefresh() const {
    return this->nRefresh;
}


bool AbstractDevice::needToStop() const {
    return this->nStop;
}


std::string AbstractDevice::getDefaultTransport() const {
    return this->defaultTransport;
}

std::string AbstractDevice::getProperty(const std::string &varName, const std::string &defValue) {
    auto result = std::getenv(varName.c_str());

    if (result == nullptr && !defValue.empty()) {
        return defValue;
    }
    if (result == nullptr) {
        throw Exceptions::InitException("Could not load " + varName + "property");
    }
    return result;
}


void AbstractDevice::quit() {
    this->nStop = true;
    LOG(INFO) << "closing thread";
    if (this->zooThread.joinable()) {
        this->zooThread.join();
    }
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}