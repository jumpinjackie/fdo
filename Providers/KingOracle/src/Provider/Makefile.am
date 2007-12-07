##
## Copyright (C) 2006  SL-King d.o.o
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

noinst_LTLIBRARIES = libShpProviderSrc.la

libShpProviderSrc_la_SOURCES = \
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

libShpProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../Message/inc/  \
  -I../ShpRead   \
  -I../ShpSpatialIndex  \
  -I../../../../fdo/Unmanaged/inc \
  -I../../../FdoCommon/inc  \
  -I../../inc  \
  -I../../../../fdo/Unmanaged/inc/fdo/Expression

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

