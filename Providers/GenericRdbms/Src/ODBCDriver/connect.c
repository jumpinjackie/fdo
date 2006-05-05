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


static int do_connect(odbcdr_context_def *context, int connect_id, char *uid, char *pswd, char *connect_string, odbcdr_DriverType driver_type);
static int alter_session(void);
static int get_dbversion(unsigned long *dbversion);
static int get_drivertype(odbcdr_connData_def * connData, odbcdr_DriverType *driver_type);
static void DumpError2( SQLSMALLINT eHandleType, SQLHANDLE hodbc);


int odbcdr_connect(
    odbcdr_context_def *context,
	char  *connect_string,
	char  *user,
	char  *pswd,
	char **vendor_data,
	int   *connect_id
	)
{
	int 				i;
	int 				found_connect;
	int					default_connect		= FALSE;
	odbcdr_connData_def	*new_connData;
	char				user_local[ODBCDR_CONNECTION_SIZE];
	char				pswd_local[ODBCDR_CONNECTION_SIZE];
	char				connect_string_local[ODBCDR_CONNECTION_SIZE];
    odbcdr_DriverType   driver_type = ODBCDriverType_Undetermined;
	int					rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1("odbcdr_connect", "connect_string '%s'", (connect_string==NULL) );
	debug1( "user '%s'", (user==NULL) );

	new_connData = (odbcdr_connData_def *)NULL;
	if( context->odbcdr_connect_count >= RDBI_MAX_CONNECTS ) {
		rdbi_status = RDBI_TOO_MANY_CONNECTS;
		goto the_exit;
	}

	if( (user == NULL || user[0] == '\0') && 
        (connect_string == NULL || connect_string[0] == '0') ) {

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

			new_connData = (odbcdr_connData_def *) ut_vm_malloc( "odbcdr_connect",
													sizeof( odbcdr_connData_def ) );
			if( new_connData == (odbcdr_connData_def *)NULL ) {
				rdbi_status = RDBI_MALLOC_FAILED;
				goto the_exit;
			}
			memset((char *)new_connData, '\0', sizeof(*new_connData));
			context->odbcdr_conns[i] = new_connData;
            if (user != NULL)
    			strcpy( new_connData->db_name, user );
			new_connData->dbversion = ODBCDR_DBVERS8;  /* MS SQL Sever 5K */

	        if( (connect_string != NULL && connect_string[0] != '0') ) {
                if (NULL != strstr(connect_string, "DRIVER={SQL Server}"))
                    driver_type = ODBCDriverType_SQLServer;
            }

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

	connect_string_local[0] = '\0';
    user_local[0] = '\0';

    /* Get local versions of input parameters, guaranteeing that there
     * are no NULL's.
     */
	connect_string_local[0] = '\0';
    user_local[0] = '\0';
    pswd_local[0] = '\0';
	if( connect_string != NULL )
		(void) strcpy( connect_string_local, connect_string );
	if (user != NULL)
		(void) strcpy( user_local, user );
	if (pswd != NULL)
		(void) strcpy( pswd_local, pswd );

	rdbi_status = do_connect( context,
                              found_connect,
							  user_local,
							  pswd_local,
							  connect_string_local,
                              driver_type );

	/*
	 * No specific connect area required -- we keep it internal
	 * to this library anyway.
	 */
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
			ut_vm_free( "odbcdr_connect: conn Data", (char *)new_connData );
			context->odbcdr_conns[found_connect] = (odbcdr_connData_def *)NULL;
		}
	} else {
		*connect_id 		  = found_connect;
		context->odbcdr_current_connect = found_connect;
		context->odbcdr_connect_count ++ ;
	}
	debug_area() odbcdr_show_context( context, (odbcdr_cursor_def *)NULL );
	debug_return(NULL, rdbi_status);
}


