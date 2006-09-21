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

EXTRA_DIST = ShpProvider.vcproj ShpProvider.rc resource.h

noinst_LTLIBRARIES = libSHPProviderSrc.la

libSHPProviderSrc_la_SOURCES = \
  stdafx.cpp \
  ShpCommandCapabilities.cpp \
  ShpConnectionCapabilities.cpp \
  ShpConnection.cpp \
  ShpConnectionInfo.cpp \
  ShpSchemaUtilities.cpp \
  ShpDescribeSchemaCommand.cpp \
  ShpDescribeSchemaMappingCommand.cpp \
  ShpApplySchemaCommand.cpp \
  ShpDestroySchemaCommand.cpp \
  ShpExpressionCapabilities.cpp \
  ShpFeatureReader.cpp \
  ShpFilterExecutor.cpp \
  ShpFileSet.cpp \
  ShpFilterCapabilities.cpp \
  ShpGeometryCapabilities.cpp \
  ShpQueryOptimizer.cpp \
  ShpFeatIdQueryTester.cpp \
  ShpFeatIdQueryEvaluator.cpp \
  ShpRasterCapabilities.cpp \
  ShpSchemaCapabilities.cpp \
  ShpSelectCommand.cpp \
  ShpSelectAggregates.cpp \
  ShpSpatialExtentsAggregateReader.cpp \
  ShpInsertCommand.cpp \
  ShpUpdateCommand.cpp \
  ShpDeleteCommand.cpp \
  ShpTopologyCapabilities.cpp \
  ShpGetSpatialContextsCommand.cpp \
  ShpSpatialContext.cpp \
  ShpSpatialContextReader.cpp \
  ShpPhysicalSchema.cpp \
  ShpLpClassDefinition.cpp \
  ShpLpFeatureSchema.cpp \
  ShpLpPropertyDefinition.cpp

noinst_HEADERS = $(libSHPProviderSrc_la_SOURCES) \
   resource.h \
   ShpFeatureReader.h \
   ShpRasterCapabilities.h \
   ShpApplySchemaCommand.h \
   ShpFileSet.h \
   ShpReader.h \
   ShpCommandCapabilities.h \
   ShpFilterCapabilities.h \
   ShpSchemaCapabilities.h \
   ShpConnectionCapabilities.h \
   ShpFilterExecutor.h \
   ShpSchemaUtilities.h \
   ShpConnection.h \
   ShpGeometryCapabilities.h \
   ShpSelectAggregates.h \
   ShpConnectionInfo.h \
   ShpGetSpatialContextsCommand.h \
   ShpSelectCommand.h \
   ShpDeleteCommand.h \
   ShpInsertCommand.h \
   ShpSpatialContext.h \
   ShpDescribeSchemaCommand.h \
   ShpLpClassDefinition.h \
   ShpSpatialContextReader.h \
   ShpDescribeSchemaMappingCommand.h \
   ShpLpFeatureSchema.h \
   ShpSpatialExtentsAggregateReader.h \
   ShpDestroySchemaCommand.h \
   ShpLpPropertyDefinition.h \
   ShpTopologyCapabilities.h \
   ShpExpressionCapabilities.h \
   ShpPhysicalSchema.h \
   ShpUpdateCommand.h \
   ShpFeatIdQueryEvaluator.h \
   ShpProvider.h \
   stdafx.h \
   ShpFeatIdQueryTester.h \
   ShpQueryOptimizer.h

libSHPProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../Message/Inc/  \
  -I../ShpRead   \
  -I../ShpSpatialIndex  \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc  \
  -I../../Inc  \
  -I$(FDO)/Unmanaged/Inc/Fdo/Expression

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

