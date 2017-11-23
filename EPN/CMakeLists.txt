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

SET(
        EPN_HEADERS
        ./include/O2/EPN/AcknowledgeConnection.h
        ./include/O2/EPN/EPNDevice.h
        ./include/O2/EPN/EPNSettings.h
        ./include/O2/EPN/FLPConnection.h
        ./include/O2/EPN/OutputConnection.h
)


GET_DIRECTORY_PROPERTY(
    O2Balancer_INCLUDE_DIRS 
    DIRECTORY ${CMAKE_SOURCE_DIR}/O2Balancer/ 
    DEFINITION O2Balancer_INCLUDE_DIRS  
)

SET(
    EPN_INCLUDE_HEADERS
    ./include
    ${O2Balancer_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${EPN_INCLUDE_HEADERS})

ADD_EXECUTABLE(epn ${EPN_SOURCES} ${EPN_HEADERS})

TARGET_LINK_LIBRARIES(
    epn
    O2Balancer
)
