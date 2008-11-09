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

#include <stdio.h>
#include <stdlib.h>
#include <Inc/ut.h>
#include <Inc/debugext.h>
#include <Inc/rdbi.h>					/* rdbi status values		*/

#include "proto_p.h"
#include "structs.h"

// for SQL_PRESERVE_CURSORS
#ifdef _WIN32
#include "odbcss.h"
#endif


static int do_connect(odbcdr_context_def *context, int connect_id, rdbi_string_def *uid, rdbi_string_def *pswd, rdbi_string_def *connect_string);
static int alter_session(void);
static int get_dbversion(unsigned long *dbversion);
static int get_drivertype(odbcdr_connData_def * connData, odbcdr_DriverType *driver_type);
static void DumpError2( SQLSMALLINT eHandleType, SQLHANDLE hodbc);
static void DumpError2W( SQLSMALLINT eHandleType, SQLHANDLE hodbc);

/************************************************************************
* Name																	*
*	odbcdr_connect - Establish a connection to an ODBC RDBMS			*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_connect(connect_string, user, pswd, vendor_data, connect_id)
*	char		 *connect_string;										*
*	char		 *user; 												*
*	char		 *pswd; 												*
*	char		**vendor_data;											*
*	int 		 *connect_id;											*
*																		*
* Description															*
*		Establish  a  connection  to  the  given user space in an		*
*		ORACLE RDBMS.													*
*																		*
* Parameters															*
*	connect_string: input												*
*		A pointer to a null terminated string which could be used		*
*		to define the network host id  and	LAN  driver protocol.		*
*		At the moment, this string is used as follows:					*
*			If the string contains "default", an automatic logon		*
*				to the user's default database is initiated.			*
*			If the string is non-empty and is not "default", it 		*
*				is used directly as the database connection string		*
*				to ORACLE.												*
*			If the string is empty or is "default", the value of		*
*				the environment variable "TWO_TASK" is used as the		*
*				database connection string. 							*
*				If "TWO_TASK" is not available, the symbol				*
*					ODBCDR_CONNECT_STRING_DEFAULT is used.				*
*			If the string is not empty and the database is the          *
*			owner database, an automatic login to the master		    *
*			database on the host is done.								*
*																		*
*	user:			input												*
*		The name of the user space (aka database) that	is	to be		*
*		connected  to.	 This  name  is  saved	and  is  used  to		*
*		determine which  RDBMS	connection to switch to.  (Hence,		*
*		don't supply the same name twice).								*
*																		*
*	pswd:			input												*
*		A pointer to a	null  terminated  character  string which		*
*		defines the password to the given user space.	Note that		*
*		this  password really is used by ORACLE (which might  not		*
*		be the	case  with	other  drivers), in addition to being		*
*		checked at a higher level.										*
*																		*
*	vendor_data:	output												*
*		A pointer to a place to  put the address of any allocated		*
*		vendor-specific (ORACLE)  control  structure area related		*
*		to the connection to  be  established.	 This is actually		*
*		just  set to NULL by the driver.   Since  vendor-specific		*
*		info  is   needed	in	other  odbcdr_*()  function	on	a		*
*		per-connection basis, and this	parameter isn't passed to		*
*		those other functions, we do  the  needed  record-keeping		*
*		internally (in the odbcdr_conns[] array).						*
*																		*
*	connect_id: 	output												*
*		A  number	that   uniquely  identifies  the  established		*
*		database  connection.	This  can	later  be  passed  to		*
*		odbcdr_switch().   The value is only  meaningful  if  this		*
*		function's return value is RDBI_SUCCESS.						*
*																		*
* Function Value														*
*		An RDBI  status  integer.	Good  is RDBI_SUCCESS (ie 0).		*
*		RDBI_TOO_MANY_CONNECTS	will  be returned if this  is  an		*
*		attempt  to exceed	RDBI_MAX_CONNECTS  connections.   See		*
*		inc/rdbi.h														*
*																		*
* Remarks																*
*		Call odbcdr_rdbi_init()	before	calling this or any other		*
*		odbcdr_*() routine! Then, this routine should be the first		*
*		of any other odbcdr_*()	routines  called  that	require a		*
*		database  connection.	Note  that	you  do  not  need	a		*
*		database connected to call odbcdr_vndr_name().					*
*																		*
*		This  module  is  not  compiled  directly, but rather  is		*
*		catenated  with any other Pro*C modules  into  one	file.		*
*																		*
*		Automatic logons generally ignore the user/pswd parameters,		*
*		except in the case a program running under the effective uid	*
*		of the vision owner, and a real uid of another user.  In this	*
*		case, `user' is used, and `pswd' is used if it is non-empty;	*
*		if `pswd' is empty, the Vision owner's password is looked up. 	*
*																		*
************************************************************************/

