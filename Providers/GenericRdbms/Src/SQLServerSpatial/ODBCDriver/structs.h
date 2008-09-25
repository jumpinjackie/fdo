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
*   structs.h - Structures needed for ODBC access						*
*                                                                       *
* Synopsis                                                              *
*   #include "structs.h"                                              *
*                                                                       *
************************************************************************/

#ifndef ODBCDR_STRUCTS_H
#define ODBCDR_STRUCTS_H 1

#include <Inc/Util/os.h>

#include <sql.h>

#include <Inc/Util/da.h>                          /* ut_da_def                    */
#include <Inc/Geometry/Fgf/AllGeometry_c.h>    /* pIGeometry_def           */

#include "constants.h"

#ifndef _WIN32
typedef unsigned char *	PBYTE;
#define  strnicmp strncasecmp
#define  _strnicmp strncasecmp
#define _stricmp  strcasecmp
#define _wcsnicmp wcsncasecmp
#endif

typedef SQLHENV odbcdr_environment_def;			/* 1 per instance of odbcdr	*/
typedef	SQLHDBC odbcdr_service_ctx_def;			/* ODBC db conenction handle */
typedef SQLHSTMT odbcdr_cursor_handle_def;		/* 1 per SQL statement		*/

/*---------------------------------------------------------------------------
 * Geometry conversion data structures.
 *  These data structure are the conversion buffers between SqlServer-specific
 *  coordinate geometries and geometries from the FdoGeometry package.
 *  See related functions odbcdr_geom_*().
 */

typedef PBYTE       odbcdr_geom_def;        /* SqlServer geometry (image)   */
typedef SQLLEN      odbcdr_geomNI_def;      /* Null indicator from SqlServer */

typedef ut_da_def   odbcdr_geom_list_def;   /* List of odbcdr_geom_def *'s   */
typedef ut_da_def   odbcdr_geomNI_list_def; /* List of odbcdr_geomNI_def *'s*/

typedef struct odbcdr_geom_col_def {        /* A bind/define column             */
    int                     position;       /* Column # (1-based)               */
    pIGeometry_def          *address;       /* Caller's bind/define address     */
	pIGeometry_def			*l_address;		/* Copies of caller's define geometries addresses */
    odbcdr_geom_list_def    geom_list;      /* List of ocidr_geom_def *'s       */
    odbcdr_geomNI_list_def  geomNI_list;    /* List of ocidr_geomNullInd_def *'s */
} odbcdr_geom_col_def;

typedef ut_da_def odbcdr_geom_col_list_def;  /* List of ocidr_geom_col_def's */

typedef	struct _odbcdr_object_def_ {	     /* Database Object Description	*/
	char			name[ODBCDR_OBJECT_NAME_SIZE];
	char			owner[ODBCDR_OBJECT_NAME_SIZE];
	char			type[ODBCDR_OBJECT_TYPE_SIZE];
} odbcdr_object_def;


/*
** Mapping of a bind variable's position and its placeholder text.
** Required because RDBI specifies bind variables by name,
** but ODBC specifies them by positions (ie. "?")
*/
typedef struct bindname_map_def {
	char	*name;	 						/* Name of the bind variable		*/
	int		position; 						/* position of the bind variable	*/
	int		is_filled;						/* Is this bindame map element assigned */
} bindname_map;


typedef struct cursor_def {					/* Statement linked list		*/
#ifdef _DEBUG
    union
    {
	    char				*sqlstring;			/* The raw sql string.			*/
	    wchar_t				*sqlstringW;			/* The raw sql string.			*/
    };
#endif
	odbcdr_cursor_handle_def	hStmt;		/* ODBC statement handle		*/
	struct cursor_def	*next;				/* Next statement				*/
	long				cumul_rows_fetched;	/* Cumulative # rows fetched	*/
	int					is_rollback;		/* Flags a rollback statement */

	// SqlServer doesn't have native geometry types, so we use BLOB (image) data to store geometries.
    odbcdr_geom_col_list_def *bound_geometries;   /* odbcdr -->SqlServer buffer  */
    odbcdr_geom_col_list_def *defined_geometries; /* SqlServer-->odbcdr buffer  */
 
    SQLLEN		        lenDataParam;

    PBYTE               odbcdr_geom;    /* define a working area for fething bound geoms */
    SQLLEN              *odbcdr_geomNI;

    PBYTE               odbcdr_blob_tmp;    /* define a working area for fething geoms as Blobs*/
    int                 odbcdr_blob_tmp_size;

} odbcdr_cursor_def;


