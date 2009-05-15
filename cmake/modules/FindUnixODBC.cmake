# - Try to find MySQL / MySQL Embedded library
# Find the MySQL includes and client library
# This module defines
#  ODBC_INCLUDE_DIR, where to find mysql.h
#  ODBC_LIBRARIES, the libraries needed to use MySQL.
#  ODBC_FOUND, If false, do not try to use MySQL.

# Copyright (c) 2006-2008, Jarosław Staniek <staniek@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CheckCXXSourceCompiles)
include(MacroPushRequiredVars)

if(WIN32)
   find_path(ODBC_INCLUDE_DIR sql.h
      PATHS
      $ENV{ODBC_INCLUDE_DIR}
      $ENV{ODBC_DIR}/include
	  #$ENV{ProgramFiles}/MySQL/*/include
	  #$ENV{SystemDrive}/MySQL/*/include
   )
else(WIN32)
   find_path(ODBC_INCLUDE_DIR sql.h
      PATHS
      $ENV{ODBC_INCLUDE_DIR}
      $ENV{ODBC_DIR}/include
      /usr/local/mysql/include
      /opt/mysql/mysql/include
      PATH_SUFFIXES
      mysql
   )
endif(WIN32)

if(WIN32)
   # path suffix for debug/release mode
   # binary_dist: mysql binary distribution
   # build_dist: custom build
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(binary_dist debug)
      set(build_dist Debug)
   else(CMAKE_BUILD_TYPE STREQUAL "Debug")
      ADD_DEFINITIONS(-DDBUG_OFF)
      set(binary_dist opt)
      set(build_dist Release)
   endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
   
   find_library(ODBC_LIBRARIES NAMES libmysql
      PATHS
      $ENV{ODBC_DIR}/lib/${binary_dist}
      $ENV{ODBC_DIR}/libmysql/${build_dist}
      $ENV{ODBC_DIR}/client/${build_dist}
	  #$ENV{ProgramFiles}/MySQL/*/lib/${binary_dist}
	  #$ENV{SystemDrive}/MySQL/*/lib/${binary_dist}
   )
else(WIN32)
   find_library(ODBC_LIBRARIES NAMES odbc
      PATHS
      $ENV{ODBC_DIR}/lib
      $ENV{ODBC_DIR}/lib64
      PATH_SUFFIXES 
      odbc
   )
endif(WIN32)

if(ODBC_LIBRARIES)
   get_filename_component(ODBC_LIB_DIR ${ODBC_LIBRARIES} PATH)
endif(ODBC_LIBRARIES)

macro_push_required_vars()
set( CMAKE_REQUIRED_INCLUDES ${ODBC_INCLUDE_DIR} )
check_cxx_source_compiles( "#include <mysql.h>\nint main() { int i = ODBC_OPT_USE_EMBEDDED_CONNECTION; }" HAVE_ODBC_OPT_EMBEDDED_CONNECTION )
macro_pop_required_vars()

if(ODBC_INCLUDE_DIR AND ODBC_LIBRARIES)
   set(ODBC_FOUND TRUE)
   message(STATUS "Found MySQL: ${ODBC_INCLUDE_DIR}, ${ODBC_LIBRARIES}")
else(ODBC_INCLUDE_DIR AND ODBC_LIBRARIES)
   set(ODBC_FOUND FALSE)
   message(STATUS "MySQL not found.")
endif(ODBC_INCLUDE_DIR AND ODBC_LIBRARIES)

if(ODBC_INCLUDE_DIR AND ODBC_EMBEDDED_LIBRARIES AND HAVE_ODBC_OPT_EMBEDDED_CONNECTION)
   set(ODBC_EMBEDDED_FOUND TRUE)
   message(STATUS "Found MySQL Embedded: ${ODBC_INCLUDE_DIR}, ${ODBC_EMBEDDED_LIBRARIES}")
else(ODBC_INCLUDE_DIR AND ODBC_EMBEDDED_LIBRARIES AND HAVE_ODBC_OPT_EMBEDDED_CONNECTION)
   set(ODBC_EMBEDDED_FOUND FALSE)
   message(STATUS "MySQL Embedded not found.")
endif(ODBC_INCLUDE_DIR AND ODBC_EMBEDDED_LIBRARIES AND HAVE_ODBC_OPT_EMBEDDED_CONNECTION)

mark_as_advanced(ODBC_INCLUDE_DIR ODBC_LIBRARIES ODBC_EMBEDDED_LIBRARIES)
