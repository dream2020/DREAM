CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME reactiveSystemGUI)
PROJECT(${MODULENAME})

FIND_PACKAGE(YARP)
FIND_PACKAGE(GUIUTILITIES REQUIRED)

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS} ${GUIUTILITIES_INCLUDE_DIRS})

SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/reactiveSystemGUIMain.cpp src/reactiveSystemGUIConfiguration.cpp src/reactiveSystemGUIComputation.cpp)
FILE(GLOB folder_header src/reactiveSystemGUI.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES}  ${GUIUTILITIES_LIBRARIES} ${FLTK_LIBRARIES} )

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 