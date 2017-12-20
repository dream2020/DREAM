CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME actuationDriver)
PROJECT(${MODULENAME})

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
set(BOOST_USE_MULTITHREADED      ON)
set(BOOST_USE_DYNAMIC_LIBS       OFF)
set(BOOST_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE(BOOST 1.55)

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/actuationDriverMain.cpp src/actuationDriverConfiguration.cpp src/actuationDriverComputation.cpp src/actuationDriverYarpInterface.cpp src/actuationDriverController.cpp)
FILE(GLOB folder_header src/actuationDriver.h src/actuationDriverYarpInterface.h src/actuationDriverController.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS}  ${BOOST_INCLUDE_DIRS} )
link_directories(C:/NaoQi/lib)
LINK_DIRECTORIES(${BOOST_LIBRARY_DIRS} )

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES} ${BOOST_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

