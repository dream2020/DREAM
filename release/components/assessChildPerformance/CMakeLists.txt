CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME assessChildPerformance)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/assessChildPerformanceMain.cpp src/assessChildPerformanceConfiguration.cpp src/assessChildPerformanceComputation.cpp src/assessChildPerformanceYarpInterface.cpp src/assessChildPerformanceController.cpp)
FILE(GLOB folder_header src/assessChildPerformance.h src/assessChildPerformanceYarpInterface.h src/assessChildPerformanceController.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

