#include "O2/Balancer/Devices/AbstractDevice.h"
#include "O2/Balancer/Devices/Connection.h"
#include "O2/Balancer/Remote/ClusterManager.h"
#include "O2/Balancer/Utilities/Utilities.h"
#include "O2/Balancer/Utilities/Settings.h"
#include "O2/Balancer/Globals.h"
#include "O2/Balancer/Utilities/DataTypes.h"
#include "O2/Balancer/Exceptions/UnimplementedException.h"
#include <cstdlib>
#include <boost/format.hpp>
#include "O2/Balancer/Exceptions/InitException.h"

using namespace O2::Balancer;

AbstractDevice::AbstractDevice(const std::string& name, std::shared_ptr<Settings> settings, bool restartOnUpdate){
    this->fId = name;
    this->settings = settings;
    this->nStop = false;
    this->nRefresh = false;
    this->restartOnUpdate = restartOnUpdate;


    this->clusterManager = std::shared_ptr<ClusterManager>(new ClusterManager(
        settings->getSettingsServer()->ip,
        settings->getSettingsServer()->port
    ));

    this->zooThread = std::thread(&AbstractDevice::checkZooKeeper,this );
    
    // this->fNetworkInterface = "default";
 //   this->fNumIoThreads = 1;
   // this->fPortRangeMin = 22000;
   // this->fPortRangeMax = 32000;
   // this->fInitializationTimeoutInS = 1;
    this->defaultTransport = this->getProperty(
        Globals::EnvironmentVariables::O2_TRANSPORT_VAR,
        Globals::FairMessageOptions::ZERO_MQ
    );
    variableChecksOut(
        this->defaultTransport, 
        Globals::FairMessageOptions::ZERO_MQ,
        Globals::FairMessageOptions::NANO_MSG,
        Globals::FairMessageOptions::SHARED_MEMORY
    );

    if(this->defaultTransport != Globals::FairMessageOptions::ZERO_MQ){
        LOG(WARN) << "ZeroMQ is the recommended transport, using " << this->defaultTransport << " might contain bugs";
    }
}


void AbstractDevice::useClusterManager(std::function<void(std::shared_ptr<ClusterManager>)> cl){
   // static bool entered = false;
   // if(entered){
   //     throw O2::Balancer::Exceptions::UnimplementedException("A deadlock has occured, you can't call this function recursively");
   // }
   // entered = true;
    std::unique_lock<std::mutex> lck (this->zoolock);

    cl(this->clusterManager);

   // entered = false;
}

bool AbstractDevice::ConditionalRun() {
    try{
        return this->conditionalRun();
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << "The following uncatched exception occured " << exception.getMessage();
        return false;
    }
}

void AbstractDevice::PreRun() {
    try{
        this->preRun();
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << "The following uncatched exception occured " << exception.getMessage();
    }
}

void AbstractDevice::Run(){
    try{
        this->run();
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << "The following uncatched exception occured " << exception.getMessage();
    }
}

void AbstractDevice::PostRun() {
    try{
        this->postRun();
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << "The following uncatched exception occured " << exception.getMessage();
    }
}

void AbstractDevice::preRun(){
    FairMQDevice::PreRun();
}

void AbstractDevice::run(){
    FairMQDevice::Run();
}

void AbstractDevice::postRun(){
    FairMQDevice::PostRun();
}
bool AbstractDevice::conditionalRun(){
    FairMQDevice::ConditionalRun();
}


void AbstractDevice::checkZooKeeper(){
    while(!this->nStop){
        std::unique_lock<std::mutex> lck (this->zoolock);
        this->nRefresh = this->clusterManager->requiresUpdate() && this->restartOnUpdate;
        lck.unlock();
        if(this->clusterManager->requiresUpdate() && !this->restartOnUpdate){
            this->refreshDevice(false);
        }
    }
}

void AbstractDevice::restartDevice(){
   //Refresh the device, stopping everything and setup the new stuff
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
}

bool AbstractDevice::addHandle(const std::string& tag, const DeviceSetting& setting){
    std::unique_lock<std::mutex> lck (this->zoolock);

    return this->clusterManager->registerConnection(
        this->fId,
        tag,
        setting
    );
}

bool AbstractDevice::needRefresh() const{
    return this->nRefresh;
}

     
bool AbstractDevice::needToStop() const{
    return this->nStop;
}



/*void AbstractDevice::PostRun(){
    LOG(INFO) << "Closing device : " << fId;
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}*/

std::string AbstractDevice::getDefaultTransport() const{
    return this->defaultTransport;
}

std::string AbstractDevice::getProperty(const std::string& varName, const std::string& defValue) {
    auto result = std::getenv(varName.c_str());

    if(result == nullptr && defValue != ""){
        return defValue;
    }
    if(result == nullptr){
        throw Exceptions::InitException("Could not load " + varName + "property");
    }
    return result;
}

//std::shared_ptr<ClusterManager> AbstractDevice::getClusterManager() const{
//    return this->clusterManager;
//}

void AbstractDevice::quit(){
    this->nStop = true;
    LOG(INFO) << "closing thread";
    if(this->zooThread.joinable()){
        this->zooThread.join();
    }
    this->clusterManager->close();
    this->clusterManager.reset();
    this->settings.reset();
}