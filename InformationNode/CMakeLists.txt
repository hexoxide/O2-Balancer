PROJECT(InformationNode)

IF( CMAKE_COMPILER_IS_GNUCC )
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
ENDIF ( CMAKE_COMPILER_IS_GNUCC )

SET(
    INFORMATION_NODE_SOURCES
    ./src/Main.cxx
    ./src/InformationDevice.cxx
    ./src/HeartbeatConnection.cxx
    ./src/AcknowledgeConnection.cxx
    ./src/InfoSettings.cxx
)

SET(
        INFORMATION_NODE_HEADERS
        ./include/O2/InformationNode/AcknowledgeConnection.h
        ./include/O2/InformationNode/HeartbeatConnection.h
        ./include/O2/InformationNode/InformationDevice.h
        ./include/O2/InformationNode/InfoSettings.h
)

GET_DIRECTORY_PROPERTY(
    O2Balancer_INCLUDE_DIRS 
    DIRECTORY ${CMAKE_SOURCE_DIR}/O2Balancer/ 
    DEFINITION O2Balancer_INCLUDE_DIRS  
)

SET(
    INFORMATION_LIB_NODE_HEADERS
    ./include
    ${O2Balancer_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${INFORMATION_LIB_NODE_HEADERS} )
ADD_EXECUTABLE(InformationNode ${INFORMATION_NODE_SOURCES} ${INFORMATION_NODE_HEADERS})

TARGET_LINK_LIBRARIES(
    InformationNode
    O2Balancer
)