int local_odbcdr_connect(
    odbcdr_context_def *context,
	rdbi_string_def  *connect_string,
	rdbi_string_def  *user,
	rdbi_string_def  *pswd,
	char **vendor_data,
	int   *connect_id
	)
{
	int 				i;
	int 				found_connect;
	int					default_connect		= FALSE;
	odbcdr_connData_def	*new_connData;
	wchar_t				user_localBuf[ODBCDR_CONNECTION_SIZE];
    rdbi_string_def     user_local;
	wchar_t				pswd_localBuf[ODBCDR_CONNECTION_SIZE];
    rdbi_string_def     pswd_local;
	wchar_t				connect_string_localBuf[ODBCDR_CONNECTION_SIZE];
    rdbi_string_def     connect_string_local;
	int					rdbi_status = RDBI_GENERIC_ERROR;
    connect_string_local.wString = connect_string_localBuf;
    user_local.wString = user_localBuf;
    pswd_local.wString = pswd_localBuf;
#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
        debug_on1("odbcdr_connect", "connect_string '%ls'", connect_string->cwString );
        debug1( "user '%ls'", user->cwString );
    }else{
        debug_on1("odbcdr_connect", "connect_string '%s'", connect_string->ccString );
        debug1( "user '%s'", user->ccString );
    }
#endif

	new_connData = (odbcdr_connData_def *)NULL;
	if( context->odbcdr_connect_count >= RDBI_MAX_CONNECTS ) {
		rdbi_status = RDBI_TOO_MANY_CONNECTS;
		goto the_exit;
	}

	if( ODBCDRV_STRING_EMPTY(user) && ODBCDRV_STRING_EMPTY(connect_string) )
    {
		rdbi_status = RDBI_INVLD_USER_PSWD;
		context->odbcdr_last_rc = ODBCDR_INVLD_USER_PSWD;
		goto the_exit;
	}
	/*
	 * Find an entry for the new connection.
	 */
	for(		i = 0;
				i < RDBI_MAX_CONNECTS && new_connData == (odbcdr_connData_def *)NULL;
				i++ ) {
		if( context->odbcdr_conns[i] == (odbcdr_connData_def *)NULL ) {

			new_connData = (odbcdr_connData_def *) ut_vm_malloc( "odbcdr_connectW",
													sizeof( odbcdr_connData_def ) );
			if( new_connData == (odbcdr_connData_def *)NULL ) {
				rdbi_status = RDBI_MALLOC_FAILED;
				goto the_exit;
			}
			memset((char *)new_connData, '\0', sizeof(*new_connData));
			context->odbcdr_conns[i] = new_connData;
            if (user->cwString != NULL)
                ODBCDRV_STRING_COPY_RST( new_connData->db_name, user );
			new_connData->dbversion = ODBCDR_DBVERS8;  /* MS SQL Sever 5K */

			found_connect = i;
		} 
	} 

	/* And now for a sanity check:	If we didn't find an connection entry,
	 * but we haven't exceeded the limit of RDBI_MAX_CONNECTS (which was
	 * already checked), there is a bug!
	 */

	if( new_connData == (odbcdr_connData_def *)NULL ) {
		rdbi_status = RDBI_GENERIC_ERROR;
		goto the_exit;
	}

	debug2("conn# %d, conn 0x%p", found_connect, new_connData);

    /* Get local versions of input parameters, guaranteeing that there
     * are no NULL's.
     */
    *connect_string_local.wString = L'\0';
    *user_local.wString = L'\0';
    *pswd_local.wString = L'\0';
    if( connect_string->cwString != NULL )
		ODBCDRV_STRING_COPY_ST( &connect_string_local, connect_string );
    if (user->cwString != NULL)
		ODBCDRV_STRING_COPY_ST( &user_local, user );
    if (pswd->cwString != NULL)
		ODBCDRV_STRING_COPY_ST( &pswd_local, pswd );

	rdbi_status = do_connect( context,
                              found_connect,
							  &user_local,
							  &pswd_local,
							  &connect_string_local);

	/*
	 * No specific connect area required -- we keep it internal
	 * to this library anyway.
	 */
    if (vendor_data != NULL)
	    *vendor_data = NULL;
	if( rdbi_status != RDBI_SUCCESS ) {
		debug0( "connect FAILED!" );
		goto the_exit;
	} else {
		debug0( "connect succeeded." );
	}
	new_connData = (odbcdr_connData_def *)NULL;

