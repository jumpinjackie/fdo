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

typedef struct _lock
{
    LONG id;
    CHAR user[SE_MAX_OWNER_LEN];
} Lock;

int OS__cdecl compare (const void *elem1, const void *elem2)
{
    return ((*((Lock*)elem1)).id - (*((Lock*)elem2)).id);
}

ArcSDEReleaseLockCommand::ArcSDEReleaseLockCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIReleaseLock> (connection),
    mOwner ()
{
}

/** Do not implement the copy constructor. **/
//ArcSDEReleaseLockCommand::ArcSDEReleaseLockCommand (const ArcSDEReleaseLockCommand &right) { }
ArcSDEReleaseLockCommand::~ArcSDEReleaseLockCommand (void)
{
}

//
// FdoIReleaseLock overrides
//

/// <summary>Gets the name of the user who owns the lock to release. If this function
/// returns null or an empty string the lock being released is assumed to
/// belong to the connected user. Users with appropriate administrative
/// privileges can release locks owned by other users. The SetLockOwner
/// method allows an administrative user to set that name.</summary>
/// <returns>Returns name of the user</returns> 
FdoString* ArcSDEReleaseLockCommand::GetLockOwner ()
{
    return (mOwner);
}

/// <summary>Sets the name of the user who owns the lock to release. If set to null
/// or an empty string the lock being released is assumed to belong to the
/// connected user. Users with appropriate administrative privileges can
/// release locks owned by other users. The SetLockOwner method allows an
/// administrative user to set that name.</summary>
/// <param name="value">Input the name of the user.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEReleaseLockCommand::SetLockOwner (FdoString* value)
{
    CHAR* owner;

    if (NULL == value)
        value = L"";
    else
    {
        wide_to_multibyte (owner, value);
        if (strlen (owner) > SE_MAX_OWNER_LEN - 1)
            throw FdoException::Create (NlsMsgGet1 (ARCSDE_OWNER_INVALID, "Owner name '%1$ls' is too long.", value));
    }

    mOwner = value;
}

