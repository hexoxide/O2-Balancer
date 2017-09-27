#include "O2/EPN/EPNDevice.h"
#include <O2/Balancer/DeviceManager.h>
#include <O2/Balancer/ProgramOptions.h>

namespace po = boost::program_options;

int main(int argc, char** argv){
    using namespace O2;
    using namespace O2::EPN;
    
    po::options_description options("EPN options");
    constexpr char CONFIG_FILE[] = "epn-config";


    options.add_options()
    (CONFIG_FILE, po::value<std::string>()->default_value("./epn.yaml"), "Configuration file");
    auto vm = Balancer::AddO2Options(options, argc, argv);


    Balancer::DeviceManager<EPNDevice> device;
    device.run();
    return EXIT_SUCCESS;
}

/*#include <runFairMQDevice.h>

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

FairMQDevicePtr getDevice(const FairMQProgOptions&){
    return new AliceO2::EPN::EPNDevice();
}
*/