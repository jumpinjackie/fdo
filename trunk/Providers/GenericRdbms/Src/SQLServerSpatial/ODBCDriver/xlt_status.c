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
*	odbcdr_xlt_stat - Translate the ODBC/SQL Server error code to		*
*		an RDBI one	and print out any error messages					*
*																		*
* Synopsis																*
*	#include <Inc/rdbi.h>												*
*	int odbcdr_xlt_stat( ocidr_status )									*
*		int ocidr_status;												*
*																		*
* Description															*
*		Translate	the  given	ODBC/SQL Server	error  code  to  an		*
*		 RDBI equivalent. 												*
*																		*
* Parameters															*
*	ocidr_status:	input												*
*		The ODBC/SQL Server error code.									*
*																		*	
*	handle_type:	input												*																			*
*		The ODBC handle type for the handle that the action whose		*
*		status we are converting occurred on (ex. SQL_HANDLE_STMT).		*
*																		*	
*	handle:			input												*																			*
*		The ODBC handle that the action whose status we are				*
*		converting occurred on (ex. SQL_HANDLE_STMT).					*
*																		*	
* Function Value														*
*		The RDBI equivalent status										*
*																		*
* Remarks																*
*		If we  don't  recognize  an  error	specifically, it gets		*
*		translated to RDBI_GENERIC_ERROR by default.					*
*																		*
************************************************************************/

#include <Inc/debugext.h>
#include <Inc/rdbi.h>				/* rdbi status values		*/
#include    <Inc/ut.h>

#include "proto_p.h"
#ifdef _WIN32
#include "odbcss.h"		    //used in DisplayErrMsg
#endif

static int get_error_from_diag_rec(odbcdr_context_def *context, SQLSMALLINT handle_type, SQLHANDLE handle);
static void save_err_msg( odbcdr_context_def *context, SQLSMALLINT plm_handle_type, SQLHANDLE plm_handle, int ConnInd);
static void save_err_msgW( odbcdr_context_def *context, SQLSMALLINT plm_handle_type, SQLHANDLE plm_handle, int ConnInd);

int
odbcdr_xlt_status(		 /* Return RDBI equiv of SQL Server status  */
    odbcdr_context_def  *context,
	SQLRETURN odbcdr_status,
	SQLSMALLINT handle_type,
	SQLHANDLE handle 	
	)
{
	int		rdbi_status = RDBI_GENERIC_ERROR;

	debug_on1( "odbcdr_xlt_status","ODBC status: %d", odbcdr_status );

	switch( odbcdr_status ) {

		case SQL_SUCCESS :
   		case SQL_SUCCESS_WITH_INFO :
			rdbi_status = RDBI_SUCCESS;
			break;

		/* DS: THIS IS NOT TRUE!
		** SQL_SUCESS_WITH_INFO is returned after executing array DML, if
		** there are ignorable errors or if a row operation failed.
		** Therefore, we need to search the diagnostic record to see if any
		** real errors occured.

        /*
		** In the SQL_ERROR case, we need to check if we can convert it to a
		** more specific database error type. The message is saved in the
		** context->odbcdr_last_err_msg field.
		*/
		case SQL_ERROR :
			rdbi_status = get_error_from_diag_rec(context, handle_type, handle);
			break;
		case SQL_NO_DATA :
			rdbi_status = RDBI_END_OF_FETCH;
			break;
		case SQL_NEED_DATA :
		case SQL_INVALID_HANDLE :
		case SQL_STILL_EXECUTING :
			rdbi_status = RDBI_GENERIC_ERROR;
            if (context->odbcdr_UseUnicode)
			    save_err_msgW(context, handle_type, handle, 1); // 1 = assuming a connection has already been made
            else
                save_err_msg(context, handle_type, handle, 1); // 1 = assuming a connection has already been made
			break;
		default:
			rdbi_status = RDBI_GENERIC_ERROR;
	}

	debug_return(NULL, rdbi_status);
}


