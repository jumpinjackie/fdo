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

EXTRA_DIST = 

noinst_LTLIBRARIES = libPostGisUnitTests.la

libPostGisUnitTests_la_SOURCES = \
PostGisConnectionUtil.cpp \
PostGisConnectTests.cpp \
PostGisDescribeSchemaTest.cpp \
PostGisFdoAdvancedSelectTest.cpp \
PostGisFdoApplySchemaTest.cpp \
PostGisFdoConnectionInfoTest.cpp \
PostGisFdoConnectTest.cpp \
PostGisFdoConstraintsTest.cpp \
PostGisFdoDatastoreTest.cpp \
PostGisFdoDeleteTest.cpp \
PostGisFdoImportExportTest.cpp \
PostGisFdoInsertTest.cpp \
PostGisFdoMultiThreadTest.cpp \
PostGisFdoSelectTest.cpp \
PostGisFdoSqlCmdTest.cpp \
PostGisFdoUpdateTest.cpp \
PostGisGeometryTests.cpp \
PostGisSchemaMgrTests.cpp \
PostGisSchemaOverrideUtil.cpp

noinst_HEADERS = $(libPostGisUnitTests_la_SOURCES) \
PostGisConnectionUtil.h \
PostGisConnectTests.h \
PostGisFdoAdvancedSelectTest.h \
PostGisFdoApplySchemaTest.h \
PostGisFdoConnectionInfoTest.h \
PostGisFdoConnectTest.h \
PostGisFdoDatastoreTest.h \
PostGisFdoDeleteTest.h \
PostGisFdoImportExportTest.h \
PostGisFdoInsertTest.h \
PostGisFdoMultiThreadTest.h \
PostGisFdoSelectTest.h \
PostGisFdoSqlCmdTest.h \
PostGisFdoUpdateTest.h \
PostGisGeometryTests.h \
PostGisSchemaMgrTests.h \
PostGisSchemaOverrideUtil.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../LockManager \
  -I../../LongTransactionManager \
  -I../../Gdbi \
  -I../../../Inc \
  -I../../.. \
  -I../../Fdo/Connection \
  -I../../Fdo/SpatialContext \
  -I../../Fdo/Other \
  -I../../ \
  -I../ \
  -I../Common \
  -I../../Fdo/FeatureCommands \
  -I$(FDOUTILITIES)/TestCommon/Inc \
  -I../../../Inc

libPostGisUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -DRDBI_STATIC

