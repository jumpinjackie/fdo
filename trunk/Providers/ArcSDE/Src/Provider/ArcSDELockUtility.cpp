/*
 * 
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
* 
 */

#include "stdafx.h"

/// <summary>Check if rows in a table can be locked.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="table">Input the table name to check for lockability.</param>
/// <param name="row_id_column">Output the row id column name. Should be CHAR[SE_MAX_COLUMN_LEN].</param>
bool ArcSDELockUtility::IsLockable (SE_CONNECTION connection, const CHAR* table, CHAR* row_id_column)
{
    LONG result;
    SE_REGINFO  registration;
    bool ret;
        
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (connection, table, registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
    ret = (0 != SE_reginfo_allow_rowlocks (registration));
    if (NULL != row_id_column)
    {
        LONG type;
        CHAR column[SE_MAX_COLUMN_LEN];

        // get the table's rowid column name and rowid column type
        result = SE_reginfo_get_rowid_column (registration, column, &type);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid column");
        sde_strcpy (sde_pus2wc(row_id_column), sde_pcus2wc(column));
    }
    else
        row_id_column[0] = '\0';
    SE_reginfo_free (registration);

    return (ret);
}

/// <summary>Make sure the given table is lock enabled.</summary>
/// Note: This will only work on tables which have an SDE maintained row_id column already,
/// i.e. it won't add one.... yet
/// <param name="connection">The connection to use.</param>
/// <param name="table">Input the table name to lock enable.</param>
/// <param name="row_id_column">Output the row id column name. Should be CHAR[SE_MAX_COLUMN_LEN].</param>
void ArcSDELockUtility::LockEnable (SE_CONNECTION connection, const CHAR* table, CHAR* row_id_column)
{
    LONG result;
    SE_REGINFO  registration;
    LONG type;
    CHAR column[SE_MAX_COLUMN_LEN];
        
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (connection, table, registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");

    if (!SE_reginfo_allow_rowlocks (registration))
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_TABLE_DOES_NOT_SUPPORT_ROW_LOCKS, "Table is not row-lockable."));

    // get the table's rowid column name and rowid column type
    result = SE_reginfo_get_rowid_column (registration, column, &type);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid column");

    /* //NOTE: the following is obsolete code that forces a table to support rowlocking if it currently doesn't.
    if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_SDE == type)
    {
        if (!SE_reginfo_allow_rowlocks (registration))
        {
            result = SE_reginfo_set_rowlocks (registration, TRUE);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"rowlocks");
            result = SE_registration_alter (connection, registration);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ALTER, "Registration info could not be altered.");
        }
    }
    else if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_USER == type)
    {
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_TABLE_DOES_NOT_SUPPORT_ROW_LOCKS, "Table is not row-lockable."));
    }
    else if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE == type)
    {
        // ToDo: Add SDE maintained row id column and make it lockable
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_TABLE_DOES_NOT_SUPPORT_ROW_LOCKS, "Table is not row-lockable."));
    }

    */


    // Grab the row id column name, for output to caller:
    if (NULL != row_id_column)
        sde_strcpy (sde_pus2wc(row_id_column), sde_pcus2wc(column));

    SE_reginfo_free (registration);
}

void ArcSDELockUtility::GatherConflicts (SE_STREAM stream, const CHAR* column, int column_number, ArcSDELockConflictReader* reader)
{
    LONG result;
    LONG id;
    wchar_t* wcolumn;

    while (SE_SUCCESS == (result = SE_stream_fetch (stream)))
    {
        if (SE_SUCCESS != (result = SE_stream_get_integer (stream, column_number, &id)))
        {
            sde_multibyte_to_wide (wcolumn, column);
            handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_GET, "Stream get ('%1$ls') failed for column '%2$ls'.", L"SE_stream_get_integer", wcolumn);
        }
        else
            reader->AddIdentity (id);
    }
    if (SE_FINISHED != result)
        handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
}

void ArcSDELockUtility::LockTableName (CHAR* locktable, ArcSDEConnection* connection, const CHAR* table)
{
    SE_CONNECTION conn;
    LONG result;
    LONG id;
    SE_REGINFO registration;
    CHAR lname[SE_MAX_SCHEMA_TABLE_LEN+1];
    CHAR db[SE_QUALIFIED_TABLE_NAME+1]; // no constant for database character array size?
    CHAR owner[SE_MAX_OWNER_LEN+1];
    CHAR name[SE_MAX_SCHEMA_TABLE_LEN+1];
    CHAR col[SE_MAX_COLUMN_LEN+1];

    conn = connection->GetConnection ();
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (conn, table, registration);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
    result = SE_reginfo_get_id (registration, &id);
    handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"id");
    SE_reginfo_free (registration);
    sde_sprintf (sde_pus2wc(lname), SE_MAX_SCHEMA_TABLE_LEN+1, _TXT("L%d"), id);
    db[0] = '\0';
    owner[0] = '\0';
    name[0] = '\0';
    col[0] = '\0';
    result = SE_table_parse_qualified_name (conn, table, db, owner, name, col, FALSE);
    wchar_t* wTable = NULL;
    sde_multibyte_to_wide(wTable, table);
    handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_PARSE_TABLE_NAME_FAILED, "Failed to parse the qualified name '%1$ls'.", wTable);
    result = SE_table_make_qualified_name (conn, db, owner, lname, NULL, locktable);
    handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_MAKE_TABLE_NAME_FAILED, "Failed to make qualified name.");
}

