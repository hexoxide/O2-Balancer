#include "AliceO2/EPNDevice.h"
#include <runFairMQDevice.h>

namespace bpo = boost::program_options;

void addCustomOptions(bpo::options_description& options){
    options.add_options()
        ("buffer-timeout", bpo::value<int>()->default_value(1000), "Buffer timeout in milliseconds")
        ("num-flps", bpo::value<int>()->required(), "Number of FLPs")
        ("test-mode", bpo::value<int>()->default_value(0), "Run in test mode")
        ("in-chan-name", bpo::value<std::string>()->default_value("stf2"), "Name of the input channel (sub-time frames)")
        ("out-chan-name", bpo::value<std::string>()->default_value("tf"), "Name of the output channel (time frames)")
        ("ack-chan-name", bpo::value<std::string>()->default_value("ack"), "Name of the acknowledgement channel");
}

FairMQDevicePtr getDevice(const FairMQProgOptions& /*config*/){
    return new AliceO2::EPN::EPNDevice();
}
