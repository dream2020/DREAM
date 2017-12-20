CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME imageSource)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})
#FIND_PACKAGE(OPENCV)

FILE(GLOB folder_source src/imageSourceMain.cpp src/imageSourceConfiguration.cpp src/imageSourceComputation.cpp)
FILE(GLOB folder_header src/imageSource.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

 