#include "O2/Balancer/SettingsManager.h"
#include <cstdlib>
using namespace O2::Balancer;

std::shared_ptr<SettingsManager> SettingsManager::instance;

SettingsManager::SettingsManager(){
    this->defaultTransport = this->getProperty("O2Transport", "zeromq");
    this->variableChecksOut(this->defaultTransport, "zeromq", "nanomsg","shmem");
}

std::shared_ptr<SettingsManager> SettingsManager::getInstance(){
    if(SettingsManager::instance == nullptr){
        SettingsManager::instance = std::shared_ptr<SettingsManager>(new SettingsManager());
    }
    return SettingsManager::instance;
}

std::string SettingsManager::getProperty(const std::string& varName, const std::string& defValue){
    auto result = std::getenv(varName.c_str());

    if(result == nullptr && defValue != ""){
        return defValue;
    }
    if(result == nullptr){
        throw Exceptions::InitException("Could not load " + varName + "property");
    }
    return result;
}

std::string SettingsManager::getDefaultTransport() const {
    return this->defaultTransport;
}
