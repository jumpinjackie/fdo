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

lib_LTLIBRARIES = libArcSDEProvider92.la

libArcSDEProvider92_la_SOURCES = \
  ../Provider/ArcSDEAcquireLockCommand.cpp \
  ../Provider/ArcSDELockConflictReader.cpp \
  ../Provider/ArcSDELockUtility.cpp \
  ../Provider/ArcSDEGetLockInfoCommand.cpp \
  ../Provider/ArcSDEGetLockedObjectsCommand.cpp \
  ../Provider/ArcSDEGetLockOwnersCommand.cpp \
  ../Provider/ArcSDELockOwnersReader.cpp \
  ../Provider/ArcSDELockedObjectReader.cpp \
  ../Provider/ArcSDEReleaseLockCommand.cpp \
  ../Provider/ArcSDEDeactivateLongTransactionCommand.cpp \
  ../Provider/ArcSDEDeleteCommand.cpp \
  ../Provider/ArcSDEDestroySchemaCommand.cpp \
  ../Provider/ArcSDEDestroySpatialContext.cpp \
  ../Provider/ArcSDEFeatureInfoReader.cpp \
  ../Provider/ArcSDEGetLongTransactionsCommand.cpp \
  ../Provider/ArcSDEGetSpatialContexts.cpp \
  ../Provider/ArcSDEIndexMapping.cpp \
  ../Provider/ArcSDEInsertCommand.cpp \
  ../Provider/ArcSDELongTransactionConflictDirectiveEnumerator.cpp \
  ../Provider/ArcSDELongTransactionReader.cpp \
  ../Provider/ArcSDELongTransactionUtility.cpp \
  ../Provider/ArcSDEPropertyMapping.cpp \
  ../Provider/ArcSDERollbackLongTransactionCommand.cpp \
  ../Provider/ArcSDESchemaManager.cpp \
  ../Provider/ArcSDESchemaMapping.cpp \
  ../Provider/ArcSDESpatialContextReader.cpp \
  ../Provider/ArcSDESpatialContextSQLReader.cpp \
  ../Provider/ArcSDESpatialContextUtility.cpp \
  ../Provider/ArcSDEUpdateCommand.cpp \
  ../Provider/ArcSDEUtils.cpp \
  ../Provider/ArcSDEActivateLongTransactionCommand.cpp \
  ../Provider/ArcSDEActivateSpatialContext.cpp \
  ../Provider/ArcSDEApplySchemaCommand.cpp \
  ../Provider/ArcSDEBLOBStreamReader.cpp \
  ../Provider/ArcSDEClassMapping.cpp \
  ../Provider/ArcSDECommitLongTransactionCommand.cpp \
  ../Provider/ArcSDECreateLongTransactionCommand.cpp \
  ../Provider/ArcSDECreateSpatialContext.cpp \
  ../Provider/ArcSDECommandCapabilities.cpp \
  ../Provider/ArcSDEConnectionCapabilities.cpp \
  ../Provider/ArcSDEConnection.cpp \
  ../Provider/ArcSDEConnectionInfo.cpp \
  ../Provider/ArcSDEDataReader.cpp \
  ../Provider/ArcSDEDescribeSchemaCommand.cpp \
  ../Provider/ArcSDEExpressionCapabilities.cpp \
  ../Provider/ArcSDEFeatureReader.cpp \
  ../Provider/ArcSDEFilterCapabilities.cpp \
  ../Provider/ArcSDEFilterToSql.cpp \
  ../Provider/ArcSDEProvider.cpp \
  ../Provider/ArcSDEReader.cpp \
  ../Provider/ArcSDESchemaCapabilities.cpp \
  ../Provider/ArcSDESelectAggregatesCommand.cpp \
  ../Provider/ArcSDESelectCommand.cpp \
  ../Provider/ArcSDESQLCommand.cpp \
  ../Provider/ArcSDESQLDataReader.cpp \
  ../Provider/ArcSDETransaction.cpp \
  ../Provider/ArcSDEDataStoreReader.cpp \
  ../Provider/ArcSDEListDataStores.cpp \
  ../Provider/stdafx.cpp

