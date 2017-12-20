CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME wp5Stub)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/wp5StubMain.cpp src/wp5StubConfiguration.cpp src/wp5StubComputation.cpp src/wp5StubYarpInterface.cpp src/wp5StubController.cpp)
FILE(GLOB folder_header src/wp5Stub.h src/wp5StubYarpInterface.h src/wp5StubController.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

