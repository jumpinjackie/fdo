include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../GenericRdbms/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/Common/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc/Rdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Inc
	${CMAKE_CURRENT_SOURCE_DIR}/.././Other
	${CMAKE_CURRENT_SOURCE_DIR}/.././Capability
	${CMAKE_CURRENT_SOURCE_DIR}/.././Connection
	${CMAKE_CURRENT_SOURCE_DIR}/.././Custom
	${CMAKE_CURRENT_SOURCE_DIR}/.././FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/.././Filter
	${CMAKE_CURRENT_SOURCE_DIR}/.././Lock
	${CMAKE_CURRENT_SOURCE_DIR}/.././Lt
	${CMAKE_CURRENT_SOURCE_DIR}/.././Schema
	${CMAKE_CURRENT_SOURCE_DIR}/.././SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/src/geometry
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/Common/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/ExpressionEngine/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../LockManager
	)

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

set( FdoRdbms_Capability_STAT_SRCS
   FdoRdbmsCommandCapabilities.cpp
   FdoRdbmsFilterCapabilities.cpp
   FdoRdbmsConnectionCapabilities.cpp
   FdoRdbmsSchemaCapabilities.cpp
   FdoRdbmsExpressionCapabilities.cpp
   FdoRdbmsTopologyCapabilities.cpp
)

add_library( FdoRdbms_Capability STATIC ${FdoRdbms_Capability_STAT_SRCS} )




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
#noinst_LTLIBRARIES = libFdoRdbms_Capability.la
#
#libFdoRdbms_Capability_la_SOURCES = \
#  FdoRdbmsCommandCapabilities.cpp \
#  FdoRdbmsFilterCapabilities.cpp \
#  FdoRdbmsConnectionCapabilities.cpp \
#  FdoRdbmsSchemaCapabilities.cpp \
#  FdoRdbmsExpressionCapabilities.cpp \
#  FdoRdbmsTopologyCapabilities.cpp
#
#noinst_HEADERS = $(libFdoRdbms_Capability_la_SOURCES) \
#  FdoRdbmsCommandCapabilities.h \
#  FdoRdbmsConnectionCapabilities.h \
#  FdoRdbmsExpressionCapabilities.h \
#  FdoRdbmsFilterCapabilities.h \
#  FdoRdbmsGeometryCapabilities.h \
#  FdoRdbmsSchemaCapabilities.h \
#  FdoRdbmsTopologyCapabilities.h
#
#INCLUDES = \
#  -I$(FDO)/Unmanaged/Inc \
#  -I$(FDO)/Unmanaged \
#  -I../../../../GenericRdbms/Inc \
#  -I$(FDOUTILITIES)/Common/Inc \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc \
#  -I../../../Inc \
#  -I../../../Inc/Rdbi \
#  -I../../ \
#  -I../../../ \
#  -I../../../Inc \
#  -I.././Other \
#  -I.././Capability \
#  -I.././Connection \
#  -I.././Custom \
#  -I.././FeatureCommands \
#  -I.././Filter \
#  -I.././Lock \
#  -I.././Lt \
#  -I.././Schema \
#  -I.././SpatialContext \
#  -I$(FDO)/Unmanaged \
#  -I$(FDO)/Unmanaged/Inc \
#  -I$(FDO)/Unmanaged/src/geometry \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc \
#  -I$(FDOUTILITIES)/Common/Inc \
#  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
#  -I../../Gdbi \
#  -I../../LongTransactionManager \
#  -I../../LockManager
#
#libFdoRdbms_Capability_la_LDFLAGS =
#
#CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
#
