#include "O2/Balancer/Utilities/DeviceSetting.h"
#include <regex>
#include <iostream>
#include <boost/algorithm/string.hpp>
using namespace O2::Balancer;

DeviceSetting::DeviceSetting(int port, std::string ip){
    this->port = port;
    this->ip = ip;
}

DeviceSetting::DeviceSetting(const std::string& setting){
    try{
        const std::regex parseSetting("([^:]+):([0-9]+)", std::regex_constants::ECMAScript);
	    std::smatch match;
    	std::regex_match(setting, match, parseSetting);
    	this->ip = match[1];
    	this->port = std::stoi(match[2]);
    } catch(const std::regex_error& error){
        std::vector<std::string> x;
        boost::split(x, setting, boost::is_any_of(":"));
        this->ip = x[0];
        this->port = std::stoi(x[1]);
    }
}

