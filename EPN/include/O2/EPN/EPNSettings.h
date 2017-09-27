#ifndef O2_EPN_EPNSETTINGS_H
#define O2_EPN_EPNSETTINGS_H

#include <string>
#include <memory>
#include <vector>
#include <O2/Balancer/DeviceSetting.h>


namespace O2{
    namespace EPN{
        class EPNSettings{
            std::shared_ptr<Balancer::DeviceSetting> informationSettings;

        public:
            EPNSettings(const std::string& configFile);
            std::shared_ptr<Balancer::DeviceSetting> getInformationNodeSetting() const;
        };
    }
}

#endif