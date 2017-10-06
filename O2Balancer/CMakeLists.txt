CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(O2Balancer)

SET(header_version_in ${O2Balancer_SOURCE_DIR}/cmake/Balancer.h.in)
SET(header_version_out ${CMAKE_CURRENT_BINARY_DIR}/include/O2/Balancer/Balancer.h)

CONFIGURE_FILE(${header_version_in} ${header_version_out})

SET(O2Balancer_INCLUDE_DIRS 
    ${O2Balancer_SOURCE_DIR}/include  
    ${CMAKE_CURRENT_BINARY_DIR}/include/
    ${FAIRROOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
    ${ZOOKEEPER_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
    ${O2Balancer_INCLUDE_DIRS}
)

IF( CMAKE_COMPILER_IS_GNUCC )
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
ENDIF ( CMAKE_COMPILER_IS_GNUCC )

SET(
    ALICEO2_LIB_SRC
    ./src/AbstractDevice.cxx
    ./src/Connection.cxx
    ./src/ResultManager.cxx
    ./src/SettingsManager.cxx
    ./src/ProgramOptions.cxx
    ./src/Exceptions/AbstractException.cxx
    ./src/Exceptions/InitException.cxx
    ./src/Exceptions/UnimplementedException.cxx
    ./src/DeviceSetting.cxx
    ./src/ClusterManager.cxx
    ./src/ClusterNodes.cxx
    ./src/Utilities.cxx
)

ADD_LIBRARY(
    O2Balancer SHARED 
    ${ALICEO2_LIB_SRC}
)

TARGET_LINK_LIBRARIES(
    O2Balancer
    
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${ZeroMQ_LIBRARY}
    ${ZOOKEEPER_LIBRARIES}
    fairmq_logger
    FairMQ
    Threads::Threads
    dl
    yaml-cpp
)

ADD_SUBDIRECTORY(tests)