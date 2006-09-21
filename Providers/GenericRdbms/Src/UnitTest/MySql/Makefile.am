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

noinst_LTLIBRARIES = libMySqlUnitTests.la

libMySqlUnitTests_la_SOURCES = \
MySqlConnectionUtil.cpp \
MySqlConnectTests.cpp \
MySqlDataTypeTests.cpp \
MySqlDescribeSchemaTest.cpp \
MySqlFdoAdvancedSelectTest.cpp \
MySqlFdoApplySchemaTest.cpp \
MySqlFdoAssociationDeleteTest.cpp \
MySqlFdoAssociationInsertTest.cpp \
MySqlFdoAssociationSchemaTest.cpp \
MySqlFdoAssociationSelectTest.cpp \
MySqlFdoAssociationUpdateTest.cpp \
MySqlFdoCapabilityTest.cpp \
MySqlFdoConnectTest.cpp \
MySqlFdoConstraintsTest.cpp \
MySqlFdoDatastoreTest.cpp \
MySqlFdoDeleteTest.cpp \
MySqlFdoImportExportTest.cpp \
MySqlFdoInsertTest.cpp \
MySqlFdoMultiThreadTest.cpp \
MySqlFdoSchemaRollbackTest.cpp \
MySqlFdoSchemaTest.cpp \
MySqlFdoSelectTest.cpp \
MySqlFdoSqlCmdTest.cpp \
MySqlFdoUpdateTest.cpp \
MySqlGeometryTests.cpp \
MySqlSchemaMgrTests.cpp \
MySqlSelectTests.cpp

noinst_HEADERS = $(libMySqlUnitTests_la_SOURCES) \
MySqlConnectionUtil.h \
MySqlConnectTests.h \
MySqlDataTypeTests.h \
MySqlDescribeSchemaTest.h \
MySqlFdoAdvancedSelectTest.h \
MySqlFdoApplySchemaTest.h \
MySqlFdoAssociationDeleteTest.h \
MySqlFdoAssociationInsertTest.h \
MySqlFdoAssociationSchemaTest.h \
MySqlFdoAssociationSelectTest.h \
MySqlFdoAssociationUpdateTest.h \
MySqlFdoCapabilityTest.h \
MySqlFdoConnectTest.h \
MySqlFdoConstraintsTest.h \
MySqlFdoDatastoreTest.h \
MySqlFdoDeleteTest.h \
MySqlFdoImportExportTest.h \
MySqlFdoInsertTest.h \
MySqlFdoMultiThreadTest.h \
MySqlFdoSchemaRollbackTest.h \
MySqlFdoSchemaTest.h \
MySqlFdoSelectTest.h \
MySqlFdoSqlCmdTest.h \
MySqlFdoUpdateTest.h \
MySqlGeometryTests.h \
MySqlSchemaMgrTests.h \
MySqlSelectTests.h

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
  -I../../Fdo/FeatureCommands

libMySqlUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -DRDBI_STATIC

