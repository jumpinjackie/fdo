//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// =============================================================================
// | The file defines various constants for the Lock Manager.
// =============================================================================


#ifndef FdoRdbmsLockManagerConstants_h
#define FdoRdbmsLockManagerConstants_h

#ifdef _WIN32
#pragma once
#endif


// ----------------------------------------------------------------------------
// ---                           String Constants                           ---
// ----------------------------------------------------------------------------

// ---> String constants of type wchar_t *

#define FDORDBMSLOCKMANAGER_CLASS_ID_ALIAS                  L"CLASS_ID"
#define FDORDBMSLOCKMANAGER_DATA_STORE_ALIAS                L"DATA_STORE"
#define FDORDBMSLOCKMANAGER_LOCK_OWNER_ALIAS                L"LOCK_OWNER"
#define FDORDBMSLOCKMANAGER_LOCK_TYPE_ALIAS                 L"LOCK_TYPE"
#define FDORDBMSLOCKMANAGER_LT_NAME_ALIAS                   L"LT_NAME"
#define FDORDBMSLOCKMANAGER_TABLE_NAME_ALIAS                L"TABLE_NAME"

#define FDORDBMSLOCKMANAGER_CLASS_ID_COLUMN					L"CLASSID"
#define FDORDBMSLOCKMANAGER_LOCK_ID_COLUMN					L"LOCKID"
#define FDORDBMSLOCKMANAGER_LOCK_TYPE_COLUMN				L"LOCKTYPE"
#define FDORDBMSLOCKMANAGER_OWNER_COLUMN					L"USERNAME"
#define FDORDBMSLOCKMANAGER_TABLENAME_COLUMN				L"TABLENAME"

#define FDORDBMSLOCKMANAGER_GEN_LOCK_TYPE_ALL_LT_EXCLUSIVE  L"X"
#define FDORDBMSLOCKMANAGER_GEN_LOCK_TYPE_EXCLUSIVE         L"E"
#define FDORDBMSLOCKMANAGER_GEN_LOCK_TYPE_LT_EXCLUSIVE      L"V"
#define FDORDBMSLOCKMANAGER_GEN_LOCK_TYPE_SHARED            L"S"

#define FDORDBMSLOCKMANAGER_WS_LOCK_TYPE_ALL_LT_EXCLUSIVE   L"EXCLUSIVE"
#define FDORDBMSLOCKMANAGER_WS_LOCK_TYPE_EXCLUSIVE          L"WORKSPACE EXCLUSIVE"
#define FDORDBMSLOCKMANAGER_WS_LOCK_TYPE_LT_EXCLUSIVE       L"VERSION EXCLUSIVE"
#define FDORDBMSLOCKMANAGER_WS_LOCK_TYPE_SHARED             L"SHARED"


// ---> String constants of type char *

#define FDORDBMSLOCKMANAGER_CLASS_ID_ALIAS_C     "CLASS_ID"
#define FDORDBMSLOCKMANAGER_DATA_STORE_ALIAS_C   "DATA_STORE"
#define FDORDBMSLOCKMANAGER_LOCK_OWNER_ALIAS_C   "LOCK_OWNER"
#define FDORDBMSLOCKMANAGER_LOCK_TYPE_ALIAS_C    "LOCK_TYPE"
#define FDORDBMSLOCKMANAGER_LT_NAME_ALIAS_C      "LT_NAME"
#define FDORDBMSLOCKMANAGER_TABLE_NAME_ALIAS_C   "TABLE_NAME"
#define FDORDBMSLOCKMANAGER_LTID_ALIAS_C		 "LTID"

#define FDORDBMSLOCKMANAGER_CLASS_ID_COLUMN_C					"CLASSID"
#define FDORDBMSLOCKMANAGER_LOCK_ID_COLUMN_C				"LOCKID"
#define FDORDBMSLOCKMANAGER_LOCK_TYPE_COLUMN_C				"LOCKTYPE"
#define FDORDBMSLOCKMANAGER_OWNER_COLUMN_C					"USERNAME"
#define FDORDBMSLOCKMANAGER_TABLENAME_COLUMN_C				"TABLENAME"
#define FDORDBMSLOCKMANAGER_DATASTORENAME_COLUMN_C			"DATASTORENAME"
#define FDORDBMSLOCKMANAGER_LTID_COLUMN_C					"LTID"

#define FDOSYS_OWNER_ALIAS L"FS"
#define CLASS_TABLE_ALIAS L"CT"
#define LOCK_TABLE_ALIAS L"LT"
#define EXPR_TABLE_ALIAS L"ET"
#endif

