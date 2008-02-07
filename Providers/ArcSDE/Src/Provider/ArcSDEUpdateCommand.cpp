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

static int OS__cdecl compare (const void *key, const void *element)
{
    return ((int)((*((LONG*)key)) - (*((LONG*)element))));
}


ArcSDEUpdateCommand::ArcSDEUpdateCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIUpdate> (connection)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEUpdateCommand::ArcSDEUpdateCommand (const ArcSDEUpdateCommand &right) { }

ArcSDEUpdateCommand::~ArcSDEUpdateCommand (void)
{
}

void ArcSDEUpdateCommand::reset ()
{
    mConflictReader = NULL;
}


//
// FdoIUpdate interface
//

/// <summary>Gets the FdoPropertyValueCollection that specifies the names and values of the 
/// properties to be updated.</summary>
/// <returns>Returns the list of properties and their values.</returns> 
FdoPropertyValueCollection* ArcSDEUpdateCommand::GetPropertyValues ()
{
    // reset cached values
    reset ();

    // Superclass does rest of the work:
    return ArcSDEFeatureCommand<FdoIUpdate>::GetPropertyValues();
}

/// <summary>Executes the update command and returns the number of modified 
/// instances.</summary>
/// <returns>Returns nothing</returns> 
FdoInt32 ArcSDEUpdateCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    LONG result = 0L;
    SE_STREAM stream;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    CHAR* where = NULL;
    int count = 0;
    CHAR** columns = NULL;
    int j = 0;
    CHAR* name = NULL;
    SHORT spatialFilterCount = 0;
    SE_FILTER *spatialFilters = NULL;
    SE_REGINFO registration;
    CHAR primary[SE_MAX_COLUMN_LEN];
    LONG type = 0L;
    int total = 0;
    FdoInt32 ret = -1;
    bool versioned;
    bool lockable;
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* wcolumn;
    FdoString* property;
    const CHAR *columnNames[2];   // id column + geometry column
    FdoPtr<FdoISQLDataReader> reader;
    CHAR logfile[SE_MAX_PATH_LEN];
    SE_SQL_CONSTRUCT  sql_construct;
    CHAR* tables[1];
    SE_LOG log;
    LONG* buffer;
    LONG row_id;

    // reset cached values
    reset ();

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // Validate that there is something to update:
    if ((mValues == NULL) || (mValues->GetCount()==0))
        return (0);

    // Get the class definition which reflects the requested feature class name
    FdoPtr<FdoClassDefinition> classDef = connection->GetRequestedClassDefinition (mClassName);

    // Handle read-only properties and default values:
    HandleReadOnlyAndDefaultValues(mValues);

    result = SE_stream_create (connection->GetConnection (), &stream);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

    // Get the name of the table we are updating:
    connection->ClassToTable(table, classDef);

    // Convert mFilter into spatialFilters and where clause, if any:
    GetFilterInfo(mConnection, mFilter, classDef, where, spatialFilterCount, spatialFilters);

    // if necessary, version the table and version enable the stream
    versioned = ArcSDELongTransactionUtility::VersionStream (connection, stream, table, true);

    // OK, down to work
    FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties ();
    FdoPtr<FdoPropertyValueCollection> values = GetPropertyValues ();

    // count the modified properties
    count = 0;
    for (int i = 0; i < values->GetCount (); i++)
    {
        FdoPtr<FdoPropertyValue> value = values->GetItem (i);
        FdoPtr<FdoIdentifier> propertyId = value->GetName();
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (propertyId->GetName());
        if (isSet (value))
            count++;
    }

    // build the column name array
    columns = new CHAR*[count];
    j = 0;
    for (int i = 0; i < values->GetCount (); i++)
    {
        FdoPtr<FdoPropertyValue> value = values->GetItem (i);
        FdoPtr<FdoIdentifier> propertyId = value->GetName();
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (propertyId->GetName());
        if (isSet (value))
        {
            CHAR column[SE_QUALIFIED_COLUMN_LEN];

            FdoPtr<FdoIdentifier> identifier = FdoIdentifier::Create (property->GetName ());
            connection->PropertyToColumn(column, classDef, identifier);
            columns[j] = (CHAR*)alloca (sde_strlen (sde_pcus2wc(column)) + 1);
            sde_strcpy (sde_pus2wc(columns[j]), sde_pcus2wc(column));
            j++;
        }
    }

    lockable = ArcSDELockUtility::IsLockable (mConnection->GetConnection (), table, column);
    if (lockable)
    {
        // get the property name that is the row_id
        sde_multibyte_to_wide (wcolumn, column);
        property = connection->ColumnToProperty (classDef, wcolumn);
        tables[0] = table;
        columnNames[0] = column;
    }
    else
        property = L"";
    // establish an empty conflict reader
    mConflictReader = new ArcSDELockConflictReader (connection, classDef->GetQualifiedName (), table, property);

    // NOTE: SE_stream_update_table() doesn't understand spatial queries, so we handle spatial queries as a separate case:
    if (NULL != spatialFilters)
    {   // need to query and update by rowid

        // Get rowid column & rowlocking status for this table:
        result = SE_reginfo_create (&registration);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
        result = SE_registration_get_info (connection->GetConnection (), table, registration);
        handle_sde_err<FdoCommandException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
        result = SE_reginfo_get_rowid_column (registration, primary, &type);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid");
        if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE == type)
        {
            wchar_t *wTable = NULL;
            sde_multibyte_to_wide(wTable, table);
            throw FdoException::Create (NlsMsgGet1(ARCSDE_SPATIAL_QUERY_ON_NONROWID_TABLE, "Cannot perform a spatial query (for insert, update, or delete) against ArcSDE table '%1$ls' since it has no rowid column.", wTable));
        }
        SE_reginfo_free (registration);

        // setup select list:
        columnNames[0] = primary;   // id column; redundant
        columnNames[1] = spatialFilters[0].column; // geometry column

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
        ApplyFilterInfoToStream(mConnection, stream, table, where, 2, columnNames, spatialFilterCount, spatialFilters);

        if (lockable)
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

        if (lockable)
        {
            result = SE_stream_fetch (stream);
            // two possibilities: everything was locked (SE_FINISHED)/selected (SE_SUCCESS) or there was a conflict (SE_LOCK_CONFLICT)
            switch (result)
            {
                case SE_SUCCESS:
                case SE_FINISHED:
                    // return number of rows selected (to be updated)
                    ret = SE_log_count (connection->GetConnection (), log);

                    if (0 != ret)
                    {
                        // get the list of row ids
                        row_id = ret; // use row_id as a temporary
                        buffer = ArcSDELockUtility::GetRowIdList (connection->GetConnection (), logfile, row_id);

                        // sort the buffer
                        qsort (buffer, ret, sizeof (LONG), compare);

                        // reuse the same stream
                        result = SE_stream_close (stream, TRUE);

                        // if necessary, version enable the stream
                        ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

                        if (lockable)
                        {
                            // unlock the applied locks
                            result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_UNLOCK_ON_UPDATE);
                            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");
                        }

                        result = SE_stream_update_ordered (stream, table, &row_id, buffer, ret, count, (const CHAR**)columns);
                        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_UPDATE_ROW_FAILED, "Preparing stream for update row failed.");

                        for (int k = 0; k < ret; k++)
                        {
                            // set the values
                            assignValues (connection, stream, table, properties, values);

                            // apply the values
                            row_id = buffer[k];
                            result = SE_stream_execute (stream);
                            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_UPDATE_BY_ID_FAILED, "Update table by ID failed.");
                        }
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

            SE_STREAM updateStream;
            result = SE_stream_create(connection->GetConnection(), &updateStream);
            handle_sde_err<FdoCommandException>(updateStream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");

            // if necessary, version the table and version enable the stream
            if (versioned)
                ArcSDELongTransactionUtility::VersionStream (connection, updateStream, table);

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
                sde_wide_to_multibyte (tempWhereClause, f2s->GetSql ());  // volatile, since memory is on stack
                if (0 == sde_strcmp (sde_pcus2wc(tempWhereClause), _TXT(" WHERE ")))
                    whereClause[0] = '\0';
                else
                    sde_strcpy (sde_pus2wc(whereClause), sde_pcus2wc(tempWhereClause));

                // Update the row in the table, based on this id value:
                result = SE_stream_update_table(updateStream, table, count, (const CHAR**)columns, whereClause);
                handle_sde_err<FdoCommandException>(updateStream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");
                assignValues (connection, updateStream, table, properties, values);
                result = SE_stream_execute(updateStream);
                handle_sde_err<FdoCommandException>(updateStream, result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");

                // Increate count of updated rows:
                ret++;
            }

            // clean up:
            SE_stream_free(updateStream);
        }
    }
    else
    {
        // simple criteria update
        // prep the stream
        result = SE_stream_update_table (stream, table, count, (const CHAR**)columns, where);
        // or by row
        // result = SE_stream_set_row (stream, table, /*row#*/1, count, (const CHAR**)columns);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_UPDATE_FAILED, "Update table failed.");


        // assign the values
        assignValues (connection, stream, table, properties, values);

        // lock the table's lock table to prevent alteration
        if (lockable)
            reader = ArcSDELockUtility::LockLockTable (mConnection, table);
        else
            reader = NULL;

        // execute
        result = SE_stream_execute (stream);
        if (SE_SUCCESS == result)
            ret = 1;  //NOTE: we should return a real row count, but ArcSDE doesn't supply the rowcount directly
        else if (SE_NO_ROWS_UPDATED == result)
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

            // Actually execute the query:
            result = SE_stream_execute (stream);
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

            // gather up the conflicts
            ArcSDELockUtility::GatherConflicts (stream, column, 1, mConflictReader);

            ret = 0;
        }
        else
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

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
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    delete[] columns;

    return (ret);
}


/// <summary> Updating objects might result in lock conflicts if objects
/// to be updated are not exclusively locked for the user attempting to
/// update the object. If objects to be updated are not exclusively locked for the 
/// user attempting to update the object, a lock conflict report is generated.
/// The function GetLockConflicts returns a lock conflict reader providing
/// access to the list of lock conflicts that occurred during the execution
/// of the update operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ArcSDEUpdateCommand::GetLockConflicts ()
{
    return (FDO_SAFE_ADDREF (mConflictReader.p));
}


