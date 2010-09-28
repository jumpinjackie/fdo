# - Try to find MySQL / MySQL Embedded library
# Find the MySQL includes and client library
# This module defines
#  ODBC_INCLUDE_DIR, where to find mysql.h
#  ODBC_LIBRARIES, the libraries needed to use MySQL.
#  ODBC_FOUND, If false, do not try to use MySQL.

# Copyright (c) 2009, Helio Chissini de Castro <helio@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CheckCXXSourceCompiles)
include(MacroPushRequiredVars)

if(UNIX)
	find_program(ODBC_CONFIG odbc_config
		HINTS
		    $ENV{ODBC_DIR}
		PATH_SUFFIXES 
			bin
		PATHS
			/usr/local/odbc
	)
	
	if(ODBC_CONFIG)
		exec_program(${ODBC_CONFIG} ARGS --libs OUTPUT_VARIABLE ODBC_CONFIG_LIBS)
		exec_program(${ODBC_CONFIG} ARGS --cflags OUTPUT_VARIABLE ODBC_CONFIG_CFLAGS)
		
		if(ODBC_CONFIG_LIBS)
			string(REGEX MATCHALL "-l[^ ]+" _config_lib ${ODBC_CONFIG_LIBS})
			string(REGEX REPLACE "-l" "" _odbc_lib "${_config_lib}")
			string(REGEX MATCHALL "-L[^ ]+"	_config_libpath	${ODBC_CONFIG_LIBS})
			string(REGEX REPLACE  "-L" "" _odbc_libpath "${_config_libpath}")
		endif(ODBC_CONFIG_LIBS)
		if(ODBC_CONFIG_CFLAGS)
			string(REGEX MATCHALL "-I[^ ]+" _config_inc ${ODBC_CONFIG_CFLAGS})
			string(REGEX REPLACE "-I" "" _odbc_inc "${_config_lib}")
			string(REGEX MATCHALL "-D[^ ]+"	_odbc_def ${ODBC_CONFIG_CFLAGS})
		endif(ODBC_CONFIG_CFLAGS)
	endif(ODBC_CONFIG)
endif(UNIX)

find_path(ODBC_INCLUDE_DIR sql.h
	HINTS
      $ENV{ODBC_INCLUDE_DIR}
      $ENV{ODBC_DIR}/include
	PATHS_SUFFIXES
	  include/odbc
	  odbc
    PATHS
	  ${_odbc_inc}
      /usr/include/odbc
)

find_library(ODBC_LIBRARY
	NAMES ${_odbc_lib} odbc
	PATH_SUFFIXES lib lib64
	PATHS ${_odbc_libpath} lib lib64
	)

find_library(ODBCINST_LIBRARY
	NAMES odbcinst
	PATH_SUFFIXES lib lib64
	PATHS ${_odbc_libpath} lib lib64
	)

set(ODBC_DEFINITIONS ${_odbc_def}) 

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODBC ODBC_INCLUDE_DIR ODBC_LIBRARY ODBCINST_LIBRARY ODBC_DEFINITIONS)
