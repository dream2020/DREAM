CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(MODULENAME usbCameraSource)
PROJECT(${MODULENAME})

find_package(qibuild)
FIND_PACKAGE(YARP)
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FILE(GLOB folder_source src/usbCameraSourceMain.cpp src/usbCameraSourceConfiguration.cpp src/usbCameraSourceComputation.cpp)
FILE(GLOB folder_header src/usbCameraSource.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

set(OPENCV_release_LIBS opencv_calib3d243.lib opencv_contrib243.lib opencv_core243.lib opencv_features2d243.lib opencv_flann243.lib opencv_gpu243.lib opencv_highgui243.lib opencv_imgproc243.lib opencv_legacy243.lib opencv_ml243.lib opencv_objdetect243.lib opencv_ts243.lib opencv_video243.lib)
link_directories(C:/NaoQi/lib)
include_directories(C:/NaoQi/include)

ADD_EXECUTABLE(${MODULENAME} ${folder_source} ${folder_header})
 
TARGET_LINK_LIBRARIES(${MODULENAME} ${YARP_LIBRARIES} ${OPENCV_release_LIBS})

INSTALL(TARGETS ${MODULENAME} DESTINATION bin) 