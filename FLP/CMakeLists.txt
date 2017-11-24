PROJECT(FLP)

SET(
        FLP_SOURCES
        ./src/FLPDevice.cxx
        ./src/Main.cxx
        ./src/HeartBeatConnection.cxx
        ./src/EPNConnection.cxx
        ./src/FLPSettings.cxx
        ./src/Utils.cxx
)

SET(
        FLP_HEADERS
        ./include/O2/FLP/EPNConnection.h
        ./include/O2/FLP/FLPDevice.h
        ./include/O2/FLP/FLPSettings.h
        ./include/O2/FLP/HeartBeatConnection.h
        ./include/O2/FLP/Utils.h
        ./include/O2/FLP/Globals.h
)

GET_DIRECTORY_PROPERTY(
    O2Balancer_INCLUDE_DIRS 
    DIRECTORY ${CMAKE_SOURCE_DIR}/O2Balancer/ 
    DEFINITION O2Balancer_INCLUDE_DIRS  
)

SET(
    FLP_INCLUDE_HEADERS
    ./include
    ${O2Balancer_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${FLP_INCLUDE_HEADERS} )
ADD_EXECUTABLE(flp ${FLP_SOURCES} ${FLP_HEADERS})

IF( CMAKE_COMPILER_IS_GNUCC )
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -g")
ENDIF ( CMAKE_COMPILER_IS_GNUCC )


TARGET_LINK_LIBRARIES(
    flp
    O2Balancer
)
