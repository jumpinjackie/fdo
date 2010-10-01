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

/************************************************************************
* Name                                                                  *
*   constants.h - Definitions needed for ODBC access					*
*                                                                       *
* Synopsis                                                              *
*   #include "constants.h"                                              *
*                                                                       *
************************************************************************/

#ifndef ODBCDR_CONSTANTS_H
#define ODBCDR_CONSTANTS_H 1


#include <Inc/rdbi.h>					/* for RDBI_MSG_SIZE */

#define INIT_ZERO       0
#define INIT_NULL       SQL_NULL_HENV 
#define INIT_M_ONE      -1
#define INIT_SLASH      "/"
#define INIT_SLASHW      L"/"

#define ODBCDR_CONNECT_STRING_DEFAULT	OS_ODBCDR_CONNECT_STRING

#define ODBCDR_DEFAULT_SCHEMA_NAME      "Default"
#define ODBCDR_DEFAULT_SCHEMA_NAMEW      L"Default"


/*
** Required for the memory allocation of null indicator and char buffers
** during bind and define. They are required because of the incompatibilities
** between RDBI and ODBC types. This value is used to set the array size
** for the buffers. An estimate of array size is used because you do not know
** the actual array size until execute time. 
*/
#define ODBCDR_MAX_ARRAY_SIZE 100

/* SQLBindParameter requires the size of data, although data is inserted
 * using SQLPutData() and teoretically this will be redundant.
 */
#define ODBCDR_BLOB_CHUNK_SIZE       (1024 * 10) // total: 10KB * ODBCDR_MAX_ARRAY_SIZE 

/* Max buffer length */
#define ODBCDR_MAX_BUFF_SIZE	RDBI_MSG_SIZE	

/* buffer size for long varchar columns */
#define ODBCDR_LONGVARCHAR_SIZE       16000

/* buffer size for long wide varchar columns */
#define ODBCDR_WLONGVARCHAR_SIZE      32000

/*
 *  Some common ODBC error codes
 * 	These need to be converted to SQL Server
 */
#define ODBCDR_TABLE_CONSTRAINT_NONEXISTENT 5571
#define ODBCDR_INCOMPATIBLE_COLUMN_TYPES	5567
#define ODBCDR_INVLD_USER_PSWD			   1017
#define ODBCDR_TABLESPACE_NONEXISTENT	    959
#define ODBCDR_OBJECT_EXISTS 			    955
#define ODBCDR_TABLE_NONEXISTENT 		    945
#define ODBCDR_INVALID_COLUMN_NAME			904
#define ODBCDR_INVALID_OPTION 		    	955
#define ODBCDR_NOT_IN_DESC_LIST			    911	/* posn parm to describe out of bounds	*/
#define ODBCDR_NO_MORE_COLS				   1007	/* no more cols to describe */
#define ODBCDR_DUPLICATE_INDEX1			 	  9 /* ??? */
#define ODBCDR_DUPLICATE_INDEX5			 	  1
#define ODBCDR_SUCCESS					 	  0
#define ODBCDR_END_OF_FETCH				   1403	/* for real ! pro*c */
#define ODBCDR_INVALID_UPDATE				  3 /* ??? */
#define ODBCDR_RESOURCE_BUSY				54
#define ODBCDR_INVALID_DROP_OPTION		    950

#define ODBCDR_DESC_STRING_LEN	30		/* string len of describe variables	*/

#define ODBCDR_DATE_SIZE 		24      // "1979-11-30 00:00:00.000"	
#define ODBCDR_ROWID_SIZE		16
#define ODBCDR_ROWID_BIND_SIZE	64
#define ODBCDR_ROWID_BIND_STRING	"row_id"

#define ODBCDR_OBJECT_NAME_SIZE	31
#define ODBCDR_OBJECT_TYPE_SIZE	15

#define ODBCDR_CONNECTION_SIZE	515

#define ODBCDR_DBVERS_BASE	(100000000L)
#define ODBCDR_DBVERS8		(8*ODBCDR_DBVERS_BASE)  /* SQL Server 5000 */

typedef enum
{
    ODBCDriverType_Undetermined,  // not yet determined
    ODBCDriverType_OracleNative,
    ODBCDriverType_OracleNonNative,
    ODBCDriverType_SQLServer,
    ODBCDriverType_Sybase,
    ODBCDriverType_Access,
    ODBCDriverType_MySQL,
    ODBCDriverType_Other  // determined, but not an ODBC driver we distinguish specifically
} odbcdr_DriverType;

