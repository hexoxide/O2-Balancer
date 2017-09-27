#define BOOST_TEST_MODULE "Testing settings"
#include <boost/test/unit_test.hpp>
#include <O2/Balancer/SettingsManager.h>
#include <O2/Balancer/DeviceSetting.h>
using namespace O2::Balancer;

BOOST_AUTO_TEST_CASE(testingSingleton){
    BOOST_CHECK(SettingsManager::getInstance() != nullptr);
}

BOOST_AUTO_TEST_CASE(testDeviceSettings){
    DeviceSetting setting(1,"test");
    BOOST_CHECK(setting.port == 1);
    BOOST_CHECK(setting.ip == "test");
}
