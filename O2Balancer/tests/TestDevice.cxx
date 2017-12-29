//
// Created by hheijden on 12/24/17.
//
#define BOOST_TEST_DYN_LINK
//#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>
#include <O2/Balancer/Devices/AbstractDevice.h>
#include <O2/Balancer/Utilities/Settings.h>
#include <O2/Balancer/Utilities/DataTypes.h>
#include <O2/Balancer/Exceptions/UnimplementedException.h>
#include <iostream>
BOOST_AUTO_TEST_SUITE(Connection_tests)
using O2::Balancer::AbstractDevice;
using O2::Balancer::Settings;
using O2::Balancer::DeviceSetting;
using O2::Balancer::Exceptions::UnimplementedException;
class TestSettings : public Settings {
public:
    TestSettings(){
        this->informationSettings = std::shared_ptr<DeviceSetting>(new DeviceSetting(1,"localhost"));
        this->settingsServer = std::shared_ptr<DeviceSetting>(new DeviceSetting(1, "localhost"));
        this->ipAddress = "localhost";
    }
    std::string getSettingsFile() const override {
        return std::string("");
    }
};
class TestDevice : public AbstractDevice {
    bool init;
public:
    TestDevice(bool init) : AbstractDevice("TestDevice", std::shared_ptr<Settings>(new TestSettings()),init){
        this->init = init;
    }
    void refreshDevice(bool inMainThread) override {
        if(inMainThread != init){
            throw UnimplementedException("Shouldn't happen!");
        }
    }
};


BOOST_AUTO_TEST_CASE(BasicTest){
        TestDevice device(true);
        device.restartDevice();
        device.ChangeState(TestDevice::STOP);

        device.ChangeState(TestDevice::RESET_TASK);
        device.WaitForEndOfState(TestDevice::RESET_TASK);

        device.ChangeState(TestDevice::RESET_DEVICE);
        device.WaitForEndOfState(TestDevice::RESET_DEVICE);
        device.ChangeState(TestDevice::END);
        device.quit();
}


BOOST_AUTO_TEST_SUITE_END()