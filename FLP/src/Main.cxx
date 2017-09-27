#include "O2/FLP/FLPDevice.h"
#include <O2/Balancer/DeviceManager.h>
#include <O2/Balancer/ProgramOptions.h>
#include "O2/FLP/FLPSettings.h"

namespace po = boost::program_options;


int main(int argc, char** argv){
    using namespace O2;
    using namespace O2::FLP;
    po::options_description options("FLP options");
    constexpr char CONFIG_FILE[] = "flp-config";

    options.add_options()
    (CONFIG_FILE, po::value<std::string>()->default_value("./flp.yaml"), "Configuration file");
    auto vm = Balancer::AddO2Options(options, argc, argv);
    //Load the settings
    FLPSettings settings( vm[CONFIG_FILE].as<std::string>());
    try{
        Balancer::DeviceManager<FLPDevice> manager(
            settings
        );
        manager.run();
    } catch (O2::Balancer::Exceptions::AbstractException exception){
        LOG(ERROR) << exception.getMessage();
        return EXIT_FAILURE;
    }

}