typedef struct connData_def {			/* Logon Data Area Definition	*/
    union
    {
    	char				db_name[RDBI_DB_NAME_SIZE];	/* Database name		*/
    	wchar_t				db_nameW[RDBI_DB_NAME_SIZE];	/* Database name		*/
    };
	odbcdr_service_ctx_def	hDbc;		/* ODBC database conenction handle */
	unsigned long		dbversion;		/* ? Oracle server version VrrmmPPpp */
	unsigned long		apiversion;		/* ? Oracle OCI API version		*/
	odbcdr_cursor_def	*cols;			/* for odbcdr_col_{act,get}		*/
	odbcdr_cursor_def	*keys;			/* for odbcdr_pkeys_{act,get}	*/
	odbcdr_cursor_def	*users;			/* for odbcdr_users_{act,get}	*/
	odbcdr_cursor_def	*objects;		/* for odbcdr_objects_{act,get}	*/
	odbcdr_cursor_def	*usr_exists;	/* for odbcdr_usr_exists		*/
	odbcdr_cursor_def	*cursors;		/* Head of linked list of statements */
	int					identity_id;		/* value						*/
	SQLLEN				identity_ni;		/* null indicator				*/
	odbcdr_cursor_def	*identity;			/* for get_gen_id()        		*/
    odbcdr_DriverType   driver_type;    /* What kind of database        */
    int                 skip_sybase_autogen_id_workaround; /* Defect workaround. */

} odbcdr_connData_def;

#define ODBCDRV_STRING_COMPARE(pStr1, pStr2) (context->odbcdr_UseUnicode ? \
    wcscmp((pStr1)->cwString, (pStr2)->cwString) : \
    strcmp((pStr1)->ccString, (pStr2)->ccString))
#define ODBCDRV_STRING_COMPARE_LST(pStr1, pStr2) (context->odbcdr_UseUnicode ? \
    wcscmp((pStr1)->cwString, pStr2##W) : \
    strcmp((pStr1)->ccString, pStr2))
#define ODBCDRV_STRING_COMPARE_CST(pStr1, pStr2) (context->odbcdr_UseUnicode ? \
    wcscmp(pStr1##W, pStr2##W) : \
    strcmp(pStr1, pStr2))
#define ODBCDRV_STRING_COPY_ST(pStr1, pStr2)  \
    if (context->odbcdr_UseUnicode) \
        wcscpy((pStr1)->wString, (pStr2)->wString); \
    else \
        strcpy((pStr1)->cString, (pStr2)->cString);
#define ODBCDRV_STRING_COPY_LST(pStr1, pStr2)  \
    if (context->odbcdr_UseUnicode) \
        wcscpy((pStr1)->wString, pStr2##W); \
    else \
        strcpy((pStr1)->cString, pStr2);
#define ODBCDRV_STRING_COPY_RST(pStr1, pStr2)  \
    if (context->odbcdr_UseUnicode) \
        wcscpy(pStr1##W, (pStr2)->wString); \
    else \
        strcpy(pStr1, (pStr2)->cString);
#define ODBCDRV_STRING_COPY_CST(pStr1, pStr2)  \
    if (context->odbcdr_UseUnicode) \
        wcscpy(pStr1##W, pStr2##W); \
    else \
        strcpy(pStr1, pStr2);

#define ODBCDRV_STRING_EMPTY(pStr1) (NULL == (pStr1)->cwString || (context->odbcdr_UseUnicode ? *(pStr1)->cwString == L'\0' : *(pStr1)->ccString == '\0'))

#define ODBCDRV_STRING_COMPARE_NOCASE_CST(pStr1, pStr2, sz) (context->odbcdr_UseUnicode ? \
    _wcsnicmp((pStr1)->cwString, pStr2##W, sz) : \
    _strnicmp((pStr1)->ccString, pStr2, sz))

#endif
