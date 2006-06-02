AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = LongTransactionManager.vcproj

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

noinst_HEADERS = $(libLTManager_la_SOURCES) \
  Disposable.h \
  LongTransactionConflict.h \
  LongTransactionConflictInfo.h \
  LongTransactionConflictInfos.h \
  LongTransactionConflicts.h \
  LongTransactionConflictSolution.h \
  LongTransactionConstants.h \
  LongTransactionDataRequestType.h \
  LongTransactionEnvInfo.h \
  LongTransactionFreezeOperation.h \
  LongTransactionInfo.h \
  LongTransactionInfos.h \
  LongTransactionLTQueryHandler.h \
  LongTransactionManager.h \
  LongTransactionManagerService.h \
  LongTransactionQueryModes.h \
  stdafx.h

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
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Gdbi \
  -I../LongTransactionManager \
  -I../LockManager

libLTManager_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

