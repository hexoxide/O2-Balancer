PROJECT(EPN)

SET(
    EPN_SOURCES
    ./src/EPNDevice.cxx
    ./src/EPNSettings.cxx
    ./src/Main.cxx
    ./src/AcknowledgeConnection.cxx
    ./src/FLPConnection.cxx
    ./src/OutputConnection.cxx
)

GET_DIRECTORY_PROPERTY(
    O2Balancer_INCLUDE_DIRS 
    DIRECTORY ${CMAKE_SOURCE_DIR}/O2Balancer/ 
    DEFINITION O2Balancer_INCLUDE_DIRS  
)

SET(
    EPN_HEADERS
    ./include
    ${O2Balancer_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${EPN_HEADERS})

ADD_EXECUTABLE(epn ${EPN_SOURCES})

TARGET_LINK_LIBRARIES(
    epn
    O2Balancer
)
