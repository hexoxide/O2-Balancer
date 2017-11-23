CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(AliceO2_Balancer)

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    # Check if you're in the root directory of the project, and give an error.
    MESSAGE(FATAL_ERROR "Please create a dedicated build directory for this load balancer. (You may need remove the CmakeCache.txt and the cache)")
ENDIF()

SET(AliceO2_Balancer_MAJOR_VERSION 0)
SET(AliceO2_Balancer_MINOR_VERSION 5)
SET(AliceO2_Balancer_PATCH_VERSION 0)

SET(SIMPATH $ENV{SIMPATH})
SET(FAIRROOTPATH $ENV{FAIRROOTPATH})

#Use the same boost as FairSoft...

SET(CMAKE_CXX_STANDARD 11)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/
SET(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules" ${CMAKE_MODULE_PATH})
SET(CMAKE_MODULE_PATH "${FAIRROOTPATH}/share/fairbase/cmake/modules_old" ${CMAKE_MODULE_PATH})
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
SET(BOOST_ROOT $ENV{SIMPATH})

SET(LINK_DIRECTORIES
    ${FAIRROOT_LIBRARY_DIR}
)


FIND_PACKAGE(Doxygen)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
FIND_PACKAGE(Zookeeper REQUIRED)


LINK_DIRECTORIES( ${LINK_DIRECTORIES})
FIND_PACKAGE(FairRoot REQUIRED)

FIND_LIBRARY(YAML_CPP REQUIRED)


FIND_PACKAGE(Threads REQUIRED)

FIND_PACKAGE(
    Boost 1.59 COMPONENTS 
    system 
    timer 
    program_options 
    filesystem 
    chrono 
    exception 
    serialization 
    log 
    log_setup 
    unit_test_framework 
    date_time 
    REQUIRED
)

ENABLE_TESTING()

ADD_SUBDIRECTORY(Docs)
ADD_SUBDIRECTORY(O2Balancer)
ADD_SUBDIRECTORY(EPN)
ADD_SUBDIRECTORY(FLP)
ADD_SUBDIRECTORY(InformationNode)
ADD_SUBDIRECTORY(Scripts)