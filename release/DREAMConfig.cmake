# If DREAM_FOUND is set, we have already done all this
IF (NOT DREAM_FOUND)

#Changing the default CMAKE_INSTALL_PREFIX
SET(CMAKE_INSTALL_PREFIX
  "${DREAM_DIR}" CACHE PATH "DREAM install prefix" FORCE
)

# Configuration file for the DREAM module collection.

# pick up DREAM's helper modules
SET(DREAM_MODULE_PATH "${DREAM_DIR}/components")
SET(DREAM_SOURCE_DIR ${DREAM_DIR})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${DREAM_MODULE_PATH})
 
# some conveniences
INCLUDE(UsePkgConfig)

# paths to libraries
SET(GUIUTILITIES_DIR "${DREAM_DIR}/components/guiUtilities") 

ENDIF (NOT DREAM_FOUND)
