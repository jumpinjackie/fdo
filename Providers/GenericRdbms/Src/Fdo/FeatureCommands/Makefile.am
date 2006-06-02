AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbms_FeatureCommands.la

libFdoRdbms_FeatureCommands_la_SOURCES = \
  FdoRdbmsDeleteCommand.cpp        \
  FdoRdbmsInsertCommand.cpp  \
  FdoRdbmsFeatureInfoReader.cpp    \
  FdoRdbmsSelectCommand.cpp \
  FdoRdbmsFeatureReader.cpp        \
  FdoRdbmsUpdateCommand.cpp  \
  FdoRdbmsFeatureSubsetReader.cpp

noinst_HEADERS = $(libFdoRdbms_FeatureCommands_la_SOURCES) \
  FdoRdbmsCommand.h \
  FdoRdbmsDataReader.h \
  FdoRdbmsDeleteCommand.h \
  FdoRdbmsFeatureCommand.h \
  FdoRdbmsFeatureInfoReader.h \
  FdoRdbmsFeatureReader.h \
  FdoRdbmsFeatureSubsetReader.h \
  FdoRdbmsInsertCommand.h \
  FdoRdbmsSelectAggregates.h \
  FdoRdbmsSelectCommand.h \
  FdoRdbmsUpdateCommand.h

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
  -I../../ \
  -I../../../ \
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

libFdoRdbms_FeatureCommands_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

