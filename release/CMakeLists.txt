# Copyright: (C) 2014 DREAM Consortium
# Authors: David Vernon
 
#Reduce warning level with cmake 2.6

IF(WIN32)
cmake_minimum_required(VERSION 2.6)
#disable annoying warnings about use of deprecated c functions
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
#removed MSVCRT from /NODEFAULTLIB to allow YARP to link
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libc;libcmt.lib;libcmtd.lib") 
ELSE(WIN32)
cmake_minimum_required(VERSION 2.4)
ENDIF(WIN32)

############################################## 
PROJECT(DREAM)
##############################################

SET(NESTED_BUILD TRUE)

SET(DREAM_DIR ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(YARP REQUIRED)

SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${YARP_DIR}/conf)

INCLUDE(${PROJECT_SOURCE_DIR}/DREAMConfig.cmake)

SET(FLTK_LIBRARIES "C:/FLTK/lib/fltk.lib" "C:/FLTK/lib/fltkgl.lib" "C:/FLTK/lib/fltkimages.lib" "C:/FLTK/lib/fltkpng.lib"  "C:/FLTK/lib/fltkzlib.lib") 

INCLUDE_DIRECTORIES("C:/FLTK" "${DREAM_DIR}/include")
LINK_DIRECTORIES("C:/NaoQi/lib")
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

SUBDIRS(components test tools)