/// <summary>Executes the release lock command, returning an FdoILockConflictReader.</summary>
/// <returns>Returns an FdoILockConflictReader</returns> 
FdoILockConflictReader* ArcSDEReleaseLockCommand::Execute ()
{
    const CHAR* columns[1];
    FdoPtr<ArcSDEConnection> connection;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* wcolumn;
    CHAR *where;
    SHORT count;
    SE_FILTER* filters;
    FdoString* property;
    LONG result;
    SE_STREAM stream;
    CHAR user_name[SE_MAX_OWNER_LEN];
    wchar_t* me;
    CHAR logfile[SE_MAX_PATH_LEN];
    SE_SQL_CONSTRUCT  sql_construct;
    CHAR* tables[1];
    FdoPtr<FdoISQLDataReader> reader;
    SE_LOG log;
    LONG number = 0;
    Lock* locks = NULL;
    wchar_t* locktable = NULL;
    FdoPtr<ArcSDELockConflictReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // get the class definition which reflects the requested feature class name
    FdoPtr<FdoClassDefinition> definition = connection->GetRequestedClassDefinition (mClassName);

    // get the filter if any
    FdoPtr<FdoFilter> filter = GetFilter ();

    // get SQL query's "from" table list
    mConnection->ClassToTable (table, definition);

    // ensure lockable table
    if (!ArcSDELockUtility::IsLockable (connection->GetConnection (), table, column))
    {
        wchar_t* wtable;
        multibyte_to_wide (wtable, table);
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_LOCKING_NOT_ENABLED, "Table '%1$ls' is not lock enabled.", wtable));
    }

    // get the property name that is the row_id
    multibyte_to_wide (wcolumn, column);
    property = connection->ColumnToProperty (definition, wcolumn);

    // get SQL query's "where" clause & spatial filters
    where = NULL;
    count = 0;
    filters = NULL;
    GetFilterInfo (connection, filter, definition, where, count, filters);

    // establish an empty conflict reader
    ret = new ArcSDELockConflictReader (connection, definition->GetQualifiedName (), table, property);

    // initialize the stream query
    result = SE_stream_create (connection->GetConnection (), &stream);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

    // if necessary, version enable the stream
    ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

    // release lock, don't return rows
    result = SE_connection_get_user_name (connection->GetConnection (), user_name);
    handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
    multibyte_to_wide (me, user_name);
    if (0 == wcscmp (GetLockOwner (), L""))
    {
        result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_UNLOCK_ON_QUERY
        | SE_ROWLOCKING_LOCK_ONLY);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");
    }
    else if (0 == wcscmp (GetLockOwner (), me))
    {
        result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_UNLOCK_ON_QUERY
        | SE_ROWLOCKING_FILTER_MY_LOCKS | SE_ROWLOCKING_LOCK_ONLY);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");
    }
    else
    {
        LONG *ids = NULL;
        CHAR **users = NULL;
        CHAR lt[SE_QUALIFIED_TABLE_NAME];

        if (0)
            throw FdoCommandException::Create (NlsMsgGet (ARCSDE_RELEASE_UNOWNED_LOCKS, "Releasing other owners locks is not supported."));

        result = SE_table_get_rowlocks (connection->GetConnection(), table, &number, &ids, &users);
        handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_GET_ROW_LOCK_LIST_FAILED, "Failed to get the row lock list.");
        if (0 != number)
        {
            // put the id's and users in an array of lock structures
            locks = (Lock*)calloc (number, sizeof (Lock));
            for (int i = 0; i < number; i++)
            {
                locks[i].id = ids[i];
                strcpy (locks[i].user, users[i]);
            }
            qsort (locks, number, sizeof (LONG), compare);
            ArcSDELockUtility::LockTableName (lt, connection, table);
            multibyte_to_wide (locktable, lt);
            SE_table_free_rowlocks_list (number, ids, users);
        }
    }

    // apply attribute and spatial query to stream
    columns[0] = column;
    ApplyFilterInfoToStream (connection, stream, table, where, 1, columns, count, filters);

    // set up a temporary log file
    ArcSDELockUtility::MakeLog (&log, mConnection->GetConnection (), table);

    // accumulate the query in the log file
    result = SE_stream_set_logfile (stream, log, FALSE);
    handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_LOG_SET_LOGFILE, "Could not set log file.");

    // lock the table's lock table to prevent alteration
    reader = ArcSDELockUtility::LockLockTable (mConnection, table);

    // actually execute the query
    result = SE_stream_execute (stream);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

    result = SE_stream_fetch (stream);
    // three possibilities: locks for the specified user exist (SE_SUCCESS) and the log file isn't filled,
    // everything was unlocked (SE_FINISHED)
    // or there was a conflict (SE_LOCK_CONFLICT)
    switch (result)
    {
        case SE_SUCCESS:
            if (0 != number)
            {
                CHAR* user;
                LONG id;
                wchar_t drop[1024];
                FdoPtr<FdoISQLCommand> sql;
                Lock key;
                Lock* item;

		// Get lock owner name:
		const wchar_t *wLockOwner = GetLockOwner();
		wchar_t *wLockOwnerUpr = (wchar_t*)alloca( (1+wcslen(wLockOwner)) * sizeof(wchar_t));
		wcscpy(wLockOwnerUpr, wLockOwner);
		FdoCommonOSUtil::wcsupr(wLockOwnerUpr);  // ToDo: Oracle-specific
                wide_to_multibyte (user, wLockOwnerUpr);

                // process each row returned (ignoring the log file)
                sql = (FdoISQLCommand*)connection->CreateCommand (FdoCommandType_SQLCommand);
                do
                {
                    if (SE_SUCCESS != (result = SE_stream_get_integer (stream, 1, &id)))
                    {
                        multibyte_to_wide (wcolumn, column);
                        handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_GET, "Stream get ('%1$ls') failed for column '%2$ls'.", L"SE_stream_get_integer", wcolumn);
                    }
                    else
                    {
                        key.id = id;
                        // look it up to see if it's a conflict (i.e. not found)
                        if (NULL != (item = (Lock*)bsearch (&key, locks, number, sizeof (LONG), compare)))
                        {
                            if (0 == strcmp (user, item->user))
                            {
                                // ToDo: optimize this singleton delete somewhat
                                //ROW_ID                                    NOT NULL NUMBER(38)
                                //USER_NAME                                 NOT NULL VARCHAR2(32)
                                FdoCommonOSUtil::swprintf (drop, ELEMENTS (drop), L"delete from %ls where user_name=upper('%ls') and row_id = %ld", locktable, GetLockOwner (), id);
                                sql->SetSQLStatement (drop);
                                sql->ExecuteNonQuery ();
                            }
                            else
                                ret->AddIdentity (id);
                        }
                        else
                        {
                            // no lock, hence no conflict
                        }
                    }
                }
                while (SE_SUCCESS == (result = SE_stream_fetch (stream)));
                if (SE_FINISHED != result)
                    handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
            }
            break;
        case SE_FINISHED:
            break;
        case SE_LOCK_CONFLICT:
            // reuse the same stream
            result = SE_stream_close (stream, TRUE);

            // if necessary, version enable the stream
            ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

            // select locks still remaining
            result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_OTHER_LOCKS);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

            // get the list of row ids from the log file
            tables[0] = table;
            sql_construct.tables = tables;
            sql_construct.num_tables = ELEMENTS (tables);
            sql_construct.where = NULL;
            ArcSDELockUtility::GetLogFile (logfile, connection->GetConnection (), log);
            result = SE_stream_query_logfile (stream, logfile, 1, columns, &sql_construct);
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

            // execute the query that fetches conflicts
            result = SE_stream_execute (stream);
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

            // gather the conflicts
            ArcSDELockUtility::GatherConflicts (stream, column, 1, ret);

            // if there were conflicts (and there will be), do a partial unlock
            if (0 != ret->mIds->GetCount ())
            {
                // reuse the same stream
                result = SE_stream_close (stream, TRUE);

                // if necessary, version enable the stream
                ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

                // select locks still remaining
                result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_MY_LOCKS | SE_ROWLOCKING_UNLOCK_ON_QUERY | SE_ROWLOCKING_LOCK_ONLY);
                handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

                // get the list of row ids from the log file
                result = SE_stream_query_logfile (stream, logfile, 1, columns, &sql_construct);
                handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

                // execute the query that unlocks rows
                result = SE_stream_execute (stream);
                handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
            }
            else
                throw FdoException::Create (NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));
            break;
        default:
            handle_sde_err<FdoCommandException> (stream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
    }

    // release the transaction lock
    if (reader != NULL)
        reader->Close ();

    // clean up
    if (NULL != locks)
        free(locks);
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    delete[] where;
    if (NULL != filters)
    {
        for (int i = 0; i < count; i++)
            if (NULL != filters[i].filter.shape)
                SE_shape_free (filters[i].filter.shape);
        delete[] filters;
    }

    return (FDO_SAFE_ADDREF (ret.p));
}

