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

EXTRA_DIST =  \
  resource.h \
  ArcSDEProvider.vcproj \
  ArcSDEProvider.rc \
  ArcSDEProvider91.vcproj \
  ArcSDEProvider91.rc \
  ArcSDEProvider92.vcproj \
  ArcSDEProvider92.rc

lib_LTLIBRARIES = libArcSDEProvider91.la

libArcSDEProvider91_la_SOURCES = \
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
  ArcSDESpatialContextSQLReader.cpp \
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
  stdafx.cpp

noinst_HEADERS = $(libArcSDEProvider91_la_SOURCES) \
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
   ArcSDESpatialContextSQLReader.h \
   ArcSDESpatialContextUtility.h \
   ArcSDESQLCommand.h \
   ArcSDESQLDataReader.h \
   ArcSDETransaction.h \
   ArcSDEUpdateCommand.h \
   ArcSDEUtils.h \
   FdoSde.h \
   resource.h \
   stdafx.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/inc/Fdo/Expression \
  -I$(FDOTHIRDPARTY)/ESRI/ArcSDEClient91/Linux/include \
  -I$(SDEHOME)/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I../../Inc

libArcSDEProvider91_la_LIBADD =  \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpe91 \
  -lsde91 \
  -lsg91

libArcSDEProvider91_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/ESRI/ArcSDEClient91/Linux/lib \
  -L$(SDEHOME)/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

