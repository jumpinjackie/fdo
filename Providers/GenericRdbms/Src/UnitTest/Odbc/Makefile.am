AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = 

noinst_LTLIBRARIES = libOdbcUnitTests.la

libOdbcUnitTests_la_SOURCES = \
OdbcBaseSetup.cpp \
OdbcConnectionUtil.cpp \
OdbcDescribeSchemaTest.cpp \
OdbcFdoAdvancedSelectTest.cpp \
OdbcFdoConnectTest.cpp \
OdbcFdoDeleteTest.cpp \
OdbcFdoInsertTest.cpp \
OdbcFdoMultiThreadTest.cpp \
OdbcFdoSchemaTest.cpp \
OdbcFdoSelectTest.cpp \
OdbcFdoSqlCmdTest.cpp \
OdbcFdoUpdateTest.cpp

noinst_HEADERS = $(libOdbcUnitTests_la_SOURCES) \
OdbcBaseSetup.h \
OdbcConnectionUtil.h \
OdbcDescribeSchemaTest.h \
OdbcFdoAdvancedSelectTest.h \
OdbcFdoConnectTest.h \
OdbcFdoDeleteTest.h \
OdbcFdoInsertTest.h \
OdbcFdoMultiThreadTest.h \
OdbcFdoSchemaTest.h \
OdbcFdoSelectTest.h \
OdbcFdoSqlCmdTest.h \
OdbcFdoUpdateTest.h

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
  -I../Common \
  -I../ \
  -I../../Fdo/FeatureCommands

libOdbcUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -DRDBI_STATIC 

