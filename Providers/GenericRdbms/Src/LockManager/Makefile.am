AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = LockManager.vcproj

noinst_LTLIBRARIES = libLockManager.la

libLockManager_la_SOURCES = \
  Disposable.cpp \
  LockConflict.cpp \
  LockInfo.cpp \
  LockType.cpp \
  LockTypes.cpp \
  OwnerInfo.cpp \
  stdafx.cpp \
  TableInfo.cpp

noinst_HEADERS = $(libLockManager_la_SOURCES) \
  Disposable.h \
  LockConflict.h \
  LockConflictQueryHandler.h \
  LockConstants.h \
  LockInfo.h \
  LockInfoQueryHandler.h \
  LockManager.h \
  LockManagerService.h \
  LockOwnersQueryHandler.h \
  LockStrategy.h \
  LockTableQueryHandler.h \
  LockType.h \
  LockTypeEnum.h \
  LockTypes.h \
  Operation.h \
  OwnerInfo.h \
  stdafx.h \
  TableInfo.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I../Fdo/Lock \
  -I../Fdo/Other \
  -I../Fdo/Connection \
  -I../Fdo/Capability \
  -I../Fdo/Custom \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Filter \
  -I../Fdo/Lt \
  -I../Fdo/Schema \
  -I../Fdo/SpatialContext \
  -I../ \
  -I../../ \
  -I../../Inc \
  -I../Gdbi \
  -I../LongTransactionManager \
  -I../LockManager

libLockManager_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

