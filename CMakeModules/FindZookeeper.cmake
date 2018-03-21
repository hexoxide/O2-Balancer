
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7 FATAL_ERROR)

INCLUDE(FindPackageHandleStandardArgs)
link_directories ("/usr/lib/arm-linux-gnueabihf/")
FIND_LIBRARY(ZOOKEEPER_LIBRARY zookeeper_mt)
FIND_PATH(ZOOKEEPER_INCLUDE_DIR "zookeeper/zookeeper.h")

SET(ZOOKEEPER_LIBRARIES ${ZOOKEEPER_LIBRARY})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Zookeeper
REQUIRED_ARGS ZOOKEEPER_INCLUDE_DIR ZOOKEEPER_LIBRARIES)
