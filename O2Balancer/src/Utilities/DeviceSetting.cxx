#include "O2/Balancer/Utilities/DeviceSetting.h"
#include <regex>
#include <iostream>
using namespace O2::Balancer;

DeviceSetting::DeviceSetting(int port, std::string ip){
    this->port = port;
    this->ip = ip;
}

const std::regex parseSetting("([^:]+):([0-9]+)", std::regex_constants::ECMAScript);

DeviceSetting::DeviceSetting(const std::string& setting){
    std::smatch match;
    std::regex_match(setting, match, parseSetting);
    this->ip = match[1];
    this->port = std::stoi(match[2]);
}

