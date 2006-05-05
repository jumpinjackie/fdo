AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libLTManager.la

libLTManager_la_SOURCES = \
  Disposable.cpp \
  LongTransactionEnvInfo.cpp \
  LongTransactionConflict.cpp \
  LongTransactionInfo.cpp \
  LongTransactionConflictInfo.cpp \
  LongTransactionInfos.cpp \
  LongTransactionConflictInfos.cpp \
  stdafx.cpp \
  LongTransactionConflicts.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../Inc \
  -I../../Inc/Rdbi \
  -I../   \
  -I../../  \
  -I../../Inc \
  -I../Fdo/Other \
  -I../Fdo/Capability \
  -I../Fdo/Connection \
  -I../Fdo/Custom \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Filter \
  -I../Fdo/Lock \
  -I../Fdo/Lt \
  -I../Fdo/Schema \
  -I../Fdo/SpatialContext \
  -I../Fdo/Topo \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/src/geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Gdbi \
  -I../LongTransactionManager \
  -I../LockManager

libLTManager_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

