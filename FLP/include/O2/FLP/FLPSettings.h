#ifndef O2_FLP_FLP_SETTINGS_H
#define O2_FLP_FLP_SETTINGS_H

#include <memory>
#include <vector>
#include <O2/Balancer/DeviceSetting.h>

namespace O2{
    namespace FLP{
        class FLPSettings{
            std::shared_ptr<Balancer::DeviceSetting> informationSettings;
            std::vector<std::shared_ptr<Balancer::DeviceSetting>> epnSettings;
        public:
            FLPSettings(const std::string fileName);
            std::shared_ptr<Balancer::DeviceSetting> getInformationNodeSetting() const;
            std::vector<std::shared_ptr<Balancer::DeviceSetting>> getEPNSettings() const;
        };
    }
}

#endif