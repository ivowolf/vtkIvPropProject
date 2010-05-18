PROJECT (vtkIvProp)
cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------------------
# VTK stuff
#
FIND_PACKAGE(VTK REQUIRED)
IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "Example ${PROJECT_NAME} requires VTK_USE_RENDERING.")
ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})

#--------------------------------------------------------------------
# XIP stuff
#
SET(XIP_PATH "V:/windows/x32/xip-0.2.2-install" CACHE PATH "XIP Path")
INCLUDE_DIRECTORIES(${XIP_PATH}/include)
LINK_DIRECTORIES(${XIP_PATH}/lib)
option(DEBUG_POSTFIX "If this option is enabled, a \"d\" will be added as debug postfix." ON)
IF(DEBUG_POSTFIX)
  add_definitions(-D_DEBUG_POSTFIX)
ENDIF(DEBUG_POSTFIX)


#--------------------------------------------------------------------
#--------------------------------------------------------------------
# Our stuff
#
ADD_EXECUTABLE(vtkIvPropDemo Cone.cxx vtkIvProp.h vtkIvProp.cpp)
TARGET_LINK_LIBRARIES(vtkIvPropDemo vtkRendering)