FdoISQLDataReader* ArcSDELockUtility::LockLockTable (ArcSDEConnection* connection, const CHAR* table)
{
    FdoPtr<FdoISQLCommand> sql;
    CHAR locktable[SE_QUALIFIED_TABLE_NAME];
    wchar_t* lt;
    wchar_t select[1024];
    FdoPtr<FdoISQLDataReader> ret;

    // WORKAROUND: Attempt to lock the lock table; if it fails, return NULL instead of throwing an exception;
    // This will at least allow users to perform non-locking operations on lock-enabled tables,
    // possibly with the risk of concurrency issues.  This is to workaround a SQL Server issue
    // where the lock tables seem to not be getting created.
    try
    {
        LockTableName (locktable, connection, table);
        sde_multibyte_to_wide (lt, locktable);
        FdoCommonOSUtil::swprintf (select, ELEMENTS (select), L"select * from %ls for update", lt);
        sql = (FdoISQLCommand*)connection->CreateCommand (FdoCommandType_SQLCommand);
        sql->SetSQLStatement (select);
        ret = sql->ExecuteReader ();
        ret->ReadNext ();
    }
    catch (FdoException *e)
    {
        e->Release();
        ret = NULL;
    }

    return (FDO_SAFE_ADDREF (ret.p));
}

void ArcSDELockUtility::MakeLog (SE_LOG* log, SE_CONNECTION connection, const CHAR* table)
{
    LONG result;
    CHAR logfile[SE_MAX_PATH_LEN];
    SE_LOGINFO loginfo;

    result = SE_log_make_name (connection, logfile);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_NAME, "Unable to generate log file name.");
    result = SE_loginfo_create (&loginfo);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_CREATE, "Unable to create LOGINFO object.");
    result = SE_loginfo_set_name (loginfo, logfile);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_SET_ITEM, "Log info item '%1$ls' could not be set.", L"Name");
    result = SE_loginfo_set_persistence (loginfo, FALSE);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_SET_ITEM, "Log info item '%1$ls' could not be set.", L"Persistence");
    result = SE_loginfo_set_target_object (loginfo, SE_LOG_FOR_TABLE, (CHAR*)table, NULL); // cast away constness
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_SET_ITEM, "Log info item '%1$ls' could not be set.", L"Target");
    result = SE_log_open_log (connection, loginfo, SE_OUTPUT_MODE, log);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_OPEN, "Unable to open log file.");
    SE_loginfo_free (loginfo);
}

void ArcSDELockUtility::GetLogFile (CHAR* logfile, SE_CONNECTION connection, SE_LOG log)
{
    LONG result;
    SE_LOGINFO loginfo;

    result = SE_loginfo_create (&loginfo);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_CREATE, "Unable to create LOGINFO object.");
    result = SE_log_get_open_log_info (connection, log, loginfo);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_GET, "Could not get log info.");
    result = SE_loginfo_get_name (loginfo, logfile);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_GET_ITEM, "Log info item '%1$ls' could not be retrieved.", L"Name");
    SE_loginfo_free (loginfo);
}

LONG* ArcSDELockUtility::GetRowIdList (SE_CONNECTION connection, CHAR* logfile, LONG &count)
{
    LONG result;
    SE_LOGINFO loginfo;
    SE_LOG log;
    LONG* ret;

    result = SE_loginfo_create (&loginfo);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_CREATE, "Unable to create LOGINFO object.");
    result = SE_loginfo_set_name (loginfo, logfile);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_INFO_SET_ITEM, "Log info item '%1$ls' could not be set.", L"Name");
    result = SE_log_open_log (connection, loginfo, SE_INPUT_MODE, &log);
    handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_OPEN, "Unable to open log file.");
    SE_loginfo_free (loginfo);

    ret = new LONG[count];
    try
    {
        result = SE_log_get_id_list (connection, log, count, &count, ret);
        handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_GET_IDS, "Could not get the list of ids from the log file.");
        result = SE_log_close (connection, log);
        handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_CLOSE, "Unable to close log file.");
    }
    catch (...)
    {
        delete[] ret;
        throw;
    }

    return (ret);
}


