CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(AliceO2Lib)

SET(header_version_in ${AliceO2Lib_SOURCE_DIR}/cmake/AliceO2.h.in)
SET(header_version_out ${CMAKE_CURRENT_BINARY_DIR}/include/AliceO2/AliceO2.h)



CONFIGURE_FILE(${header_version_in} ${header_version_out})

SET(AliceO2Lib_INCLUDE_DIRS 
    ${AliceO2Lib_SOURCE_DIR}/include/  
    ${CMAKE_CURRENT_BINARY_DIR}/include/
    ${FAIRROOT_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

SET(
    ALICEO2_LIB_SRC
    ./src/test.cxx
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
)