##
## Copyright (C) 2004-2007  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = PostGisProvider.vcproj PostGisProvider.rc resource.h md5.hpp

noinst_LTLIBRARIES = libPostGISProviderSrc.la

libPostGISProviderSrc_la_SOURCES = \
ApplySchemaCommand.cpp \
FilterProcessor.cpp \
SchemaCapabilities.cpp \
CommandCapabilities.cpp \
GeometryCapabilities.cpp \
SchemaDescription.cpp \
ConnectionCapabilities.cpp \
GetSpatialContextsCommand.cpp \
SelectAggregatesCommand.cpp \
Connection.cpp \
InsertCommand.cpp \
SelectCommand.cpp \
ConnectionInfo.cpp \
ListDataStores.cpp \
SpatialContextCollection.cpp \
CreateDataStore.cpp \
md5.cpp \
SpatialContext.cpp \
DataReader.cpp \
PgCursor.cpp \
SpatialContextReader.cpp \
DataStoreReader.cpp \
PgGeometryColumn.cpp \
SQLCommand.cpp \
DeleteCommand.cpp \
PgGeometry.cpp \
SQLDataReader.cpp \
DescribeSchemaCommand.cpp \
PgSpatialRefSysReader.cpp \
DestroyDataStore.cpp \
PgTablesReader.cpp \
TopologyCapabilities.cpp \
ExpressionCapabilities.cpp \
PgTableColumnsReader.cpp \
Transaction.cpp \
ExpressionProcessor.cpp \
PgUtility.cpp \
UpdateCommand.cpp \
FeatureReader.cpp \
PostGisProvider.cpp \
FilterCapabilities.cpp \
RasterCapabilities.cpp

noinst_HEADERS = $(libPostGISProviderSrc_la_SOURCES) \
ApplySchemaCommand.h \
FilterCapabilities.h \
Reader.h \
CommandCapabilities.h \
FilterProcessor.h \
Command.h \
GeometryCapabilities.h \
SchemaCapabilities.h \
ConnectionCapabilities.h \
GetSpatialContextsCommand.h \
SchemaDescription.h \
Connection.h \
InsertCommand.h \
SelectAggregatesCommand.h \
ConnectionInfo.h \
ListDataStores.h \
SelectCommand.h \
CreateDataStore.h \
Logger.h \
SpatialContextCollection.h \
DataReader.h \
PgCursor.h \
SpatialContext.h \
DataStoreReader.h \
PgGeometryColumn.h \
SpatialContextReader.h \
DeleteCommand.h \
PgGeometry.h \
SQLCommand.h \
DescribeSchemaCommand.h \
PgSpatialRefSysReader.h \
SQLDataReader.h \
DestroyDataStore.h \
PgTablesReader.h \
stdafx.h \
ExpressionCapabilities.h \
PgTableColumnsReader.h \
TopologyCapabilities.h \
ExpressionProcessor.h \
PgUtility.h \
Transaction.h \
FeatureCommand.h \
PostGisProvider.h \
UpdateCommand.h \
FeatureReader.h \
RasterCapabilities.h

libPostGISProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../ \
  -I../../Inc \
  -I../Message/Inc \
  -I../../../../Fdo/Unmanaged/Inc \
  -I../../../../Utilities/Common/Inc \
  -I../../../../Thirdparty/pgsql/include \
  -I../../../../Thirdparty/boost

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32