the_exit:

	if (rdbi_status != RDBI_SUCCESS) {
		if (new_connData != (odbcdr_connData_def *)NULL)
		{
            if (SQL_NULL_HDBC != new_connData->hDbc)
                SQLFreeHandle(SQL_HANDLE_DBC, new_connData->hDbc);
			ut_vm_free( "odbcdr_connect: conn Data", (char *)new_connData );
			context->odbcdr_conns[found_connect] = (odbcdr_connData_def *)NULL;
		}
	} else {
		*connect_id 		  = found_connect;
		context->odbcdr_current_connect = found_connect;
		context->odbcdr_connect_count ++ ;
		rdbi_status = odbcdr_set_sess_env( context );
	}
	debug_area() odbcdr_show_context( context, (odbcdr_cursor_def *)NULL );
	debug_return(NULL, rdbi_status);
}

int odbcdr_connect(
    odbcdr_context_def *context,
	const char  *connect_string,
	const char  *user,
	const char  *pswd,
	char **vendor_data,
	int   *connect_id
	)
{
    rdbi_string_def strConnect_string;
    rdbi_string_def strUser;
    rdbi_string_def strPswd;
    strConnect_string.ccString = connect_string;
    strUser.ccString = user;
    strPswd.ccString = pswd;
    return local_odbcdr_connect(context, &strConnect_string, &strUser, &strPswd, vendor_data, connect_id);
}

int odbcdr_connectW(
    odbcdr_context_def *context,
	const wchar_t  *connect_string,
	const wchar_t  *user,
	const wchar_t  *pswd,
	char **vendor_data,
	int   *connect_id
	)
{
    rdbi_string_def strConnect_string;
    rdbi_string_def strUser;
    rdbi_string_def strPswd;
    strConnect_string.cwString = connect_string;
    strUser.cwString = user;
    strPswd.cwString = pswd;
    return local_odbcdr_connect(context, &strConnect_string, &strUser, &strPswd, vendor_data, connect_id);
}

