AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = ArcSDEProvider.vcproj ArcSDEProvider.rc resource.h

noinst_LTLIBRARIES = libArcSDEProviderSrc.la

libArcSDEProviderSrc_la_SOURCES = \
  ArcSDEAcquireLockCommand.cpp \
  ArcSDELockConflictReader.cpp \
  ArcSDELockUtility.cpp \
  ArcSDEGetLockInfoCommand.cpp \
  ArcSDEGetLockedObjectsCommand.cpp \
  ArcSDEGetLockOwnersCommand.cpp \
  ArcSDELockOwnersReader.cpp \
  ArcSDELockedObjectReader.cpp \
  ArcSDEReleaseLockCommand.cpp \
  ArcSDEDeactivateLongTransactionCommand.cpp \
  ArcSDEDeleteCommand.cpp \
  ArcSDEDestroySchemaCommand.cpp \
  ArcSDEDestroySpatialContext.cpp \
  ArcSDEFeatureInfoReader.cpp \
  ArcSDEGetLongTransactionsCommand.cpp \
  ArcSDEGetSpatialContexts.cpp \
  ArcSDEIndexMapping.cpp \
  ArcSDEInsertCommand.cpp \
  ArcSDELongTransactionConflictDirectiveEnumerator.cpp \
  ArcSDELongTransactionReader.cpp \
  ArcSDELongTransactionUtility.cpp \
  ArcSDEPropertyMapping.cpp \
  ArcSDERollbackLongTransactionCommand.cpp \
  ArcSDESchemaManager.cpp \
  ArcSDESchemaMapping.cpp \
  ArcSDESpatialContextReader.cpp \
  ArcSDESpatialContextUtility.cpp \
  ArcSDEUpdateCommand.cpp \
  ArcSDEUtils.cpp \
  ArcSDEActivateLongTransactionCommand.cpp \
  ArcSDEActivateSpatialContext.cpp \
  ArcSDEApplySchemaCommand.cpp \
  ArcSDEBLOBStreamReader.cpp \
  ArcSDEClassMapping.cpp \
  ArcSDECommitLongTransactionCommand.cpp \
  ArcSDECreateLongTransactionCommand.cpp \
  ArcSDECreateSpatialContext.cpp \
  ArcSDECommandCapabilities.cpp \
  ArcSDEConnectionCapabilities.cpp \
  ArcSDEConnection.cpp \
  ArcSDEConnectionInfo.cpp \
  ArcSDEConnectionPropertyDictionary.cpp \
  ArcSDEDataReader.cpp \
  ArcSDEDescribeSchemaCommand.cpp \
  ArcSDEExpressionCapabilities.cpp \
  ArcSDEFeatureReader.cpp \
  ArcSDEFilterCapabilities.cpp \
  ArcSDEFilterToSql.cpp \
  ArcSDEProvider.cpp \
  ArcSDEReader.cpp \
  ArcSDESchemaCapabilities.cpp \
  ArcSDESelectAggregatesCommand.cpp \
  ArcSDESelectCommand.cpp \
  ArcSDESQLCommand.cpp \
  ArcSDESQLDataReader.cpp \
  ArcSDETransaction.cpp \
  ArcSDEDataStoreReader.cpp \
  ArcSDEListDataStores.cpp \
  ConnectionStringParser.cpp \
  stdafx.cpp

noinst_HEADERS = $(libArcSDEProviderSrc_la_SOURCES) \
   ArcSDEAcquireLockCommand.h \
   ArcSDEActivateLongTransactionCommand.h \
   ArcSDEActivateSpatialContext.h \
   ArcSDEApplySchemaCommand.h \
   ArcSDEBLOBStreamReader.h \
   ArcSDECommand.h \
   ArcSDECommandCapabilities.h \
   ArcSDECommitLongTransactionCommand.h \
   ArcSDEConnection.h \
   ArcSDEConnectionCapabilities.h \
   ArcSDEConnectionInfo.h \
   ArcSDEConnectionPropertyDictionary.h \
   ArcSDECreateLongTransactionCommand.h \
   ArcSDECreateSpatialContext.h \
   ArcSDEDataReader.h \
   ArcSDEDataStoreReader.h \
   ArcSDEDeactivateLongTransactionCommand.h \
   ArcSDEDeleteCommand.h \
   ArcSDEDescribeSchemaCommand.h \
   ArcSDEDestroySchemaCommand.h \
   ArcSDEDestroySpatialContext.h \
   ArcSDEExpressionCapabilities.h \
   ArcSDEFeatureCommand.h \
   ArcSDEFeatureInfoReader.h \
   ArcSDEFeatureReader.h \
   ArcSDEFilterCapabilities.h \
   ArcSDEFilterToSql.h \
   ArcSDEGetLockedObjectsCommand.h \
   ArcSDEGetLockInfoCommand.h \
   ArcSDEGetLockOwnersCommand.h \
   ArcSDEGetLongTransactionsCommand.h \
   ArcSDEGetSpatialContexts.h \
   ArcSDEInsertCommand.h \
   ArcSDEListDataStores.h \
   ArcSDELockConflictReader.h \
   ArcSDELockedObjectReader.h \
   ArcSDELockOwnersReader.h \
   ArcSDELockUtility.h \
   ArcSDELongTransactionConflictDirectiveEnumerator.h \
   ArcSDELongTransactionReader.h \
   ArcSDELongTransactionUtility.h \
   ArcSDEProvider.h \
   ArcSDEReader.h \
   ArcSDEReleaseLockCommand.h \
   ArcSDERollbackLongTransactionCommand.h \
   ArcSDESchemaCapabilities.h \
   ArcSDESchemaManager.h \
   ArcSDESelectAggregatesCommand.h \
   ArcSDESelectCommand.h \
   ArcSDESpatialContextReader.h \
   ArcSDESpatialContextUtility.h \
   ArcSDESQLCommand.h \
   ArcSDESQLDataReader.h \
   ArcSDETransaction.h \
   ArcSDEUpdateCommand.h \
   ArcSDEUtils.h \
   ConnectionStringParser.h \
   FdoSde.h \
   resource.h \
   stdafx.h

libArcSDEProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(SDEHOME)/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

