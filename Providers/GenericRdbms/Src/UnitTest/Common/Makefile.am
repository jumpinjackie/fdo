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

EXTRA_DIST = Pch.cpp

noinst_LTLIBRARIES = libCommonUnitTests.la

libCommonUnitTests_la_SOURCES = \
UnitTest.cpp \
glibpatch.cpp \
ConnectionUtil.cpp \
ConnectTests.cpp \
DataTypeTests.cpp \
DescribeSchemaTest.cpp \
FdoAdvancedSelectTest.cpp \
FdoApplySchemaTest.cpp \
FdoAssociationDeleteTest.cpp \
FdoAssociationInsertTest.cpp \
FdoAssociationSchemaTest.cpp \
FdoAssociationSelectTest.cpp \
FdoAssociationUpdateTest.cpp \
FdoCapabilityTest.cpp \
FdoConnectTest.cpp \
FdoConstraintsTest.cpp \
FdoDatastoreTest.cpp \
FdoDeleteTest.cpp \
FdoFilterTest.cpp \
FdoExpressionFunctionTest.cpp \
FdoForeignSchemaTest.cpp \
FdoImportExportTest.cpp \
FdoInsertTest.cpp \
FdoMultiThreadTest.cpp \
FdoSchemaRollbackTest.cpp \
FdoSchemaTest.cpp \
FdoSelectTest.cpp \
FdoSqlCmdTest.cpp \
FdoUpdateTest.cpp \
GeometryTests.cpp \
MessageTest.cpp \
SchemaMgrTests.cpp \
SchemaOverrideUtil.cpp \
SelectTests.cpp \
XmlFormatter.cpp \
UnitTestUtil.cpp \
ReaderTest.cpp

noinst_HEADERS = $(libCommonUnitTests_la_SOURCES) \
ConnectionUtil.h \
ConnectTests.h \
DataTypeTests.h \
DescribeSchemaTest.h \
FdoAdvancedSelectTest.h \
FdoApplySchemaTest.h \
FdoAssociationDeleteTest.h \
FdoAssociationInsertTest.h \
FdoAssociationSchemaTest.h \
FdoAssociationSelectTest.h \
FdoAssociationUpdateTest.h \
FdoCapabilityTest.h \
FdoConnectTest.h \
FdoConstraintsTest.h \
FdoDatastoreTest.h \
FdoDeleteTest.h \
FdoFilterTest.h \
FdoExpressionFunctionTest.h \
FdoForeignSchemaTest.h \
FdoImportExportTest.h \
FdoInsertTest.h \
FdoMultiThreadTest.h \
FdoSchemaRollbackTest.h \
FdoSchemaTest.h \
FdoSelectTest.h \
FdoSqlCmdTest.h \
FdoUpdateTest.h \
GeometryTests.h \
MessageTest.h \
SchemaMgrTests.h \
SchemaOverrideUtil.h \
SelectTests.h \
StringPropertiesDictionary.h \
UnitTestUtil.h \
XmlFormatter.h \
Pch.h \
ReaderTest.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
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
  -I../../Fdo/FeatureCommands \
  -I$(FDOUTILITIES)/TestCommon/Inc


libCommonUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS -DRDBI_STATIC


