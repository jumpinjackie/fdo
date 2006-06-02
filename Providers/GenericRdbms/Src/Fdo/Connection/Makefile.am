AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbms_Connection.la

libFdoRdbms_Connection_la_SOURCES = \
  DbiConnection.cpp \
  FdoRdbmsConnectionPropertyDictionary.cpp \
  FdoRdbmsConnection.cpp \
  FdoRdbmsPropertyDictionary.cpp

noinst_HEADERS = $(libFdoRdbms_Connection_la_SOURCES) \
  DbiConnection.h \
  FdoRdbmsConnection.h \
  FdoRdbmsConnectionInfo.h \
  FdoRdbmsConnectionPropertyDictionary.h \
  FdoRdbmsPropertyDictionary.h

INCLUDES = \
  -I../ \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../Inc \
  -I../../../Inc/Rdbi \
  -I../../ -I../../../ \
  -I../../../Inc \
  -I.././Other \
  -I.././Capability \
  -I.././Connection \
  -I.././Custom \
  -I.././FeatureCommands \
  -I.././Filter \
  -I.././Lock \
  -I.././Lt \
  -I.././Schema \
  -I.././SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/src/geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi \
  -I../../LongTransactionManager \
  -I../../LockManager


libFdoRdbms_Connection_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

