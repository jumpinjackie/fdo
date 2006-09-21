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

EXTRA_DIST = RFP.vcproj RFPProvider.rc resource.h

noinst_LTLIBRARIES = libGRFPProviderSrc.la

libGRFPProviderSrc_la_SOURCES = \
	FdoRfpClassData.cpp \
	FdoRfpCommandCapabilities.cpp \
	FdoRfpConnection.cpp \
	FdoRfpConnectionCapabilities.cpp \
	FdoRfpConnectionInfo.cpp \
	FdoRfpDataReader.cpp \
	FdoRfpDatasetCache.cpp \
	FdoRfpDescribeSchemaCommand.cpp \
	FdoRfpDescribeSchemaMapping.cpp \
	FdoRfpExpressionCapabilities.cpp \
	FdoRfpFeatureReader.cpp \
	FdoRfpFilterCapabilities.cpp \
	FdoRfpFilterEvaluator.cpp \
	FdoRfpGeoRasterExtractor.cpp \
	FdoRfpGetSpatialContexts.cpp \
	FdoRfpImage.cpp \
	FdoRfpImageFactory.cpp \
	FdoRfpQueryExecutor.cpp \
	FdoRfpRasterCapabilities.cpp \
	FdoRfpRasterPropertyDictionary.cpp \
	FdoRfpRasterUtil.cpp \
	FdoRfpStreamReaderByTile.cpp \
	FdoRfpRect.cpp \
	FdoRfpSchemaCapabilities.cpp \
	FdoRfpSchemaData.cpp \
	FdoRfpSelectAggregate.cpp \
	FdoRfpSelectCommand.cpp \
	FdoRfpSpatialContext.cpp \
	FdoRfpSpatialContextReader.cpp \
	FdoRfpStreamReaderRGB24.cpp \
	FdoRfpVariant.cpp \
	FdoRfpLinux.cpp \
	FdoRfpBandRaster.cpp \
	FdoRfpGeoBandRaster.cpp \
	FdoRfpGeoBandRasterImp.cpp \
	FdoRfpGeoRaster.cpp \
	FdoRfpRaster.cpp \
	FdoRfpGeoreference.cpp \
	FdoRfpKeyColor.cpp \
	stdafx.cpp

noinst_HEADERS = $(libGRFPProviderSrc_la_SOURCES) \
	FDORFP.h \
	FdoRfpGeoRasterExtractor.h \
	FdoRfpBandRaster.h \
	FdoRfpGeoreference.h \
	FdoRfpClassData.h \
	FdoRfpGetSpatialContexts.h \
	FdoRfpCommand.h \
	FdoRfpImage.h \
	FdoRfpCommandCapabilities.h \
	FdoRfpImageDescriptor.h \
	FdoRfpCommonReader.h \
	FdoRfpImageFactory.h \
	FdoRfpConnection.h \
	FdoRfpKeyColor.h \
	FdoRfpConnectionCapabilities.h \
	FdoRfpQueryExecutor.h \
	FdoRfpConnectionInfo.h \
	FdoRfpQueryResult.h \
	FdoRfpRaster.h \
	FdoRfpDataReader.h \
	FdoRfpRasterCapabilities.h \
	FdoRfpDatasetCache.h \
	FdoRfpRasterPropertyDictionary.h \
	FdoRfpDefaultSchema.h \
	FdoRfpRasterUtil.h \
	FdoRfpDescribeSchemaCommand.h \
	FdoRfpSchemaCapabilities.h \
	FdoRfpDescribeSchemaMapping.h \
	FdoRfpSchemaData.h \
	FdoRfpEnvelope.h \
	FdoRfpSelectAggregate.h \
	FdoRfpExpressionCapabilities.h \
	FdoRfpSelectCommand.h \
	FdoRfpFeatureCommand.h \
	FdoRfpSpatialContext.h \
	FdoRfpFeatureReader.h \
	FdoRfpSpatialContextReader.h \
	FdoRfpFilterCapabilities.h \
	FdoRfpStreamReaderByTile.h \
	FdoRfpFilterEvaluator.h \
	FdoRfpStreamReaderRGB24.h \
	FdoRfpGeoBandRaster.h \
	FdoRfpVariant.h \
	FdoRfpGeoBandRasterImp.h \
	FdoRfpGeoRaster.h

libGRFPProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../ \
  -I../../Inc  \
  -I../Message/Inc/  \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/GDAL1.3/linux/include

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32
