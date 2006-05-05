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

#include <malloc.h>

#include <sdeerno.h>

#include "ArcSDESQLCommand.h"
#include "ArcSDEFeatureReader.h"
#include "ArcSDEUtils.h"

ArcSDESQLCommand::ArcSDESQLCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoISQLCommand> (connection),
    mSql (NULL)
{
}

/** Do not implement the copy constructor. **/
//ArcSDESQLCommand::ArcSDESQLCommand (const ArcSDESQLCommand &right) { }

ArcSDESQLCommand::~ArcSDESQLCommand (void)
{
    if (NULL != mSql)
    {
        delete[] mSql;
        mSql = NULL;
    }
}

/// <summary>Gets the SQL statement to be executed as a string.</summary>
/// <returns>Returns the SQL statement.</returns> 
FdoString* ArcSDESQLCommand::GetSQLStatement ()
{
    return (mSql);
}

/// <summary>Sets the SQL statement to be executed as a string.</summary>
/// <param name="value">Input the SQL statement.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDESQLCommand::SetSQLStatement (FdoString* value)
{
    if (NULL != mSql)
    {
        delete[] mSql;
        mSql = NULL;
    }

    if (NULL != value)
    {
        mSql = new wchar_t[wcslen (value) + 1];
        wcscpy (mSql, value);
    }
}

/// <summary>Executes the SQL statement against the connection object and returns
/// the number of rows affected.</summary>
/// <returns>Returns the number of rows affected.</returns> 
FdoInt32 ArcSDESQLCommand::ExecuteNonQuery ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoString* sql = NULL;
    CHAR* buffer = NULL;
    LONG result = 0;
    SE_STREAM stream;
    FdoInt32 ret = 0;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // get & validate sql statement:
    sql = GetSQLStatement ();
    if (NULL == sql)
        throw FdoException::Create (NlsMsgGet(ARCSDE_SQL_COMMAND_MISSING_STATEMENT, "No SQL statement specified for execution by the SQL Command."));

    // convert the SQL statement
    wide_to_multibyte (buffer, (wchar_t*)sql);

    result = SE_stream_create (connection->GetConnection (), &stream);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

    result = SE_stream_prepare_sql (stream, buffer);
    if (SE_SUCCESS != result)
    {
        SE_stream_free (stream);  // ignore return code
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_SQL_COMMAND_PREPARE_FAILED, "The given SQL statement '%1$ls' could not be prepared by ArcSDE.", sql);
    }

    result = SE_stream_execute (stream);
    if (SE_SUCCESS != result)
    {
        SE_stream_free (stream);  // ignore return code
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_SQL_COMMAND_EXECUTE_FAILED, "The given SQL statement '%1$ls' failed to executed.", sql);
    }

    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");

    return (ret);
}

/// <summary>Executes the SQL statement against the connection object and returns
/// an FdoISQLDataReader.</summary>
/// <returns>Returns the data reader.</returns> 
FdoISQLDataReader* ArcSDESQLCommand::ExecuteReader ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoString* sql = NULL;
    CHAR* buffer = NULL;
    LONG result = 0;
    SE_STREAM stream;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // Get & validate SQL:
    sql = GetSQLStatement ();
    if (NULL == sql)
        throw FdoException::Create (NlsMsgGet(ARCSDE_SQL_COMMAND_MISSING_STATEMENT, "No SQL statement specified for execution by the SQL Command."));

    // convert the SQL statement
    wide_to_multibyte (buffer, (wchar_t*)sql);

    result = SE_stream_create (connection->GetConnection (), &stream);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

    result = SE_stream_prepare_sql (stream, buffer);
    if (SE_SUCCESS != result)
    {
        SE_stream_free (stream);  // ignore return code
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SQL_COMMAND_PREPARE_FAILED, "The given SQL statement '%1$ls' could not be prepared by ArcSDE.", sql);
    }

    result = SE_stream_execute (stream);
    if (SE_SUCCESS != result)
    {
        SE_stream_free (stream); // ignore return code
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SQL_COMMAND_EXECUTE_FAILED, "The given SQL statement '%1$ls' failed to execute.", sql);
    }

    return (new ArcSDESQLDataReader (connection, stream));
}

