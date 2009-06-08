include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/./Fdo
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged
	${CMAKE_CURRENT_SOURCE_DIR}/../../../GenericRdbms/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/Common/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../
	${CMAKE_CURRENT_SOURCE_DIR}/../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Src/Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/Common/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../LockManager
	)

add_subdirectory( Fdo )
add_subdirectory( SchemaMgr )

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

set( FdoRdbmsODBC_STAT_SRCS
   Odbc.cpp
   stdafx.cpp
)

add_library( FdoRdbmsODBC STATIC ${FdoRdbmsODBC_STAT_SRCS} )




#original Makefile.am contents follow:

###
### Copyright (C) 2004-2006  Autodesk, Inc.
### 
### This library is free software; you can redistribute it and/or
### modify it under the terms of version 2.1 of the GNU Lesser
### General Public License as published by the Free Software Foundation.
### 
### This library is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
### Lesser General Public License for more details.
### 
### You should have received a copy of the GNU Lesser General Public
### License along with this library; if not, write to the Free Software
### Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
###
#
#AUTOMAKE_OPTIONS = foreign
#
#SUBDIRS = Fdo SchemaMgr
#
#DIST_SUBDIRS = Fdo SchemaMgr
#
#EXTRA_DIST = Odbc.sln Odbc.vcproj build.bat resource.h Odbc.rc
#
#noinst_LTLIBRARIES = libFdoRdbmsODBC.la
#
#libFdoRdbmsODBC_la_SOURCES  = \
#  Odbc.cpp \
#  stdafx.cpp
#
#noinst_HEADERS = $(libFdoRdbmsODBC_la_SOURCES) \
#  stdafx.h
#
#libFdoRdbmsODBC_la_LDFLAGS = \
#  Fdo/libFdoRdbmsODBC_Fdo.la \
#  SchemaMgr/libFdoRdbmsODBC_SchemaMgr.la \
#  SchemaMgr/Lp/libFdoRdbmsODBC_SchemaMgr_Lp.la \
#  SchemaMgr/Ov/libFdoRdbmsODBC_SchemaMgr_Ov.la \
#  SchemaMgr/Ph/libFdoRdbmsODBC_SchemaMgr_Ph.la
#
#INCLUDES = \
#  -I./Fdo  \
#  -I$(FDO)/Unmanaged/Inc \
#  -I$(FDO)/Unmanaged \
#  -I../../../GenericRdbms/Inc \
#  -I$(FDOUTILITIES)/Common/Inc \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc \
#  -I../../Inc \
#  -I../../Inc/Rdbi \
#  -I../ \
#  -I../../ \
#  -I../../Inc \
#  -I../Fdo/Other \
#  -I../Fdo/Capability \
#  -I../Fdo/Connection \
#  -I../Fdo/Custom \
#  -I../Fdo/FeatureCommands \
#  -I../Fdo/Filter \
#  -I../Fdo/Lock \
#  -I../Fdo/Lt \
#  -I../Fdo/Schema \
#  -I../Fdo/SpatialContext \
#  -I$(FDO)/Unmanaged \
#  -I$(FDO)/Unmanaged/Inc \
#  -I$(FDO)/Unmanaged/Src/Geometry \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc \
#  -I$(FDOUTILITIES)/Common/Inc \
#  -I../Gdbi \
#  -I../LongTransactionManager \
#  -I../LockManager
#
#CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
#