static int get_error_from_diag_rec(
		odbcdr_context_def *context, 
		SQLSMALLINT handle_type, 
		SQLHANDLE handle)
{
	int		rdbi_status = RDBI_SUCCESS;
	int		crit_err_found = FALSE;
	RETCODE	rec_retcode = SQL_SUCCESS;
	RETCODE	field_retcode = SQL_SUCCESS;
	SQLWCHAR	szSqlState[ODBCDR_MAX_BUFF_SIZE];
	SQLWCHAR 	szErrorMsg[ODBCDR_MAX_BUFF_SIZE];
	SDWORD		pfNativeError = 0L;
	SWORD	 	pcbErrorMsg = 0;
	SQLSMALLINT cRecNmbr = 1;
	SDWORD  	SS_Severity = 0;
	SQLINTEGER	Rownumber = 0;
	SQLINTEGER  Colnumber = 0;
    szSqlState[0] = L'\0';
    szErrorMsg[0] = L'\0';

	/*
	** Loop through the diagnostic records until there are no records
	** left or a critical error is found.
	** For now, we will only deal with the first critical error, and
	** ignore the rest.
	*/
	while ((rec_retcode != SQL_NO_DATA_FOUND) && !crit_err_found) {
		rec_retcode = (context->odbcdr_UseUnicode) ? 
            SQLGetDiagRecW(handle_type, handle,
					cRecNmbr, szSqlState, &pfNativeError,
                    szErrorMsg, ODBCDR_MAX_BUFF_SIZE, &pcbErrorMsg) : 
            SQLGetDiagRec(handle_type, handle,
					cRecNmbr, (SQLCHAR*)szSqlState, &pfNativeError,
					(SQLCHAR*)szErrorMsg, ODBCDR_MAX_BUFF_SIZE, &pcbErrorMsg);
 
		if (rec_retcode != SQL_NO_DATA_FOUND) {
			field_retcode = (context->odbcdr_UseUnicode) ? 
                SQLGetDiagFieldW( handle_type, handle, cRecNmbr,
					SQL_DIAG_ROW_NUMBER, &Rownumber,
					SQL_IS_INTEGER, NULL) :
                SQLGetDiagField( handle_type, handle, cRecNmbr,
					SQL_DIAG_ROW_NUMBER, &Rownumber,
					SQL_IS_INTEGER, NULL);

			if (Rownumber != SQL_NO_ROW_NUMBER  && Rownumber != SQL_ROW_NUMBER_UNKNOWN)	{
				field_retcode = (context->odbcdr_UseUnicode) ? 
                    SQLGetDiagFieldW( handle_type, handle, cRecNmbr,
						SQL_DIAG_COLUMN_NUMBER , &Colnumber,
						SQL_IS_INTEGER, NULL) : 
                    SQLGetDiagField( handle_type, handle, cRecNmbr,
						SQL_DIAG_COLUMN_NUMBER , &Colnumber,
						SQL_IS_INTEGER, NULL);
			}
#ifdef _WIN32
			field_retcode = (context->odbcdr_UseUnicode) ? 
                SQLGetDiagFieldW( handle_type, handle, cRecNmbr,
					SQL_DIAG_SS_SEVERITY, &SS_Severity,
                    SQL_IS_INTEGER, NULL):
                SQLGetDiagField(handle_type, handle, cRecNmbr,
					SQL_DIAG_SS_SEVERITY, &SS_Severity,
					SQL_IS_INTEGER,NULL);
#endif

			switch( pfNativeError ) {
				case 208 :
					rdbi_status = RDBI_NO_SUCH_TABLE;
					crit_err_found = TRUE;
					break;
				case 916 :
					rdbi_status = RDBI_NOT_VALID_USER_IN_DATABASE;
					crit_err_found = TRUE;
					break;
				case 1555 :
					rdbi_status = RDBI_RESOURCE_BUSY;
					crit_err_found = TRUE;
					break;
				case 18456 :
					rdbi_status = RDBI_INVLD_USER_PSWD;
					crit_err_found = TRUE;
					break;
                case 913: 
                    // "Could not find database ID 52. Database may not be activated yet or may be in transition."
                    // Severity 16. This is rather a bug related to updating views.
                    rdbi_status = RDBI_GENERIC_ERROR;
                    break;
                case 2601:
				case 2627:
                    // Duplicate index found.
                    rdbi_status = RDBI_DUPLICATE_INDEX;
					crit_err_found = TRUE;
                    break;
				case 2714:
					// There is already an object named '%.*ls' in the database.
					rdbi_status = RDBI_OBJECT_EXISTS;
					crit_err_found = TRUE;
                    break;
				case 3701:
					// No such object
					rdbi_status = RDBI_NO_SUCH_TABLE;
					crit_err_found = TRUE;
					break;
				default :
					// If we did not identify any specific errors in the 
					// diagnostic record, use the severity level to 
					// determine if a error occurred
                    // *** DS doesn't agree.
					//if (SS_Severity > 10) {
						rdbi_status = RDBI_GENERIC_ERROR;
						crit_err_found = TRUE;
					//} else {
					//	rdbi_status = RDBI_SUCCESS;
					//}
			} //switch
			cRecNmbr++; //Increment to next diagnostic record.
		} //if plm_retcode

	} // End while.

    if (context->odbcdr_UseUnicode)
	    save_err_msgW( context, handle_type, handle, 1); // 1 = assuming a connection has already been made
    else
        save_err_msg( context, handle_type, handle, 1); // 1 = assuming a connection has already been made

	return rdbi_status;
}

