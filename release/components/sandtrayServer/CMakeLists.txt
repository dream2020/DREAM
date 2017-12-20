CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME sandtrayServer)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/sandtrayServerMain.cpp src/sandtrayServerConfiguration.cpp src/sandtrayServerComputation.cpp src/sandtrayServerYarpInterface.cpp src/sandtrayServerController.cpp)
FILE(GLOB folder_header src/sandtrayServer.h src/sandtrayServerYarpInterface.h src/sandtrayServerController.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

