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
SelectTests.cpp \
XmlFormatter.cpp \
UnitTestUtil.cpp

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
SelectTests.h \
StringPropertiesDictionary.h \
UnitTestUtil.h \
XmlFormatter.h \
Pch.h

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
  -I../../Fdo/FeatureCommands

libCommonUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS -DRDBI_STATIC


