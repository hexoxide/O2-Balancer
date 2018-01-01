#define BOOST_TEST_DYN_LINK
//#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>
#include <O2/Balancer/Utilities/DataTypes.h>
#include <O2/Balancer/Exceptions/InitException.h>
#include <O2/Balancer/Exceptions/UnimplementedException.h>
#include <boost/format.hpp>

using O2::Balancer::DeviceSetting;
BOOST_AUTO_TEST_SUITE(Data_tests)
BOOST_AUTO_TEST_CASE(TestDeviceSettingCorrectParser) {
    const std::string ip = "127.0.0.2.02";
    const int port = 3000;
    DeviceSetting setting((boost::format("%s:%i") % ip % port).str());

    BOOST_CHECK_EQUAL(setting.ip,ip);
    BOOST_CHECK_EQUAL(setting.port,port);
}

BOOST_AUTO_TEST_CASE(TestDeviceSettingConstructor) {
    const std::string ip = "1221.2.12.21";
    const int port = 3000;
    DeviceSetting setting(port,ip);
    BOOST_CHECK_EQUAL(port,setting.port);
    BOOST_CHECK_EQUAL(ip, setting.ip);
}

BOOST_AUTO_TEST_CASE(TestIncorrectSettingConstructor) {
    try {
        const int port = 3000;
        DeviceSetting(port, ""); // Empty string is invalid.
    } catch (const O2::Balancer::Exceptions::UnimplementedException& ex ) {
        // yeey
        return;
    }
    BOOST_FAIL("Constructor should call an unimplemented exception");
}

BOOST_AUTO_TEST_CASE(TestDeviceSettingInvalidParser) {
        try {
            DeviceSetting setting("1asdfadsfadfaf");
        } catch(const O2::Balancer::Exceptions::InitException& ex) {
            return;
        }
        BOOST_FAIL("This constructor should call an initexception");
    }

BOOST_AUTO_TEST_SUITE_END()