project(SimpleXIP)
cmake_minimum_required(VERSION 2.8.0)

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
# Qt stuff
#
find_package(QT4 REQUIRED)
SET(QT_USE_QTOPENGL ON)
INCLUDE(${QT_USE_FILE})

#--------------------------------------------------------------------
#--------------------------------------------------------------------
# Our stuff
#

#--------------------------------------------------------------------
# Define the header files
#
set(HEADERS
  qCtkXipSGWidget.h
  window.h
)
set(moc_sources
  qCtkXipSGWidget.h
  window.h
)

#--------------------------------------------------------------------
# Define the source files
#
set(SOURCES
  qCtkXipSGWidget.cpp
  window.cpp
  main.cpp
)

#--------------------------------------------------------------------
# build and link
#
QT4_WRAP_CPP(gen_moc_sources ${moc_sources})

add_executable(SimpleXIP ${HEADERS} ${SOURCES} ${gen_moc_sources})

TARGET_LINK_LIBRARIES(SimpleXIP ${QT_LIBRARIES} ${QT_QTOPENGL_LIBRARY} optimized xipcommon debug xipcommond optimized xipivcore debug xipivcored)

