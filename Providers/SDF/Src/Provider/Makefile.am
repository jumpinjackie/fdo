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

EXTRA_DIST = SDF.vcproj SDF.rc resource.h

noinst_LTLIBRARIES = libSDFProviderSrc.la

libSDFProviderSrc_la_SOURCES = \
  RTree.cpp \
  BinaryReader.cpp \
  BinaryWriter.cpp \
  DataDb.cpp \
  DataIO.cpp \
  ExInfoDb.cpp \
  KeyDb.cpp \
  PhysName.cpp \
  SchemaDb.cpp \
 TableReformatter.cpp \
  utf8_.cpp \
  SdfApplySchema.cpp \
  SdfCommandCapabilities.cpp \
  SdfCompareHandler.cpp \
  SdfConnection.cpp \
  SdfConnectionCapabilities.cpp \
  SdfConnectionInfo.cpp \
  SdfCreateSDFFile.cpp \
  SdfCreateSpatialContext.cpp \
  SdfDataValidator.cpp \
  SdfDelete.cpp \
  SdfDeletingFeatureReader.cpp \
  SdfDescribeSchema.cpp \
  SdfExpressionCapabilities.cpp \
  SdfImpExtendedSelect.cpp \
  SdfFilterCapabilities.cpp \
  SdfGeometryCapabilities.cpp \
  SdfGetSpatialContexts.cpp \
  SdfInsert.cpp \
  SdfRasterCapabilities.cpp \
  SdfSchemaCapabilities.cpp \
  SdfSchemaMergeContext.cpp \
  SdfScrollableFeatureReader.cpp \
  SdfSelect.cpp \
  SdfSelectAggregates.cpp \
  SdfSimpleFeatureReader.cpp \
  SdfIndexedScrollableFeatureReader.cpp \
  SdfSpatialContextReader.cpp \
  SdfTopologyCapabilities.cpp \
  SdfUpdate.cpp \
  SdfUpdatingFeatureReader.cpp \
  SdfDataReader.cpp \
  SdfDistinctDataReader.cpp \
  SdfQueryOptimizer.cpp \
  SdfCreateDataStore.cpp \
  SdfDeleteDataStore.cpp \
  SdfSpatialExtentsAggregateReader.cpp \
  SDF.cpp

noinst_HEADERS = $(libSDFProviderSrc_la_SOURCES) \
   BinaryReader.h \
   BinaryWriter.h \
   DataDb.h \
   DataIO.h \
   KeyDb.h \
   PhysName.h \
   resource.h \
   RTree.h \
   RTreeInfrastructure.h \
   SchemaDb.h \
   TableReformatter.h \
   SDF.h \
   SdfApplySchema.h \
   SdfCollection.h \
   SdfCommand.h \
   SdfCommandCapabilities.h \
   SdfConnection.h \
   SdfConnectionCapabilities.h \
   SdfConnectionInfo.h \
   SdfCreateDatastore.h \
   SdfCreateSDFFile.h \
   SdfCreateSpatialContext.h \
   SdfDataReader.h \
   SdfDataValidator.h \
   SdfDelete.h \
   SdfDeleteDataStore.h \
   SdfDeletingFeatureReader.h \
   SdfDescribeSchema.h \
   SdfDistinctDataReader.h \
   SdfExpressionCapabilities.h \
   SdfExtendedSelect.h \
   SdfExtHashMap.h \
   SdfImpExtendedSelect.h \
   SdfFeatureCommand.h \
   SdfFilterCapabilities.h \
   SdfGeometryCapabilities.h \
   SdfGetSpatialContexts.h \
   SdfImpScrollableFeatureReader.h \
   SdfInsert.h \
   SDFProvider.h \
   SdfQueryOptimizer.h \
   SdfRasterCapabilities.h \
   SdfSchemaCapabilities.h \
   SdfSchemaMergeContext.h \
   SdfScrollableFeatureReader.h \
   SdfSelect.h \
   SdfSelectAggregates.h \
   SdfSimpleFeatureReader.h \
   SdfIndexedScrollableFeatureReader.h \
   SdfSpatialContextReader.h \
   SdfSpatialExtentsAggregateReader.h \
   SdfTopologyCapabilities.h \
   SdfUpdate.h \
   SdfUpdatingFeatureReader.h \
   stdafx.h \
   utf8_.h

libSDFProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I/usr/include \
  -I../Message/Inc \
  -I../Utils \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SQLiteInterface \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc

CXXFLAGS = @CXXFLAGS_SDF@ -DSQLITE -D_FILE_OFFSET_BITS=64 