noinst_HEADERS = $(libArcSDEProvider92_la_SOURCES) \
   ../Provider/ArcSDEAcquireLockCommand.h \
   ../Provider/ArcSDEActivateLongTransactionCommand.h \
   ../Provider/ArcSDEActivateSpatialContext.h \
   ../Provider/ArcSDEApplySchemaCommand.h \
   ../Provider/ArcSDEBLOBStreamReader.h \
   ../Provider/ArcSDECommand.h \
   ../Provider/ArcSDECommandCapabilities.h \
   ../Provider/ArcSDECommitLongTransactionCommand.h \
   ../Provider/ArcSDEConnection.h \
   ../Provider/ArcSDEConnectionCapabilities.h \
   ../Provider/ArcSDEConnectionInfo.h \
   ../Provider/ArcSDECreateLongTransactionCommand.h \
   ../Provider/ArcSDECreateSpatialContext.h \
   ../Provider/ArcSDEDataReader.h \
   ../Provider/ArcSDEDataStoreReader.h \
   ../Provider/ArcSDEDeactivateLongTransactionCommand.h \
   ../Provider/ArcSDEDeleteCommand.h \
   ../Provider/ArcSDEDescribeSchemaCommand.h \
   ../Provider/ArcSDEDestroySchemaCommand.h \
   ../Provider/ArcSDEDestroySpatialContext.h \
   ../Provider/ArcSDEExpressionCapabilities.h \
   ../Provider/ArcSDEFeatureCommand.h \
   ../Provider/ArcSDEFeatureInfoReader.h \
   ../Provider/ArcSDEFeatureReader.h \
   ../Provider/ArcSDEFilterCapabilities.h \
   ../Provider/ArcSDEFilterToSql.h \
   ../Provider/ArcSDEGetLockedObjectsCommand.h \
   ../Provider/ArcSDEGetLockInfoCommand.h \
   ../Provider/ArcSDEGetLockOwnersCommand.h \
   ../Provider/ArcSDEGetLongTransactionsCommand.h \
   ../Provider/ArcSDEGetSpatialContexts.h \
   ../Provider/ArcSDEInsertCommand.h \
   ../Provider/ArcSDEListDataStores.h \
   ../Provider/ArcSDELockConflictReader.h \
   ../Provider/ArcSDELockedObjectReader.h \
   ../Provider/ArcSDELockOwnersReader.h \
   ../Provider/ArcSDELockUtility.h \
   ../Provider/ArcSDELongTransactionConflictDirectiveEnumerator.h \
   ../Provider/ArcSDELongTransactionReader.h \
   ../Provider/ArcSDELongTransactionUtility.h \
   ../Provider/ArcSDEProvider.h \
   ../Provider/ArcSDEReader.h \
   ../Provider/ArcSDEReleaseLockCommand.h \
   ../Provider/ArcSDERollbackLongTransactionCommand.h \
   ../Provider/ArcSDESchemaCapabilities.h \
   ../Provider/ArcSDESchemaManager.h \
   ../Provider/ArcSDESelectAggregatesCommand.h \
   ../Provider/ArcSDESelectCommand.h \
   ../Provider/ArcSDESpatialContextReader.h \
   ../Provider/ArcSDESpatialContextSQLReader.h \
   ../Provider/ArcSDESpatialContextUtility.h \
   ../Provider/ArcSDESQLCommand.h \
   ../Provider/ArcSDESQLDataReader.h \
   ../Provider/ArcSDETransaction.h \
   ../Provider/ArcSDEUpdateCommand.h \
   ../Provider/ArcSDEUtils.h \
   ../Provider/FdoSde.h \
   ../Provider/resource.h \
   ../Provider/stdafx.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/ESRI/ArcSDEClient92/Linux/include \
  -I$(SDEHOME)/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I../Provider

libArcSDEProvider92_la_LIBADD =  \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpe \
  -lsde \
  -lsg

libArcSDEProvider92_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/ESRI/ArcSDEClient92/Linux/lib \
  -L$(SDEHOME)/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

