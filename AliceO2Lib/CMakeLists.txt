CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(AliceO2Lib)

SET(header_version_in ${AliceO2Lib_SOURCE_DIR}/cmake/O2.h.in)
SET(header_version_out ${CMAKE_CURRENT_BINARY_DIR}/include/O2/O2.h)

CONFIGURE_FILE(${header_version_in} ${header_version_out})

SET(AliceO2Lib_INCLUDE_DIRS 
    ${AliceO2Lib_SOURCE_DIR}/include  
    ${CMAKE_CURRENT_BINARY_DIR}/include/
    ${FAIRROOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

INCLUDE_DIRECTORIES(
    ${AliceO2Lib_INCLUDE_DIRS}
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
    ./src/exceptions/AbstractException.cxx
    ./src/exceptions/InitException.cxx
    ./src/exceptions/UnimplementedException.cxx
)

ADD_LIBRARY(
    AliceO2 SHARED 
    ${ALICEO2_LIB_SRC}
)

TARGET_LINK_LIBRARIES(
    AliceO2
    
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${ZeroMQ_LIBRARY}
    fairmq_logger
    FairMQ
    Threads::Threads
    dl
    yaml-cpp
)