///////////////////////////////////////////////////////////////////////////
static void save_err_msg( 
	odbcdr_context_def  *context, 
	SQLSMALLINT plm_handle_type,	// Handle type
	SQLHANDLE plm_handle,			// Handle name
	int ConnInd)	    			// Connection indicator
{
	RETCODE		plm_retcode = SQL_SUCCESS;
	UCHAR		plm_szSqlState[ODBCDR_MAX_BUFF_SIZE];
	UCHAR 	 	plm_szErrorMsg[ODBCDR_MAX_BUFF_SIZE];
	SDWORD		plm_pfNativeError = 0L;
	SWORD	 	plm_pcbErrorMsg = 0;
	SQLSMALLINT 	plm_cRecNmbr = 1;
	SDWORD  	plm_SS_MsgState = 0;
	SDWORD  	plm_SS_Severity = 0;
	SQLINTEGER	plm_Rownumber = 0;
	USHORT		plm_SS_Line;
	SQLSMALLINT	plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR   	plm_SS_Procname[555];
	SQLCHAR   	plm_SS_Srvname[555];
    plm_szSqlState[0] = '\0';
    plm_szSqlState[0] = '\0';

	debug_on("odbcdr_xlt_status: save_err_msg");

	context->odbcdr_last_err_msg[0] = '\0';

  	while (plm_retcode != SQL_NO_DATA_FOUND) {
		plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
					plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
					plm_szErrorMsg, ODBCDR_MAX_BUFF_SIZE - 1, &plm_pcbErrorMsg);
 
		// Note that if the application has not yet made a
		// successful connection, the SQLGetDiagField
		// information has not yet been cached by ODBC
		// Driver Manager and these calls to SQLGetDiagField
		// will fail.
		if (plm_retcode != SQL_NO_DATA_FOUND) {
			if (ConnInd) {
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
						SQL_IS_INTEGER,
						NULL);
#ifdef _WIN32
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_LINE, &plm_SS_Line,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
						sizeof(plm_SS_Procname),
						&plm_cbSS_Procname);
				plm_retcode = SQLGetDiagField(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
						sizeof(plm_SS_Srvname),
						&plm_cbSS_Srvname);
#endif
			} // if ConnInd
			// display error messages, filtering out expected "errors"
            if ((plm_pfNativeError == 5701) ||	// Change database context error code
                (plm_pfNativeError == 5703)	) {	// Change language setting error code
					break;// out of while loop, do not display error messages
			} else {
				
				// Save the message.
				strcpy( context->odbcdr_last_err_msg, (char*)plm_szErrorMsg );

				debug1("szSqlState = %s",plm_szSqlState);
				debug1("pfNativeError = %d",plm_pfNativeError);
				debug1("szErrorMsg = %s",plm_szErrorMsg);
				debug1("pcbErrorMsg = %d",plm_pcbErrorMsg);
				if (ConnInd) {
					debug1("ODBCRowNumber = %d", plm_Rownumber);
					debug1("SSrvrLine = %d", plm_Rownumber);
					debug1("SSrvrMsgState = %d",plm_SS_MsgState);
					debug1("SSrvrSeverity = %d",plm_SS_Severity);
					debug1("SSrvrProcname = %s",plm_SS_Procname);
					debug1("SSrvrSrvname = %s",plm_SS_Srvname);
				}
				break;
			} //else
		} //if plm_retcode
		plm_cRecNmbr++; //Increment to next diagnostic record.
	} // End while.

	debug_return_void(NULL);

}

