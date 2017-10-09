#ifndef O2_BALANCER_UTILITIES_SETTINGS_H
#define O2_BALANCER_UTILITIES_SETTINGS_H

#include <boost/program_options.hpp>
#include <memory>
#include <string>
#include <vector>

namespace YAML{
    class Node;
}

namespace O2{

    
    namespace Balancer{
        class DeviceSetting;
        
        class Settings{
        private:
            std::shared_ptr<DeviceSetting> informationSettings;
            std::string ipAddress;
        protected:
            virtual std::string getSettingsFile() const = 0;
            YAML::Node load(const boost::program_options::variables_map& settings);
        public:
            std::shared_ptr<DeviceSetting> getInformationNodeSetting() const;
            std::string getIPAddress() const;
            Settings();

        };
    }
}

#endif