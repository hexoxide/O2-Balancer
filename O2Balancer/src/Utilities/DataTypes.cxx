#include "O2/Balancer/Utilities/DataTypes.h"
#include <vector>

#include <boost/algorithm/string.hpp>
using namespace O2::Balancer;

DeviceSetting::DeviceSetting(int port, std::string ip){
    this->port = port;
    this->ip = ip;
}

DeviceSetting::DeviceSetting(const std::string& setting){
    std::vector<std::string> x;
    boost::split(x, setting, boost::is_any_of(":"));
    this->ip = x[0];
    this->port = std::stoi(x[1]);

}

