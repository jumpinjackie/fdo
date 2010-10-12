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

EXTRA_DIST = WMSProvider.vcproj FdoWmsProvider.rc

noinst_LTLIBRARIES = libWMSProviderSrc.la

libWMSProviderSrc_la_SOURCES = \
  FdoWmsCommandCapabilities.cpp \
  FdoWmsConnectionCapabilities.cpp \
  FdoWmsConnection.cpp \
  FdoWmsConnectionInfo.cpp \
  FdoWmsDataReader.cpp \
  FdoWmsDescribeSchemaCommand.cpp \
  FdoWmsExpressionCapabilities.cpp \
  FdoWmsFeatureReader.cpp \
  FdoWmsFilterCapabilities.cpp \
  FdoWmsGeometryCapabilities.cpp \
  FdoWmsRasterCapabilities.cpp \
  FdoWmsSchemaCapabilities.cpp \
  FdoWmsSelectCommand.cpp \
  FdoWmsTopologyCapabilities.cpp \
  FdoWmsUtils.cpp \
  FdoWmsActivateSpatialContextCommand.cpp \
  FdoWmsGetSpatialContextsCommand.cpp \
  FdoWmsSpatialContextReader.cpp \
  FdoWmsBandRasterGdal.cpp \
  FdoWmsRasterGdal.cpp \
  FdoWmsImage.cpp \
  FdoWmsRasterPropertyDictionaryGdal.cpp \
  FdoWmsStreamReaderGdalByRow.cpp \
  FdoWmsDescribeSchemaMappingCommand.cpp \
  FdoWmsDimension.cpp \
  FdoWmsLayerCollection.cpp \
  FdoWmsBoundingBoxCollection.cpp \
  FdoWmsLayer.cpp \
  FdoWmsBoundingBox.cpp \
  FdoWmsServiceMetadata.cpp \
  FdoWmsCapabilities.cpp \
  FdoWmsStyleCollection.cpp \
  FdoWmsDelegate.cpp \
  FdoWmsStyle.cpp \
  FdoWmsDimensionCollection.cpp \
  FdoWmsRequestMetadata.cpp \
  FdoWmsGetMap.cpp \
  FdoWmsSelectAggregatesCommand.cpp \
  FdoWmsSpatialExtentsAggregateReader.cpp \
  FdoWmsGetFeatureClassCRSNames.cpp \
  FdoWmsGetFeatureClassStyles.cpp \
  FdoWmsGetImageFormats.cpp 

noinst_HEADERS = $(libWMSProviderSrc_la_SOURCES) \
  FdoWmsActivateSpatialContextCommand.h \
  FdoWmsGetSpatialContextsCommand.h \
  FdoWmsBandRasterGdal.h \
  FdoWmsGlobals.h \
  FdoWmsBoundingBoxCollection.h \
  FdoWmsImage.h \
  FdoWmsBoundingBox.h \
  FdoWmsLayerCollection.h \
  FdoWmsCapabilities.h \
  FdoWmsLayer.h \
  FdoWmsCommandCapabilities.h \
  FdoWmsRasterCapabilities.h \
  FdoWmsCommand.h \
  FdoWmsRasterGdal.h \
  FdoWmsConnectionCapabilities.h \
  FdoWmsRasterPropertyDictionaryGdal.h \
  FdoWmsConnection.h \
  FdoWmsReader.h \
  FdoWmsConnectionInfo.h \
  FdoWmsRect.h \
  FdoWmsDataReader.h \
  FdoWmsSchemaCapabilities.h \
  FdoWmsDelegate.h \
  FdoWmsSelectCommand.h \
  FdoWmsDescribeSchemaCommand.h \
  FdoWmsServiceMetadata.h \
  FdoWmsDescribeSchemaMappingCommand.h \
  FdoWmsSpatialContextReader.h \
  FdoWmsDimensionCollection.h \
  FdoWmsStreamReaderGdalByRow.h \
  FdoWmsDimension.h \
  FdoWmsStyleCollection.h \
  FdoWmsEnvelope.h \
  FdoWmsStyle.h \
  FdoWmsExpressionCapabilities.h \
  FdoWmsTopologyCapabilities.h \
  FdoWmsFeatureCommand.h \
  FdoWmsUtils.h \
  FdoWmsReverseEpsgCodes.h \
  FdoWmsFeatureReader.h \
  FdoWmsXmlGlobals.h \
  FdoWmsFilterCapabilities.h \
  resource.h \
  FdoWmsGeometryCapabilities.h \
  FdoWmsRequestMetadata.h \
  stdafx.h \
  FdoWmsGetMap.h \
  FdoWmsSpatialExtentsAggregateReader.h \
  FdoWmsSelectAggregatesCommand.h \
  FdoWmsGetFeatureClassCRSNames.h \
  FdoWmsGetFeatureClassStyles.h \
  FdoWmsGetImageFormats.h 

libWMSProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I./ \
  -I../ \
  -I../Message/Inc \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/OWS/Inc \
  -I$(FDOGDAL)/include

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

