PROJECT(FLP)

SET(
    FLP_SOURCES
    ./src/FLPDevice.cxx
    ./src/Main.cxx
    ./src/HeartBeatConnection.cxx
    ./src/EPNConnection.cxx
    ./src/FLPSettings.cxx
)

GET_DIRECTORY_PROPERTY(
    O2Balancer_INCLUDE_DIRS 
    DIRECTORY ${CMAKE_SOURCE_DIR}/O2Balancer/ 
    DEFINITION O2Balancer_INCLUDE_DIRS  
)

SET(
    FLP_HEADERS
    ./include
    ${O2Balancer_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${FLP_HEADERS} )
ADD_EXECUTABLE(flp ${FLP_SOURCES})

TARGET_LINK_LIBRARIES(
    flp
    O2Balancer
)