// Driver-specific constants:
#define ODBCDR_DRIVER_ORACLE_DRIVERNAME_WINDOWS_MB        "SQORA32.DLL"
#define ODBCDR_DRIVER_ORACLE_DRIVERNAME_LINUX_MB          "libsqora.so"
#define ODBCDR_DRIVER_SQLSERVER_DRIVERNAME_MB     "SQLSRV32.DLL"
#define ODBCDR_DRIVER_SYBASE_DRIVERNAME_MB        "Adaptive Server Enterprise (ANSI)"
#define ODBCDR_DRIVER_ACCESS_DRIVERNAME_MB        "ODBCJT32.DLL"
#define ODBCDR_DRIVER_MYSQL_DRIVERNAME_LINUX_MB   "LIBMYODBC3.SO"
#define ODBCDR_DRIVER_MYSQL_DRIVERNAME_WINDOWS_MB "MYODBC3.DLL"
#define ODBCDR_DRIVER_ORACLE_ROWID_NAME           "ROWID"
#define ODBCDR_DRIVER_ORACLE_OWM_TABLE_SUFFIX     "_LT"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_VERSION   "VERSION"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_NEXTVER   "NEXTVER"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_DELSTATUS "DELSTATUS"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_LTLOCK    "LTLOCK"

#define ODBCDR_DRIVER_ORACLE_OWM_TABLE_SUFFIXW     L"_LT"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_VERSIONW   L"VERSION"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_NEXTVERW   L"NEXTVER"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_DELSTATUSW L"DELSTATUS"
#define ODBCDR_DRIVER_ORACLE_OWM_COLUMN_LTLOCKW    L"LTLOCK"
#define ODBCDR_DRIVER_ORACLE_ROWID_NAMEW           L"ROWID"

#define ODBCDR_DRIVER_SYBASE_DEFAULT_SCHEMA_NAME    "dbo"
#define ODBCDR_DRIVER_SYBASE_DEFAULT_SCHEMA_NAMEW  L"dbo"

 /**************************************************************************
 * Macros to use as wrappers around many function calls.
 */

/* ODBCDR_ODBC_ERR() -- Call an ODBCDR function, set statuses, exit if error
 *	Assume local variable:	int rdbi_status
 *	Assume local label:		the_exit
 */
#define ODBCDR_ODBC_ERR( functionCall_I, handle_type_I, handle_I, functionName_I, purpose_I ) \
{ \
	short	_odbcValue = (functionCall_I); \
	if ( _odbcValue != ODBCDR_SUCCESS ) \
	{ \
		debug2( "%s for %s FAILED.", functionName_I, purpose_I ); \
		rdbi_status = odbcdr_xlt_status( context, _odbcValue, handle_type_I, handle_I  ); \
		context->odbcdr_last_rc = _odbcValue; \
		goto the_exit; \
	} \
	else \
	{ \
		debug2( "%s for %s succeeded.", functionName_I, purpose_I ); \
		context->odbcdr_last_rc = _odbcValue; \
	} \
}

#define ODBCDR_ODBC_ERR2( functionCall_I, handle_type_I, handle_I, functionName_I, purpose_I ) \
{ \
	short	_odbcValue = (functionCall_I); \
	if ( _odbcValue != ODBCDR_SUCCESS ) \
	{ \
		debug2( "%s for %s FAILED.", functionName_I, purpose_I ); \
		rdbi_status = odbcdr_xlt_status( context, _odbcValue, handle_type_I, handle_I  ); \
		context->odbcdr_last_rc = _odbcValue; \
        if ( _odbcValue == SQL_SUCCESS_WITH_INFO && rdbi_status == RDBI_SUCCESS ) \
            rdbi_status = RDBI_GENERIC_ERROR; \
		goto the_exit; \
	} \
	else \
	{ \
		debug2( "%s for %s succeeded.", functionName_I, purpose_I ); \
		context->odbcdr_last_rc = _odbcValue; \
	} \
}

/* ODBCDR_RDBI_ERR() -- Call an RDBI function, set status, exit if error
 *	Assume local variable:	int rdbi_status
 *	Assume local label:		the_exit
 */
#define ODBCDR_RDBI_ERR( functionCall_I ) \
{ \
	int		_rdbiValue = (functionCall_I); \
	if ( (_rdbiValue == RDBI_SUCCESS) || (_rdbiValue == SQL_SUCCESS_WITH_INFO) ) \
	{ } else \
	{ \
		rdbi_status = _rdbiValue; \
		goto the_exit; \
	} \
}

#define _check_status  if (rdbi_status != RDBI_SUCCESS) goto the_exit;

#endif
