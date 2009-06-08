include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../
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
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Other
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Capability
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Connection
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Custom
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/FeatureCommands
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Filter
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Lock
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Lt
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/Schema
	${CMAKE_CURRENT_SOURCE_DIR}/../../../Src/Fdo/SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDO)/Unmanaged/Src/Geometry
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/SchemaMgr/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/Common/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOUTILITIES)/ExpressionEngine/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/../../Gdbi
	${CMAKE_CURRENT_SOURCE_DIR}/../../LongTransactionManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../LockManager
	${CMAKE_CURRENT_SOURCE_DIR}/../../ODBCDriver
	${CMAKE_CURRENT_SOURCE_DIR}/$(FDOODBC)/include
	)

add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX )

set( FdoRdbmsODBC_Fdo_STAT_SRCS
   FdoRdbmsOdbcCommandCapabilities.cpp
   FdoRdbmsOdbcConnectionCapabilities.cpp
   FdoRdbmsOdbcConnection.cpp
   FdoRdbmsOdbcConnectionInfo.cpp
   FdoRdbmsOdbcExpressionCapabilities.cpp
   FdoRdbmsOdbcFilterCapabilities.cpp
   FdoRdbmsOdbcFilterProcessor.cpp
   FdoRdbmsOdbcSchemaCapabilities.cpp
   FdoRdbmsOdbcSpatialManager.cpp
   FdoRdbmsOdbcSpatialSqlFilter.cpp
)

add_library( FdoRdbmsODBC_Fdo STATIC ${FdoRdbmsODBC_Fdo_STAT_SRCS} )




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
#noinst_LTLIBRARIES = libFdoRdbmsODBC_Fdo.la
#
#libFdoRdbmsODBC_Fdo_la_SOURCES = \
#FdoRdbmsOdbcCommandCapabilities.cpp \
#FdoRdbmsOdbcConnectionCapabilities.cpp \
#FdoRdbmsOdbcConnection.cpp \
#FdoRdbmsOdbcConnectionInfo.cpp \
#FdoRdbmsOdbcExpressionCapabilities.cpp \
#FdoRdbmsOdbcFilterCapabilities.cpp \
#FdoRdbmsOdbcFilterProcessor.cpp \
#FdoRdbmsOdbcSchemaCapabilities.cpp \
#FdoRdbmsOdbcSpatialManager.cpp \
#FdoRdbmsOdbcSpatialSqlFilter.cpp
#
#noinst_HEADERS = $(libFdoRdbmsODBC_Fdo_la_SOURCES) \
#  FdoRdbmsOdbcCommandCapabilities.h \
#  FdoRdbmsOdbcConnection.h \
#  FdoRdbmsOdbcConnectionCapabilities.h \
#  FdoRdbmsOdbcConnectionInfo.h \
#  FdoRdbmsOdbcCreateDataStore.h \
#  FdoRdbmsOdbcDeleteDataStore.h \
#  FdoRdbmsOdbcExpressionCapabilities.h \
#  FdoRdbmsOdbcFilterCapabilities.h \
#  FdoRdbmsOdbcFilterProcessor.h \
#  FdoRdbmsOdbcGeometryCapabilities.h \
#  FdoRdbmsOdbcProviderInfo.h \
#  FdoRdbmsOdbcSchemaCapabilities.h \
#  FdoRdbmsOdbcSpatialManager.h \
#  FdoRdbmsOdbcSpatialSqlFilter.h
#
#INCLUDES = \
#  -I../ \
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
#  -I../../../Src/Fdo/Other \
#  -I../../../Src/Fdo/Capability \
#  -I../../../Src/Fdo/Connection \
#  -I../../../Src/Fdo/Custom \
#  -I../../../Src/Fdo/FeatureCommands \
#  -I../../../Src/Fdo/Filter \
#  -I../../../Src/Fdo/Lock \
#  -I../../../Src/Fdo/Lt \
#  -I../../../Src/Fdo/Schema \
#  -I../../../Src/Fdo/SpatialContext \
#  -I$(FDO)/Unmanaged \
#  -I$(FDO)/Unmanaged/Inc \
#  -I$(FDO)/Unmanaged/Src/Geometry \
#  -I$(FDOUTILITIES)/SchemaMgr/Inc \
#  -I$(FDOUTILITIES)/Common/Inc \
#  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
#  -I../../Gdbi \
#  -I../../LongTransactionManager \
#  -I../../LockManager \
#  -I../../ODBCDriver \
#  -I$(FDOODBC)/include
#
#
#libFdoRdbmsODBC_Fdo_la_LDFLAGS =
#
#CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX
#
#
#