static
int	do_connect(
    odbcdr_context_def *context,
	int				connect_id,
	rdbi_string_def *uid,
	rdbi_string_def	*pswd,
	rdbi_string_def	*connect_string
	)
{
	wchar_t         sql_buf[50];
    rdbi_string_def sqlval;
	SQLHSTMT        hStmt5;
	SQLRETURN       rc;
	int	            rdbi_status = RDBI_GENERIC_ERROR;
    wchar_t         szOutConnBuf[1024];
    rdbi_string_def szOutConn;
    SQLSMALLINT     cbOutConn;
    szOutConn.wString = szOutConnBuf;
    sqlval.wString = sql_buf;

#ifdef _DEBUG
    if (context->odbcdr_UseUnicode){
	    debug_on3("odbcdr_connectW:do_connect", "cid: %d, uid: '%ls', dblink: '%ls'",
            connect_id, uid->cwString, connect_string->cwString);
    }else{
	    debug_on3("odbcdr_connectW:do_connect", "cid: %d, uid: '%s', dblink: '%s'",
            connect_id, uid->ccString, connect_string->ccString);
    }
#endif

	/*
	** Set up ODBC environment (only once for all connections).
	*/
	if ( context->odbcdr_env == SQL_NULL_HENV ) {
		// Allocate the environment handle
		context->odbcdr_env = SQL_NULL_HENV;
		rc = SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HENV, &context->odbcdr_env);
		// Set ODBC version 
		rc = SQLSetEnvAttr(context->odbcdr_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	}	
	
	/*
	** Attach to the ODBC data source.
	*/
	{	
		// Allocate the connection handle (svc)
		odbcdr_service_ctx_def	hDbc = SQL_NULL_HDBC;
		rc = SQLAllocHandle(SQL_HANDLE_DBC,	context->odbcdr_env, &hDbc);

		context->odbcdr_conns[connect_id]->hDbc = hDbc;
		context->odbcdr_conns[connect_id]->apiversion = SQL_OV_ODBC3; //CMB: check this
#ifdef _WIN32
        // For SQL Server ODBC Driver only: enable SQL_COPT_SS_PRESERVE_CURSORS (Do not close open server cursors if a transaction is committed);
        // This connection setting must be set before connecting and we do not have enough info about the driver at this point to determine
        // whether or not we are using SQL Server or another ODBC data source, so we always set it.
        // Other drivers should safely ignore this setting since it is SQLServer-specific, so ignore the returned error code:
        if (context->odbcdr_UseUnicode)
            rc = SQLSetConnectAttrW(hDbc, SQL_COPT_SS_PRESERVE_CURSORS, (SQLPOINTER)SQL_PC_ON, SQL_IS_INTEGER); 
        else
            rc = SQLSetConnectAttr(hDbc, SQL_COPT_SS_PRESERVE_CURSORS, (SQLPOINTER)SQL_PC_ON, SQL_IS_INTEGER); 
#endif

        /* There are two distinct cases:
         * 1. The "connect_string" is just that -- a fully-qualified
         *    parameter to establish a connection. It can contain uid and 
		 *	  password, but these parameters are optional.
		 *    If the uid and password are not specified the connection is 
		 *    authenticated using the windows authentication method; otherwise,
		 *    the connection is authenticated using the SQL Server method.
         * 2. The "connect_string" is really just a datastore name, is used
         *    in a datastore/userid/password combination.  Some connections
         *    may not actually need the userid or password.
         *
         * This overloading of the connect_string parameter could be improved,
         * but we'll deal with it as-is to avoid disrupting RDBI and other
         * drivers.  To distinguish the cases, we'll take advantage of the fact
         * that, in ODBC, a true connection string needs at least two parameters
         * (driver name and one of several tags denoting the datastore).  Thus, 
         * the 2+ parameters must be separated.  The separator is a semicolon.
         */
        if (context->odbcdr_UseUnicode)
        {
            if (NULL == wcschr (connect_string->cwString, L';'))
            {
                /* One parameter -- this must be an ODBC Data Source Name (DSN). */
                rc = SQLConnectW(hDbc, (SQLWCHAR*)connect_string->cwString, SQL_NTS,
                    (SQLWCHAR*)uid->cwString, SQL_NTS,(SQLWCHAR*)pswd->cwString, SQL_NTS);
            }
            else
            {
                /* Two or more parameters -- treat it as a real connection string. */
                rc = SQLDriverConnectW(hDbc, NULL, (SQLWCHAR*) connect_string->cwString, SQL_NTS, 
                    (SQLWCHAR*)szOutConn.wString, 1024, &cbOutConn, SQL_DRIVER_NOPROMPT);
            }
        }
        else
        {
            if (NULL == strchr (connect_string->ccString, ';'))
            {
                /* One parameter -- this must be an ODBC Data Source Name (DSN). */
                rc = SQLConnect(hDbc, (SQLCHAR*)connect_string->ccString, SQL_NTS,
                    (SQLCHAR*)uid->ccString, SQL_NTS, (SQLCHAR*)pswd->ccString, SQL_NTS);
            }
            else
            {
                /* Two or more parameters -- treat it as a real connection string. */
                rc = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)connect_string->ccString, SQL_NTS, 
                    (SQLCHAR*)szOutConn.cString, 1024, &cbOutConn, SQL_DRIVER_NOPROMPT);
            }
        }

        if ( rc == SQL_ERROR )
        {
            if (context->odbcdr_UseUnicode)
                DumpError2W(SQL_HANDLE_DBC, hDbc);
            else
                DumpError2(SQL_HANDLE_DBC, hDbc);
        } else {
            rdbi_status = RDBI_SUCCESS;
 		
            /* Now that we have a connection, we can inspect for driver type more thoroughly. */
            rdbi_status = get_drivertype(
                context->odbcdr_conns[connect_id],
                &context->odbcdr_conns[connect_id]->driver_type);

            if (ODBCDriverType_SQLServer == context->odbcdr_conns[connect_id]->driver_type)
            {
                if (context->odbcdr_UseUnicode)
                {
			        rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt5);
                    odbcdr_swprintf(sqlval.wString, 50, L"SET NOCOUNT OFF");
                    rc = SQLExecDirectW(hStmt5, (SQLWCHAR*)sqlval.wString, SQL_NTS);

			        rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);

		            /*
		            ** Set up default connection properties
		            */
		            // Use server side cursors by default
		            rc = SQLSetConnectAttrW(hDbc, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, SQL_IS_INTEGER); 
		            rc = SQLSetConnectAttrW(hDbc, SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_READ_ONLY, SQL_IS_INTEGER); 
		            rc = SQLSetConnectAttrW(hDbc, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)1, SQL_IS_INTEGER); 
                    // TODO: investigate "[Microsoft][ODBC Driver Manager] Option type out of range"
                    //if ( rc == SQL_ERROR )
                    //    DumpError2(SQL_HANDLE_DBC, hDbc);

		            // Operate in syncronous mode
		            rc = SQLSetConnectAttrW(hDbc, SQL_ATTR_ASYNC_ENABLE, SQL_ASYNC_ENABLE_OFF, SQL_IS_INTEGER);
                    if ( rc == SQL_ERROR )
                        DumpError2W(SQL_HANDLE_DBC, hDbc);

		            // Use autocommit mode
		            rc = SQLSetConnectAttrW(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
                    if ( rc == SQL_ERROR )
                        DumpError2W(SQL_HANDLE_DBC, hDbc);

                }
                else
                {
			        rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt5);
                    sprintf(sqlval.cString, "SET NOCOUNT OFF");
                    rc = SQLExecDirect(hStmt5, (SQLCHAR*)sqlval.ccString, SQL_NTS);

			        rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);

		            /*
		            ** Set up default connection properties
		            */
		            // Use server side cursors by default
		            rc = SQLSetConnectAttr(hDbc, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_STATIC, SQL_IS_INTEGER); 
		            rc = SQLSetConnectAttr(hDbc, SQL_ATTR_CONCURRENCY, (SQLPOINTER)SQL_CONCUR_READ_ONLY, SQL_IS_INTEGER); 
		            rc = SQLSetConnectAttr(hDbc, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)1, SQL_IS_INTEGER); 
                    // TODO: investigate "[Microsoft][ODBC Driver Manager] Option type out of range"
                    //if ( rc == SQL_ERROR )
                    //    DumpError2(SQL_HANDLE_DBC, hDbc);

		            // Operate in syncronous mode
		            rc = SQLSetConnectAttr(hDbc, SQL_ATTR_ASYNC_ENABLE, SQL_ASYNC_ENABLE_OFF, SQL_IS_INTEGER);
                    if ( rc == SQL_ERROR )
                        DumpError2(SQL_HANDLE_DBC, hDbc);

		            // Use autocommit mode
		            rc = SQLSetConnectAttr(hDbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
                    if ( rc == SQL_ERROR )
                        DumpError2(SQL_HANDLE_DBC, hDbc);

                }
            }
            else if (ODBCDriverType_MySQL == context->odbcdr_conns[connect_id]->driver_type)
            {
                if (context->odbcdr_UseUnicode)
                {
                    rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt5);
                    odbcdr_swprintf(sqlval.wString, 50, L"set sql_mode='ANSI_QUOTES'");
                    rc = SQLExecDirectW(hStmt5, (SQLWCHAR*)sqlval.cwString, SQL_NTS);
                    rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);
                }
                else
                {
                    rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt5);
                    sprintf(sqlval.cString, "set sql_mode='ANSI_QUOTES'");
                    rc = SQLExecDirect(hStmt5, (SQLCHAR*)sqlval.ccString, SQL_NTS);
                    rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);
                }
            }
        }
	}	/* end of ODBC connection block */
    
	debug_return(NULL, rdbi_status);
}


