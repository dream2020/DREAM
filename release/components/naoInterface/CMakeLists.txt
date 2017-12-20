CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME naoInterface)
PROJECT(${MODULENAME})

find_package(qibuild)

macro(get_WIN32_WINNT version)
    if (WIN32 AND CMAKE_SYSTEM_VERSION)
        set(ver ${CMAKE_SYSTEM_VERSION})
        string(REPLACE "." "" ver ${ver})
        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

        set(${version} "0x${ver}")
    endif()
endmacro()

get_WIN32_WINNT(ver)
add_definitions(-D_WIN32_WINNT=${ver})

FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/naoInterfaceMain.cpp src/naoInterfaceConfiguration.cpp src/naoInterfaceComputation.cpp src/naoInterfaceYarpInterface.cpp src/naoInterfaceController.cpp src/naoInterfaceModule.cpp)
FILE(GLOB folder_header src/naoInterface.h src/naoInterfaceYarpInterface.h src/naoInterfaceController.h src/naoInterfaceModule.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS} ${ALCOMMON_INCLUDE_DIRS}  ${ALPROXIES_INCLUDE_DIRS})

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 qi_use_lib(${MODULENAME} ALCOMMON ALPROXIES)
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 