static void save_err_msgW( 
	odbcdr_context_def  *context, 
	SQLSMALLINT plm_handle_type,	// Handle type
	SQLHANDLE plm_handle,			// Handle name
	int ConnInd)	    			// Connection indicator
{
	RETCODE		plm_retcode = SQL_SUCCESS;
	SQLWCHAR    plm_szSqlState[ODBCDR_MAX_BUFF_SIZE];
	SQLWCHAR 	plm_szErrorMsg[ODBCDR_MAX_BUFF_SIZE];
	SDWORD		plm_pfNativeError = 0L;
	SWORD	 	plm_pcbErrorMsg = 0;
	SQLSMALLINT plm_cRecNmbr = 1;
	SDWORD  	plm_SS_MsgState = 0;
	SDWORD  	plm_SS_Severity = 0;
	SQLINTEGER	plm_Rownumber = 0;
	USHORT		plm_SS_Line;
	SQLSMALLINT	plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLWCHAR   	plm_SS_Procname[555];
	SQLWCHAR   	plm_SS_Srvname[555];
    plm_szSqlState[0] = L'\0';
    plm_szErrorMsg[0] = L'\0';

	debug_on("odbcdr_xlt_status: save_err_msg");

	context->odbcdr_last_err_msgW[0] = L'\0';

  	while (plm_retcode != SQL_NO_DATA_FOUND) {
		plm_retcode = SQLGetDiagRecW(plm_handle_type, plm_handle,
					plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
					plm_szErrorMsg, ODBCDR_MAX_BUFF_SIZE - 1, &plm_pcbErrorMsg);
 
		// Note that if the application has not yet made a
		// successful connection, the SQLGetDiagField
		// information has not yet been cached by ODBC
		// Driver Manager and these calls to SQLGetDiagField
		// will fail.
		if (plm_retcode != SQL_NO_DATA_FOUND) {
			if (ConnInd) {
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
						SQL_IS_INTEGER,
						NULL);
#ifdef _WIN32
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_LINE, &plm_SS_Line,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
						SQL_IS_INTEGER,
						NULL);
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
						sizeof(plm_SS_Procname),
						&plm_cbSS_Procname);
				plm_retcode = SQLGetDiagFieldW(
						plm_handle_type, plm_handle, plm_cRecNmbr,
						SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
						sizeof(plm_SS_Srvname),
						&plm_cbSS_Srvname);
#endif
			} // if ConnInd
			// display error messages, filtering out expected "errors"
            if ((plm_pfNativeError == 5701) ||	// Change database context error code
                (plm_pfNativeError == 5703)	) {	// Change language setting error code
					break;// out of while loop, do not display error messages
			} else {
				
				// Save the message.
				wcscpy( context->odbcdr_last_err_msgW, (wchar_t*)plm_szErrorMsg );

				debug1("szSqlState = %ls",plm_szSqlState);
				debug1("pfNativeError = %d",plm_pfNativeError);
				debug1("szErrorMsg = %ls",plm_szErrorMsg);
				debug1("pcbErrorMsg = %d",plm_pcbErrorMsg);
				if (ConnInd) {
					debug1("ODBCRowNumber = %d", plm_Rownumber);
					debug1("SSrvrLine = %d", plm_Rownumber);
					debug1("SSrvrMsgState = %d",plm_SS_MsgState);
					debug1("SSrvrSeverity = %d",plm_SS_Severity);
					debug1("SSrvrProcname = %ls",plm_SS_Procname);
					debug1("SSrvrSrvname = %ls",plm_SS_Srvname);
				}
				break;
			} //else
		} //if plm_retcode
		plm_cRecNmbr++; //Increment to next diagnostic record.
	} // End while.

	debug_return_void(NULL);

}

///////////////////////////////////////////////////////////////////////////