static
int	do_connect(
    odbcdr_context_def *context,
	int				 connect_id,
	char			*uid,
	char			*pswd,
	char			*connect_string,
    odbcdr_DriverType driver_type
	)
{

	char        sql_buf[50];
	SQLHSTMT    hStmt;
	SQLHSTMT    hStmt5;
	SQLRETURN   rc;
	int	        rdbi_status = RDBI_GENERIC_ERROR;
    char        szInConn[100];
    char        szOutConn[1024];
    const char *paramDelimiter;
    SQLSMALLINT cbOutConn;

	debug_on3("odbcdr_connect:do_connect", "cid: %d, uid: '%s', dblink: '%s'",
			connect_id, ISNULL((char *)uid), ISNULL((char *)connect_string));

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

		/*
		** Set up default connection properties
		*/
        if (ODBCDriverType_SQLServer == driver_type)
        {
		    // Do not close open server cursors if a transaction is committed
#ifdef _WIN32
		    rc = SQLSetConnectAttr(hDbc, SQL_COPT_SS_PRESERVE_CURSORS, (SQLPOINTER)SQL_PC_ON, SQL_IS_INTEGER); 
#endif

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
        paramDelimiter = strchr (connect_string, ';');
        if (NULL == paramDelimiter)
        {
            /* One parameter -- this must be an ODBC Data Source Name (DSN). */
            rc = SQLConnect(hDbc, 
                                 (SQLCHAR*)connect_string, SQL_NTS,
                                 (SQLCHAR*)uid, SQL_NTS,
                                 (SQLCHAR*)pswd, SQL_NTS);
        }
        else
        {
            /* Two or more parameters -- treat it as a real connection string. */
            rc = SQLDriverConnect(hDbc, NULL,
                                 (SQLCHAR*) connect_string, SQL_NTS, 
                                 (SQLCHAR*)szOutConn,  sizeof(szOutConn), &cbOutConn, 
                                 SQL_DRIVER_NOPROMPT);
        }

        if ( rc == SQL_ERROR )  {
            DumpError2(SQL_HANDLE_DBC, hDbc);
        } else {
            rdbi_status = RDBI_SUCCESS;
 		
			context->odbcdr_conns[connect_id]->hDbc = hDbc;
			context->odbcdr_conns[connect_id]->apiversion = SQL_OV_ODBC3; //CMB: check this

            /* Now that we have a connection, we can inspect for driver type more thoroughly. */
            rdbi_status = get_drivertype(
                context->odbcdr_conns[connect_id],
                &context->odbcdr_conns[connect_id]->driver_type);

            if (ODBCDriverType_SQLServer == context->odbcdr_conns[connect_id]->driver_type)
            {
			    /*
			    ** switch to SQL Server database. Use 'master' database.
			    ** In SQL Server schema object namespace is specified by 
			    ** the database, not the login username
			    */
			    rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt);
                strcpy(sql_buf, "USE master" );
			    rc = SQLExecDirect(hStmt, (SQLCHAR*)sql_buf, SQL_NTS);
                DumpError2(SQL_HANDLE_DBC, hDbc);

			    rc = SQLAllocHandle(SQL_HANDLE_STMT,	hDbc,&hStmt5);
			    sprintf(sql_buf, "SET NOCOUNT OFF");
			    rc = SQLExecDirect(hStmt5, (SQLCHAR*)sql_buf, SQL_NTS);

			    //Destroy the statement handles
			    rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			    rc = SQLFreeHandle(SQL_HANDLE_STMT, hStmt5);
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

    *driver_type = ODBCDriverType_Undetermined;

    ret = SQLGetInfo(connData->hDbc, SQL_DRIVER_NAME, szDriverName, sizeof(szDriverName), NULL);
    if (ret != SQL_SUCCESS)
        goto the_exit;

    if (0==_stricmp((const char *) szDriverName, ODBCDR_DRIVER_ORACLE_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_OracleNative;
    }
    else if (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_SQLSERVER_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_SQLServer;
    }
    else if (0==_stricmp((const char*)szDriverName, ODBCDR_DRIVER_ACCESS_DRIVERNAME_MB))
    {
        *driver_type = ODBCDriverType_Access;
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

#else

#include <tchar.h>

static void DumpError2
    (
    SQLSMALLINT eHandleType,
    SQLHANDLE hodbc
    )
    {
    SQLTCHAR    szState[SQL_SQLSTATE_SIZE + 1];
    SQLTCHAR    szMessage[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLINTEGER  nServerError;
    SQLSMALLINT cbMessage;
    UINT        nRec = 1;

    while (SQL_SUCCEEDED(SQLGetDiagRec(eHandleType, hodbc, nRec, szState,
        &nServerError, szMessage, SQL_MAX_MESSAGE_LENGTH + 1, &cbMessage)))
        {
        _tprintf(_T("Message: %s\n"), szMessage);
        nRec++;
        }

    return;
    }
#endif

