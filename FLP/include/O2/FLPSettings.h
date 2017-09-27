#ifndef O2_FLP_FLP_SETTINGS_H
#define O2_FLP_FLP_SETTINGS_H

#include <string>
#include <memory>
#include <vector>
namespace O2{
    namespace FLP{

        struct DeviceSetting{
            int port;
            std::string ip;
            DeviceSetting(int port, std::string ip);
        };

        class FLPSettings{
            std::shared_ptr<DeviceSetting> informationSettings;
            std::vector<std::shared_ptr<DeviceSetting>> epnSettings;
        public:
            FLPSettings(const std::string fileName);
            std::shared_ptr<DeviceSetting> getInformationNodeSetting() const;
            std::vector<std::shared_ptr<DeviceSetting>> getEPNSettings() const;
        };
    }
}

#endif