static int
alter_session()
{
	// alter session not implemented
	return RDBI_SUCCESS;
}

static int
get_dbversion(
	unsigned long		*dbversion
	)
{
	// db version not implemented
	return RDBI_SUCCESS;
}

static int
get_drivertype(
    odbcdr_connData_def * connData,
    odbcdr_DriverType *driver_type
    )
{
    int rdbi_status = RDBI_GENERIC_ERROR;

    SQLCHAR    szDriverName[128];
    SQLRETURN ret = SQL_SUCCESS;
//SQLUINTEGER info = 0;
//ret = SQLGetInfo(connData->hDbc, SQL_GETDATA_EXTENSIONS, (SQLPOINTER)&info, 0, NULL);



    *driver_type = ODBCDriverType_Undetermined;

    ret = SQLGetInfo(connData->hDbc, SQL_DRIVER_NAME, szDriverName, sizeof(szDriverName), NULL);
    if (ret != SQL_SUCCESS)
        goto the_exit;

    if ( (0==_stricmp((const char *) szDriverName, ODBCDR_DRIVER_ORACLE_DRIVERNAME_WINDOWS_MB))
      || (0==_strnicmp((const char*) szDriverName, ODBCDR_DRIVER_ORACLE_DRIVERNAME_LINUX_MB, strlen(ODBCDR_DRIVER_ORACLE_DRIVERNAME_LINUX_MB))) )
    {
        *driver_type = ODBCDriverType_OracleNative;
    }
    else if (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_SQLSERVER_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_SQLServer;
    }
    else if (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_SYBASE_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_Sybase;
    }
    else if (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_ACCESS_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_Access;
    }
    // NOTE: the MySQL ODBC reported driver names are actually *reversed* on Windows and Linux,
    //       i.e. On Windows the reported driver name is "libmyodbc3.so" and on Linux the reported driver name is "myodbc3.dll"
    else if ((0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_MYSQL_DRIVERNAME_LINUX_MB))
	     || (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_MYSQL_DRIVERNAME_WINDOWS_MB)))

    {
        *driver_type = ODBCDriverType_MySQL;
    }
    else
    {
        SQLCHAR    szDbmsName[128];
        char *ptr= (char *) szDbmsName;

        ret = SQLGetInfo(connData->hDbc, SQL_DBMS_NAME, szDbmsName, sizeof(szDbmsName), NULL);
        if (ret != SQL_SUCCESS)
            goto the_exit;
        while (ptr[0] != '\0')
        {
            *ptr = toupper(*ptr);
            ptr++;
        }

        if (strstr((const char *) szDbmsName, "ORACLE") != NULL)
        {
            *driver_type = ODBCDriverType_OracleNonNative;
        }
        else
        {
            *driver_type = ODBCDriverType_Other;
        }
    }

    rdbi_status = RDBI_SUCCESS;

