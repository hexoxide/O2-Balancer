#include "O2/InformationNode/InfoSettings.h"
#include <yaml-cpp/yaml.h>
using namespace O2::InformationNode;

InfoSettings::InfoSettings(const boost::program_options::variables_map& settings ) : Settings() {
    YAML::Node res = this->load(settings);
    this->heartRate = res["heartrate"].as<int>();
    this->acknowledgePort = res["acknowledgePort"].as<int>();
    this->heartBeatPort = res["heartBeatPort"].as<int>();
}

std::string InfoSettings::getSettingsFile() const {
    return "info-config";
}

int InfoSettings::getHeartRate() const{
    return this->heartRate;
}
int InfoSettings::getAcknowledgePort() const{
    return this->acknowledgePort;
}

int InfoSettings::getHeartBeatPort() const{
    return this->heartBeatPort;
}