/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef FDOSDE_H
#define FDOSDE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32




// common C++ headers:
#include "malloc.h"
#include <limits>       // For quiet_NaN()
#include <float.h>      // For _isnan()
#ifdef _WIN32
#include <crtdbg.h>  // for _crtSetDbgFlag() and _CrtCheckMemory() and _ASSERTE
#endif


// define MIMIC_OTHER_PROVIDER to match arbitrary behaviour with 'the other' provider
#define MIMIC_OTHER_PROVIDER

// version number for the SDE.DEFAULT version
#define SDE_DEFAULT           1
#define SDE_DEFAULT_LT_NAME   L"SDE.DEFAULT"

// spatial context constants:
#define DEFAULT_M_TOLERANCE  (0.001)

// Base FDO headers
#include <Fdo.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoCommonOSUtil.h>
#include <FdoCommonStringUtil.h>
#include <FdoCommonMiscUtil.h>
#include <FdoCommonConnPropDictionary.h>

// common STL headers:
#include <vector>

// common SDE headers:
#include <sdeerno.h>
#include <sdetype.h>
#include <pef.h>

// OS-specific headers:
#ifndef _WIN32
#include <wctype.h>  // for wchar related functions
#endif

// common ArcSDE Provider headers:
class ArcSDEConnection;
#include "../Message/Inc/ArcSDEMessage.h"
#include <ArcSDEProvider.h>
#include <ArcSDEUtils.h>
#include <ArcSDESchemaManager.h>
#include <ArcSDE/Override/FdoArcSDESchemaMapping.h>
#include <ArcSDE/Override/FdoArcSDEClassMapping.h>
#include <ArcSDE/Override/FdoArcSDEPropertyMapping.h>
#include <ArcSDE/Override/FdoArcSDEIndexMapping.h>
#include <ArcSDEConnection.h>
#include <ArcSDEReader.h>
#include <ArcSDESQLDataReader.h>
#include <ArcSDEFeatureReader.h>
#include <ArcSDETransaction.h>
#include <ArcSDECommand.h>
#include <ArcSDESelectCommand.h>
#include <ArcSDEInsertCommand.h>
#include <ArcSDEDeleteCommand.h>
#include <ArcSDEUpdateCommand.h>
#include <ArcSDEDescribeSchemaCommand.h>
#include <ArcSDEDestroySchemaCommand.h>
#include <ArcSDEApplySchemaCommand.h>
#include <ArcSDESQLCommand.h>
#include <ArcSDEConnectionInfo.h>
#include <ArcSDEConnectionPropertyDictionary.h>
#include <ArcSDEConnectionCapabilities.h>
#include <ArcSDESchemaCapabilities.h>
#include <ArcSDECommandCapabilities.h>
#include <ArcSDEFilterCapabilities.h>
#include <ArcSDEExpressionCapabilities.h>
#include <ArcSDEFilterToSql.h>
#include <ArcSDEFeatureInfoReader.h>
#include <ArcSDELongTransactionUtility.h>
#include <ArcSDECreateLongTransactionCommand.h>
#include <ArcSDEGetLongTransactionsCommand.h>
#include <ArcSDEActivateLongTransactionCommand.h>
#include <ArcSDEDeactivateLongTransactionCommand.h>
#include <ArcSDELongTransactionConflictDirectiveEnumerator.h>
#include <ArcSDECommitLongTransactionCommand.h>
#include <ArcSDERollbackLongTransactionCommand.h>
#include <ArcSDEBLOBStreamReader.h>
#include <ArcSDELockUtility.h>
#include <ArcSDELockConflictReader.h>
#include <ArcSDEAcquireLockCommand.h>
#include <ArcSDELockedObjectReader.h>
#include <ArcSDEGetLockInfoCommand.h>
#include <ArcSDEReleaseLockCommand.h>
#include <ArcSDEGetLockedObjectsCommand.h>
#include <ArcSDELockOwnersReader.h>
#include <ArcSDEGetLockOwnersCommand.h>
#include <ArcSDESpatialContextUtility.h>
#include <ArcSDEActivateSpatialContext.h>
#include <ArcSDECreateSpatialContext.h>
#include <ArcSDEDestroySpatialContext.h>
#include <ArcSDEGetSpatialContexts.h>
#include <ArcSDESpatialContextReader.h>
#include <ArcSDESelectAggregatesCommand.h>
#include <ArcSDEDataReader.h>
#include <ArcSDEListDataStores.h>
#include <ArcSDEDataStoreReader.h>

#endif // FDOSDE_H

