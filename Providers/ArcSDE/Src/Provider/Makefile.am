AUTOMAKE_OPTIONS = foreign

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

libArcSDEProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(SDEHOME)/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