the_exit:
    return rdbi_status;
}

#ifndef _WIN32

static void DumpError2
    (
    SQLSMALLINT eHandleType,
    SQLHANDLE hodbc
    )
{
}
static void DumpError2W
    (
    SQLSMALLINT eHandleType,
    SQLHANDLE hodbc
    )
{
}

#else

#include <tchar.h>

static void DumpError2
    (
    SQLSMALLINT eHandleType,
    SQLHANDLE hodbc
    )
    {
    SQLCHAR    szState[SQL_SQLSTATE_SIZE + 1];
    SQLCHAR    szMessage[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLINTEGER  nServerError;
    SQLSMALLINT cbMessage;
    UINT        nRec = 1;

    while (SQL_SUCCEEDED(SQLGetDiagRec(eHandleType, hodbc, nRec, szState,
        &nServerError, szMessage, SQL_MAX_MESSAGE_LENGTH + 1, &cbMessage)))
        {
#ifdef _DEBUG
        printf("Message: %s\n", szMessage);
#endif
        nRec++;
        }

    return;
    }
static void DumpError2W
    (
    SQLSMALLINT eHandleType,
    SQLHANDLE hodbc
    )
    {
    SQLWCHAR    szState[SQL_SQLSTATE_SIZE + 1];
    SQLWCHAR    szMessage[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLINTEGER  nServerError;
    SQLSMALLINT cbMessage;
    UINT        nRec = 1;

    while (SQL_SUCCEEDED(SQLGetDiagRecW(eHandleType, hodbc, nRec, szState,
        &nServerError, szMessage, SQL_MAX_MESSAGE_LENGTH + 1, &cbMessage)))
        {
#ifdef _DEBUG
        wprintf(L"Message: %ls\n", szMessage);
#endif
        nRec++;
        }

    return;
    }
#endif

