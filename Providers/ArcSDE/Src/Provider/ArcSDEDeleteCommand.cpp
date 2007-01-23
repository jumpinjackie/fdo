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
#include "ArcSDEUtils.h"

ArcSDEDeleteCommand::ArcSDEDeleteCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIDelete> (connection)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEDeleteCommand::ArcSDEDeleteCommand (const ArcSDEDeleteCommand &right) { }

ArcSDEDeleteCommand::~ArcSDEDeleteCommand (void)
{
}

void ArcSDEDeleteCommand::reset ()
{
    mConflictReader = NULL;
}

/// <summary>Executes the delete command and returns the number of instances
/// deleted</summary>
/// <returns>Returns the number of instances deleted.</returns> 
FdoInt32 ArcSDEDeleteCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    LONG result = 0L;
    SE_STREAM stream;
    CHAR* where = NULL;
    SHORT spatialFilterCount = 0;
    SE_FILTER *spatialFilters = NULL;
    SE_REGINFO registration;
    CHAR primary[SE_MAX_COLUMN_LEN];
    LONG type = 0L;
    int limit = 0;
    int number = 0;
    int total = 0;
    FdoInt32 ret = -1;
    bool versioned;
    bool lockable;
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* wcolumn;
    FdoString* property;
    const CHAR *columnNames[2];  // id column + geometry column
    FdoPtr<FdoISQLDataReader> reader;
    LONG* buffer;
    SE_LOG log;
    CHAR logfile[SE_MAX_PATH_LEN];
    SE_SQL_CONSTRUCT  sql_construct;
    CHAR* tables[1];

    // reset cached values
    reset ();

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // Get the class definition which reflects the requested feature class name
    FdoPtr<FdoClassDefinition> classDef = connection->GetRequestedClassDefinition (mClassName);

    // Get the name of the table we are updating:
    connection->ClassToTable(table, classDef);
    wchar_t *wtable = NULL;
    multibyte_to_wide(wtable, table);

    result = SE_stream_create (connection->GetConnection (), &stream);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

    // if necessary, version the table and version enable the stream
    versioned = ArcSDELongTransactionUtility::VersionStream (connection, stream, table, true);

    // Convert mFilter into spatialFilters and where clause, if any:
    GetFilterInfo(mConnection, mFilter, classDef, where, spatialFilterCount, spatialFilters);

    lockable = ArcSDELockUtility::IsLockable (mConnection->GetConnection (), table, column);
    if (lockable)
    {
        // get the property name that is the row_id
        multibyte_to_wide (wcolumn, column);
        property = connection->ColumnToProperty (classDef, wcolumn);
        tables[0] = table;
        columnNames[0] = column;
    }
    else
        property = L"";
    // establish an empty conflict reader
    mConflictReader = new ArcSDELockConflictReader (connection, classDef->GetQualifiedName (), table, property);


    // NOTE: SE_stream_delete_from_table() doesn't understand spatial queries, so we handle spatial queries as a separate case
    // NOTE: SQL Server has some problems deleting from some tables when calling
    // SE_stream_delete_by_id_list() or SE_stream_delete_from_table(), and even worse problems when calling
    // SE_stream_delete_table().
    bool bIsSQLServer = (connection->RdbmsId() == SE_DBMS_IS_SQLSERVER);
    bIsSQLServer = false;  // We turned off this workaround because it is no better than the non-workaround code.
    if ((NULL != spatialFilters) || bIsSQLServer)
    {   // need to query and delete by rowid

        // Get rowid column & rowlocking status for this table:
        result = SE_reginfo_create (&registration);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
        result = SE_registration_get_info (connection->GetConnection (), table, registration);
        handle_sde_err<FdoCommandException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
        result = SE_reginfo_get_rowid_column (registration, primary, &type);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid");
        if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE == type)
            throw FdoException::Create (NlsMsgGet1(ARCSDE_SPATIAL_QUERY_ON_NONROWID_TABLE, "Cannot perform a spatial query (for insert, update, or delete) against ArcSDE table '%1$ls' since it has no rowid column.", wtable));

        SE_reginfo_free (registration);

        // Set up select clause:
        long lColumnCount = 1;
        columnNames[0] = primary;  // id column; redundant
        if (NULL != spatialFilters)
        {
            columnNames[1] = spatialFilters[0].column;  // geometry column, required in select clause for spatial query on user-defined id tables
            lColumnCount++;
        }

        if (lockable)
        {
            // lock the table's lock table to prevent alteration
            reader = ArcSDELockUtility::LockLockTable (connection, table);
            // lock rows selected
            result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_LOCK_ON_QUERY);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");
        }
        else
            reader = NULL;

        // Initialize stream with sql & spatial queries:
        ApplyFilterInfoToStream(mConnection, stream, table, where, lColumnCount, columnNames, spatialFilterCount, spatialFilters);

        if (lockable || bIsSQLServer)
        {
            // set up a temporary log file
            ArcSDELockUtility::MakeLog (&log, connection->GetConnection (), table);
            ArcSDELockUtility::GetLogFile (logfile, connection->GetConnection (), log);

            // accumulate the query in the log file
            result = SE_stream_set_logfile (stream, log, FALSE);
            handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_LOG_SET_LOGFILE, "Could not set log file.");
        }

        // Execute query:
        result = SE_stream_execute (stream);
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

        if (lockable || bIsSQLServer)
        {
            result = SE_stream_fetch (stream);
            // two possibilities: everything was deleted (SE_FINISHED) or there was a conflict (SE_LOCK_CONFLICT)
            switch (result)
            {
                case SE_SUCCESS:
                case SE_FINISHED:
                    // return number of rows selected (to be updated)
                    ret = SE_log_count (connection->GetConnection (), log);

                    if (0 != ret)
                    {
                        // get the list of row ids
                        LONG rows = ret;
                        buffer = ArcSDELockUtility::GetRowIdList (connection->GetConnection (), logfile, rows);

                        // reuse the same stream
                        result = SE_stream_close (stream, TRUE);

                        // if necessary, version enable the stream
                        ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

                        // delete the rows
                        result = SE_stream_delete_by_id_list (stream, table, buffer, ret);
                        if (result != SE_NO_ROWS_DELETED)
                            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_UPDATE_ROW_FAILED, "Preparing stream for update row failed.");

                        delete[] buffer;
                    }

                    break;
                case SE_LOCK_CONFLICT:
                    // reuse the same stream
                    result = SE_stream_close (stream, TRUE);

                    // if necessary, version enable the stream
                    ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

                    // select other people's locks
                    result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_OTHER_LOCKS);
                    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

                    // get the list of row ids from the log file
                    tables[0] = table;
                    sql_construct.tables = tables;
                    sql_construct.num_tables = ELEMENTS (tables);
                    sql_construct.where = NULL;
                    result = SE_stream_query_logfile (stream, logfile, 1, (const CHAR**)columnNames, &sql_construct);
                    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

                    // execute the query that fetches conflicts
                    result = SE_stream_execute (stream);
                    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

                    // gather up the conflicts
                    ArcSDELockUtility::GatherConflicts (stream, column, 1, mConflictReader);

                    ret = 0;
                    break;
                default:
                    handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
            }

            // release the transaction lock
            if (reader != NULL)
                reader->Close ();

            // close the log file
            result = SE_log_close (connection->GetConnection (), log);
            handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_CLOSE, "Unable to close log file.");
        }
        else  // not lockable
        {
            CHAR whereClause[1024];  // need size in case id column is a string with large string values

            SE_STREAM deleteStream;
            result = SE_stream_create(connection->GetConnection(), &deleteStream);
            handle_sde_err<FdoCommandException>(deleteStream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");

            // if necessary, version the table and version enable the stream
            if (versioned)
                ArcSDELongTransactionUtility::VersionStream (connection, deleteStream, table);

            ret = 0;
            while ((result = SE_stream_fetch (stream)) != SE_FINISHED)
            {
                handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");

                // Get this row's ID value:
                FdoPtr<FdoDataPropertyDefinitionCollection> ids = classDef->GetIdentityProperties();
                FdoPtr<FdoDataPropertyDefinition> id = ids->GetItem(0);
                FdoPtr<FdoExpression> expr = GetValueFromStream(stream, 1, id);

                // build the where clause:
                wchar_t buffer[1024];
                FdoCommonOSUtil::swprintf(buffer, ELEMENTS(buffer), L"%ls = %ls", id->GetName(), expr->ToString());
                FdoPtr<FdoFilter> updateFilter = FdoFilter::Parse(buffer);
                FdoPtr<ArcSDEFilterToSql> f2s = new ArcSDEFilterToSql (connection, classDef);
                f2s->HandleFilter (updateFilter);
                CHAR* tempWhereClause = NULL;
                wide_to_multibyte (tempWhereClause, f2s->GetSql ());  // volatile, since memory is on stack
                if (0 == strcmp (tempWhereClause, " WHERE "))
                    whereClause[0] = '\0';
                else
                    strcpy (whereClause, tempWhereClause);

                // Update the row in the table, based on this id value:
                result = SE_stream_delete_from_table(deleteStream, table, whereClause);
                handle_sde_err<FdoCommandException>(deleteStream, result, __FILE__, __LINE__, ARCSDE_DELETE_FAILED, "Delete from table '%1$ls' failed.", wtable);

                // Increate count of updated rows:
                ret++;
            }

            // clean up:
            SE_stream_free(deleteStream);
        }
    }
    else
    {   // simple delete by criteria

        // lock the table's lock table to prevent alteration
        if (lockable)
            reader = ArcSDELockUtility::LockLockTable (mConnection, table);
        else
            reader = NULL;

        // NOTE: SE_stream_delete_table() doesn't work against ArcSDE 8.3 server,
        // so we use SE_stream_delete_from_table() instead.
        result = SE_stream_delete_from_table (stream, table, where);
        /*
        result = SE_stream_delete_table (stream, table, where);
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_DELETE_FAILED, "Delete from table '%1$ls' failed.", wtable);
        result = SE_stream_execute(stream);
        */
        if (SE_SUCCESS == result)
            ret = 1;  //NOTE: we should return a real row count, but ArcSDE doesn't supply the rowcount directly
        else if (SE_NO_ROWS_DELETED == result)
            ret = 0;
        else if ((SE_LOCK_CONFLICT == result) && lockable)
        {
            // reuse the same stream... may not need all this again
            result = SE_stream_close (stream, TRUE); 
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");

            // if necessary, version enable the stream
            if (versioned)
                ArcSDELongTransactionUtility::VersionStream (connection, stream, table, versioned);

            // select other people's locked rows
            result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_OTHER_LOCKS);
            handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

            // apply attribute and spatial query to stream
            columnNames[0] = column;
            ApplyFilterInfoToStream (mConnection, stream, table, where, 1, columnNames, 0, NULL);

            // execute the query that fetches conflicts
            result = SE_stream_execute (stream);
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

            // gather up the conflicts
            ArcSDELockUtility::GatherConflicts (stream, column, 1, mConflictReader);

            ret = 0;
        }
        else
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_DELETE_FAILED, "Delete from table '%1$ls' failed.", wtable);

        // release the transaction lock
        if (reader != NULL)
            reader->Close ();
    }

    // Clean up:
    delete[] where;
    if (NULL != spatialFilters)
    {
        for (int i = 0; i < spatialFilterCount; i++)
            if (NULL != spatialFilters[i].filter.shape)
                SE_shape_free (spatialFilters[i].filter.shape);
        delete[] spatialFilters;
    }
    result = SE_stream_close (stream, TRUE);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");

    return (ret);
}

/// <summary> Deleting objects might result in lock conflicts if objects
/// to be deleted are not exclusively locked for the user attempting to
/// delete the object. A lock conflict report is generated.
/// The function GetLockConflicts returns a lock conflict reader that provides
/// access to the list of lock conflicts that occurred during the execution
/// of the delete operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ArcSDEDeleteCommand::GetLockConflicts ()
{
    return (FDO_SAFE_ADDREF (mConflictReader.p));
}

