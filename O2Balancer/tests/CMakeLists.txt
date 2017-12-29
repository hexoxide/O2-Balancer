PROJECT(O2Balancer_tests)


SET(
        O2Balancer_tests_sources
        ./TestMain.cxx

        ./TestExceptions.cxx
        ./TestDevice.cxx
)



ADD_EXECUTABLE(Tests ${O2Balancer_tests_sources})

TARGET_LINK_LIBRARIES(
    Tests
    O2Balancer
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

ADD_TEST(BalancerLib Tests)