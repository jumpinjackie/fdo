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

#ifdef SDE_UNICODE
	#define sde_multibyte_to_wide(w,mb) w = sde_pus2wc(mb);
	#define sde_wide_to_multibyte(mb,w) mb = sde_pwc2us(w);
	#define sde_sprintf FdoCommonOSUtil::swprintf
	#define sde_stricmp FdoCommonOSUtil::wcsicmp
	#define sde_strlen	wcslen
	#define sde_strcpy	wcscpy
	#define sde_strcat	wcscat
	#define sde_strncpy wcsncpy
	#define sde_strcmp  wcscmp
	#define sde_strchr  wcschr
#ifdef _WIN32
	#define sde_strdup  _wcsdup
#else
	#define sde_strdup  wcsdup
#endif
	#define sde_PropertyValue(parser, prop)	sde_pcwc2us(parser.GetPropertyValueW(prop))
	#define sde_pwc2us(str) (CHAR*)(wchar_t*)(str)
	#define sde_pcwc2us(str) (const CHAR*)(const wchar_t*)(str)
	#define sde_pus2wc(str) (wchar_t*)(CHAR*)(str)
	#define sde_pcus2wc(str) (const wchar_t*)(const CHAR*)(str)
	#define sde_cstwc(str) (const wchar_t*)(str)
	#define sde_std_string std::wstring
	#define sde_ltoa(lSuffix, sSuffix) FdoCommonOSUtil::ltow(lSuffix, sSuffix, 15)
    #define sde_isalpha(string, stringLength, current) (iswalpha(*((const wchar_t*)string)) ? -1 : 0)
    #define sde_isalnum(string, stringLength, current) (iswalnum(*((const wchar_t*)string)) ? -1 : 0)
    #define sde_ismbstrail(str, current) 0
    #define sde_ismbslead(str, current) 0
	#define SDE_CHAR wchar_t
	#ifndef _TXT
		#define _TXT(x) L## x
	#endif
#else
	#define sde_multibyte_to_wide(w,mb) multibyte_to_wide(w,mb)
	#define sde_wide_to_multibyte(mb,w) wide_to_multibyte(mb,w)
	#define sde_sprintf FdoCommonOSUtil::scprintf
	#define sde_stricmp FdoCommonOSUtil::stricmp
	#define sde_strlen	strlen
	#define sde_strcpy	strcpy
	#define sde_strcat	strcat
	#define sde_strncpy strncpy
	#define sde_strcmp  strcmp
	#define sde_strchr  strchr
#ifdef _WIN32
	#define sde_strdup  _strdup
#else
	#define sde_strdup  strdup
#endif
	#define sde_PropertyValue(parser, prop)	sde_pcwc2us(parser.GetPropertyValue(prop))
	#define sde_pwc2us(str) str
	#define sde_pcwc2us(str) (const CHAR*)(str)
	#define sde_pus2wc(str) str
	#define sde_pcus2wc(str) (const CHAR*)(str)
	#define sde_cstwc(str) (const CHAR*)(str)
	#define sde_std_string std::string
	#define sde_ltoa(lSuffix, sSuffix) FdoCommonOSUtil::ltoa(lSuffix, sSuffix)
    #define sde_isalpha(string, stringLength, current) FdoCommonOSUtil::ismbcalpha((const unsigned char*)string, stringLength, (const unsigned char*)current)
    #define sde_isalnum(string, stringLength, current) FdoCommonOSUtil::ismbcalnum((const unsigned char*)string, stringLength, (const unsigned char*)current)
    #define sde_ismbstrail(str, current) FdoCommonOSUtil::ismbstrail((const unsigned char*)str, (const unsigned char*)current)
    #define sde_ismbslead(str, current) FdoCommonOSUtil::ismbslead((const unsigned char*)str, (const unsigned char*)current)
	#define SDE_CHAR CHAR
	#ifndef _TXT
		#define _TXT(x) x
	#endif
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
#include <ArcSDEGetSchemaNamesCommand.h>
#include <ArcSDEGetClassNamesCommand.h>

#endif // FDOSDE_H


