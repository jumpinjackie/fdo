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

#ifndef RDBI_VNDR_INFO_DEFINED
/************************************************************************
* Name                                                                  *
*   rdbi/vndr_info.h - define vendor information structure              *
*                       - rdbi_vndr_info_def                            *
*                                                                       *
* Synopsis                                                              *
*   #include <Inc/Rdbi/vndr_info.h>                                     *
*                                                                       *
*                                                                       *
* Description                                                           *
*       The  structure defined here provides information about an       *
*       RDBMS vendor. For example, column precision and scale limits    *
*       are provided.                                                   *
*                                                                       *
* Remarks                                                               *
*                                                                       *
*                                                                       *
************************************************************************/

#define RDBI_VNDR_INFO_DEFINED
#include "Inc/rdbi.h"


//
// Defines used to initialize the dbversion field of the rdbi_vndr_info_def structure.
// It's only used by the ODBC driver to expose the type of backend driver.
#define  RDBI_DBVERSION_UNKNOW          0
#define  RDBI_DBVERSION_SQLSERVER       1
#define  RDBI_DBVERSION_MYSQL           2
#define  RDBI_DBVERSION_ODBC_ORACLE     3
#define  RDBI_DBVERSION_ODBC_SQLSERVER  4
#define  RDBI_DBVERSION_ODBC_ACCESS     5
#define  RDBI_DBVERSION_ODBC_MYSQL      6


typedef struct rdbi_vndr_info_def {
    char name[RDBI_TABLE_NAME_SIZE];         /* vendor name                   */
    unsigned long   dbversion;              /* database version              */
    long minPrecision;                      /* Minimum allowed precision on numeric columns */
    long maxPrecision;                      /* Maximum allowed precision on numeric columns */
    long minScale;                          /* Minimum allowed scale on numeric columns */
    long maxScale;                          /* Maximum allowed scale on numeric columns */
    long minStrLen;                         /* Minimum length for string columns */
    long maxStrLen;                         /* Maximum length for string columns */
	int  maxFetchSize;

} rdbi_vndr_info_def;

#endif

