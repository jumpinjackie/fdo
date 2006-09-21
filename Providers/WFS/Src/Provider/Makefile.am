##
## Copyright (C) 2004-2006  Autodesk, Inc.
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

EXTRA_DIST = WFSProvider.vcproj FdoWfsProvider.rc

noinst_LTLIBRARIES = libWFSProviderSrc.la

libWFSProviderSrc_la_SOURCES = \
  ./FdoWfsCommandCapabilities.cpp \
  ./FdoWfsConnectionCapabilities.cpp \
  ./FdoWfsConnection.cpp \
  ./FdoWfsConnectionInfo.cpp \
  ./FdoWfsDataReader.cpp \
  ./FdoWfsDelegate.cpp \
  ./FdoWfsDescribeFeatureType.cpp \
  ./FdoWfsDescribeSchemaCommand.cpp \
  ./FdoWfsExpressionCapabilities.cpp \
  ./FdoWfsFeatureReader.cpp \
  ./FdoWfsFeatureType.cpp \
  ./FdoWfsFeatureTypeList.cpp \
  ./FdoWfsFilterCapabilities.cpp \
  ./FdoWfsGeometryCapabilities.cpp \
  ./FdoWfsGetFeature.cpp \
  ./FdoWfsGetSpatialContexts.cpp \
  ./FdoWfsOgcFilterCapabilities.cpp \
  ./FdoWfsOgcScalarCapabilities.cpp \
  ./FdoWfsOgcSpatialCapabilities.cpp \
  ./FdoWfsRasterCapabilities.cpp \
  ./FdoWfsSchemaCapabilities.cpp \
  ./FdoWfsSchemaCopyHandler.cpp \
  ./FdoWfsSchemaMerger.cpp \
  ./FdoWfsSchemaRefHandler.cpp \
  ./FdoWfsSelectAggregatesCommand.cpp \
  ./FdoWfsSelectCommand.cpp \
  ./FdoWfsServiceMetadata.cpp \
  ./FdoWfsSpatialContextReader.cpp \
  ./FdoWfsTopologyCapabilities.cpp \
  ./FdoWfsUtils.cpp \
  ./stdafx.cpp


noinst_HEADERS = $(libWFSProviderSrc_la_SOURCES) \
  ./FdoWfsCapabilitiesSaxContext.h \
  ./FdoWfsExpressionCapabilities.h \
  ./FdoWfsNamedIoStream.h \
  ./FdoWfsSchemaCopyHandler.h \
  ./feature.h \
  ./FdoWfsCommandCapabilities.h \
  ./FdoWfsFeatureReader.h \
  ./FdoWfsOgcFilterCapabilities.h \
  ./FdoWfsSchemaMerger.h \
  ./geometry.h \
  ./FdoWfsConnectionCapabilities.h \
  ./FdoWfsFeatureType.h \
  ./FdoWfsOgcScalarCapabilities.h \
  ./FdoWfsSchemaRefHandler.h \
  ./resource.h \
  ./FdoWfsConnection.h \
  ./FdoWfsFeatureTypeList.h \
  ./FdoWfsOgcSpatialCapabilities.h \
  ./FdoWfsSelectAggregatesCommand.h  \
  ./stdafx.h \
  ./FdoWfsConnectionInfo.h \
  ./FdoWfsFilterCapabilities.h \
  ./FdoWfsOperations.h \
  ./FdoWfsSelectCommand.h \
  ./xlinks.h \
  ./FdoWfsDataReader.h \
  ./FdoWfsGeometryCapabilities.h \
  ./FdoWfsProvider.h \
  ./FdoWfsServiceMetadata.h \
  ./FdoWfsDelegate.h \
  ./FdoWfsGetFeature.h \
  ./FdoWfsRasterCapabilities.h \
  ./FdoWfsSpatialContextReader.h \
  ./FdoWfsDescribeFeatureType.h \
  ./FdoWfsGetSpatialContexts.h \
  ./FdoWfsReader.h \
  ./FdoWfsTopologyCapabilities.h \
  ./FdoWfsDescribeSchemaCommand.h \
  ./FdoWfsGlobals.h \
  ./FdoWfsSchemaCapabilities.h \
  ./FdoWfsUtils.h

libWFSProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I./   \
  -I$(FDO)/Unmanaged/Inc  \
  -I./inc -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/OWS/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

