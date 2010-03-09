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

EXTRA_DIST = KingOracleProvider.vcproj KgOraProvider.rc resource.h

noinst_LTLIBRARIES = libKingOracleProviderSrc.la

libKingOracleProviderSrc_la_SOURCES = \
c_FdoOra_API2.cpp \
c_KgOraRasterCapabilities.cpp \
c_FgfToSdoGeom.cpp \
c_KgOraReader.cpp \
c_KgOraDataReader.cpp \
c_FilterStringBuffer.cpp \
c_KgOraSchemaCapabilities.cpp \
c_KgOraApplySchema.cpp \
c_KgOraSchemaDesc.cpp \
c_KgOraCommandCapabilities.cpp \
c_KgOraSchemaPool.cpp \
c_KgOraConnectionCapabilities.cpp \
c_KgOraSelectCommand.cpp \
c_KgOraConnection.cpp \
c_KgOraSpatialContext.cpp \
c_KgOraConnectionInfo.cpp \
c_KgOraSpatialContextReader.cpp \
c_KgOraCreateDataStore.cpp \
c_KgOraSQLCommand.cpp \
c_KgOraCreateSpatialContext.cpp \
c_KgOraSQLDataReader.cpp \
c_KgOraDelete.cpp \
c_KgOraSqlParamDesc.cpp \
c_KgOraDescribeSchemaCommand.cpp \
c_KgOraSridDesc.cpp \
c_KgOraExpressionCapabilities.cpp \
c_KgOraTopologyCapabilities.cpp \
c_KgOraExpressionProcessor.cpp \
c_KgOraUpdate.cpp \
c_KgOraFeatureReader.cpp \
c_LogAPI.cpp \
c_KgOraFeatureReaderInsert.cpp \
c_OCI_API.cpp \
c_Oci_ColumnData.cpp \
c_Oci_Statement.cpp \
c_KgOraSelectAggregates.cpp \
c_KgOraFilterCapabilities.cpp \
c_Ora_API2.cpp \
c_KgOraFilterProcessor.cpp \
c_SdoGeomToAGF2.cpp \
c_KgOraGeometryCapabilities.cpp \
c_KgOraInsert.cpp \
c_KgOraGetSpatialContextsCommand.cpp\
c_SDO_DIM_ELEMENT.cpp \
c_SDO_GEOMETRY.cpp \
c_SdeGeom2AGF.cpp


noinst_HEADERS = $(libKingOracleProviderSrc_la_SOURCES) \
c_FdoOra_API.h \
c_KgOraReader.h \
c_FgfToSdoGeom.h \
c_KgOraSchemaCapabilities.h \
c_FilterStringBuffer.h \
c_KgOraSchemaDesc.h \
c_KgOraApplySchema.h \
c_KgOraSchemaPool.h \
c_KgOraCommandCapabilities.h \
c_KgOraSelectCommand.h \
c_KgOraConnectionCapabilities.h \
c_KgOraSpatialContext.h \
c_KgOraConnection.h \
c_KgOraSpatialContextReader.h \
c_KgOraConnectionInfo.h \
c_KgOraSQLCommand.h \
c_KgOraCreateDataStore.h \
c_KgOraSQLDataReader.h \
c_KgOraCreateSpatialContext.h \
c_KgOraSqlParamDesc.h \
c_KgOraDelete.h \
c_KgOraSridDesc.h \
c_KgOraDescribeSchemaCommand.h \
c_KgOraTopologyCapabilities.h \
c_KgOraExpressionCapabilities.h \
c_KgOraUpdate.h \
c_KgOraExpressionProcessor.h \
c_LogAPI.h \
c_KgOraFdoCommand.h \
c_OCCI_API.h \
c_KgOraFdoFeatureCommand.h \
c_Ora_API.h \
c_KgOraFeatureReader.h \
c_SdoGeomToAGF.h \
c_KgOraFeatureReaderInsert.h \
c_KgOraFilterCapabilities.h \
c_KgOraFilterProcessor.h \
c_KgOraGeometryCapabilities.h \
c_KgOraGetSpatialContextsCommand.h \
c_KgOraInsert.h \
c_KgOraRasterCapabilities.h \
FDO_31_Types.h \
KgOraProvider.h \
c_SDO_DIM_ELEMENT.h \
c_SDO_GEOMETRY.h \
c_SdeGeom2AGF.h
stdafx.h

libKingOracleProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../ \
  -I../../inc \
  -I../Message/Inc \
  -I../../../../Fdo/Unmanaged/Inc \
  -I../../../../Utilities/Common/Inc \
  -I/usr/include/oracle/11.2/client \
  -I$(FDOORACLE)/include

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D_FDO_3_2 -D_FDO_3_5

