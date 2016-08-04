// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifdef _WIN32
    #ifdef SDF_EXPORTS
        #define SDF_API __declspec(dllexport)
        #else
        #define SDF_API __declspec(dllimport)
    #endif
#else 
#define SDF_API
#endif

#include <Fdo.h>


// SQLite required headers. This is a SQLite wrapper library
#include "SQLiteDataBase.h"
#include "SQLiteTable.h"
#include "SQLiteData.h"
#include "SQLiteCursor.h"
#include "SQLiteTransaction.h"

//database record number type -- just redefine it for clarity
typedef SQLiteRecNumbDef REC_NO;
typedef std::vector<REC_NO> recno_list;


