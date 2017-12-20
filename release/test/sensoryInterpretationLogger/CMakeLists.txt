CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME sensoryInterpretationLogger)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/sensoryInterpretationLoggerMain.cpp src/sensoryInterpretationLoggerConfiguration.cpp src/sensoryInterpretationLoggerComputation.cpp src/SensoryInterpretationLogHandler.cpp src/sensoryInterpretationLoggerController.cpp src/yarpUtils.cpp)
FILE(GLOB folder_header src/sensoryInterpretationLogger.h src/SensoryInterpretationLogHandler.h src/sensoryInterpretationLoggerController.h  src/yarpUtils.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

