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

ArcSDECommitLongTransactionCommand::ArcSDECommitLongTransactionCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoICommitLongTransaction> (connection),
    mName ()
{
    FdoPtr<ArcSDEConnection> conn;

    conn = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));

    mDispositions = new ArcSDELongTransactionConflictDirectiveEnumerator (conn);
}

ArcSDECommitLongTransactionCommand::~ArcSDECommitLongTransactionCommand (void)
{
    // ToDo: invalidate the conflict enumerator
}

/// <summary>Gets the name of the long transaction to commit as a string.</summary>
/// <returns>Returns the name of the long transaction</returns> 
FdoString* ArcSDECommitLongTransactionCommand::GetName ()
{
    return (mName);
}

/// <summary>Sets the name of the long transaction to create as a string.</summary>
/// <param name="value">Input the name of the long transaction</param> 
/// <returns>Returns nothing</returns> 
void ArcSDECommitLongTransactionCommand::SetName (FdoString* value)
{
    mName = ArcSDELongTransactionUtility::CheckName (value);
}

void ArcSDECommitLongTransactionCommand::checkConflict (
    SE_CONNECTION connection,
    CHAR* table,
    CHAR* rowid,
    LONG source,
    LONG diff,
    LONG type,
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list)
{
    SE_STREAM stream;
    CHAR* tables[1];
    const CHAR* columns[1];
    SE_SQL_CONSTRUCT  sql_construct;
    LONG result;
    LONG row;
    SHORT row_ind;

    tables[0] = table;
    columns[0] = rowid;
    sql_construct.tables = tables;
    sql_construct.num_tables = ELEMENTS (tables);
    sql_construct.where = NULL;
    result = SE_stream_create (connection, &stream);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
    result = SE_stream_set_state (stream, source, diff, type);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
    result = SE_stream_query (stream, ELEMENTS (columns), columns, &sql_construct);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_QUERY, "Stream query failed.");
    result = SE_stream_bind_output_column (stream, 1, &row, &row_ind);
    wchar_t *wRowId = NULL;
    sde_multibyte_to_wide(wRowId, rowid);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_BIND_COLUMN_FAILED, "Failed to bind column '%1$ls'.", wRowId);
    result = SE_stream_execute (stream);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
    while (SE_FINISHED != (result = SE_stream_fetch (stream)))
    {
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
        list->AddConflict (new ArcSDELongTransactionConflictDirectiveEnumerator::Conflict (row));
    }
    result = SE_stream_close (stream, true);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
}

void ArcSDECommitLongTransactionCommand::checkTable (
    SE_REGINFO table,
    LONG parent,
    LONG child)
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    CHAR name[SE_QUALIFIED_TABLE_NAME];
    LONG result;
    CHAR rowid[SE_MAX_COLUMN_LEN];
    LONG type;
    wchar_t* wname;
    wchar_t* wrowid;
    FdoPtr <FdoClassDefinition> definition;
    FdoString* property;
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list;

    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    conn = connection->GetConnection ();
    result = SE_reginfo_get_table_name (table, name);
    result = SE_reginfo_get_rowid_column (table, rowid, &type);
    sde_multibyte_to_wide (wname, name);
    sde_multibyte_to_wide (wrowid, rowid);
    definition = connection->TableToClass (wname);
    property = connection->ColumnToProperty (definition, wrowid);
    list = new ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList (definition->GetQualifiedName (), property);

    // SE_STATE_DIFF_NOCHANGE_UPDATE
    // Return all the features that haven't changed in the parent (source) state,
    // but have been updated in the child (differences) state.
    // OK, these don't conflict.

    // SE_STATE_DIFF_NOCHANGE_DELETE
    // Return all the features that haven't changed in the parent (source) state,
    // but have been deleted in the child (differences) state. 
    // OK, these don't conflict.

    // SE_STATE_DIFF_UPDATE_NOCHANGE
    // Return all the features that have been updated in the parent (source) state,
    // but unchanged in the differences state. 
    // OK, these don't conflict.

    // SE_STATE_DIFF_UPDATE_UPDATE
    // Return all the features that have been updated in
    // both the parent (source) and (child ) differences state.
    checkConflict (conn, name, rowid, parent, child, SE_STATE_DIFF_UPDATE_UPDATE, list);

    // SE_STATE_DIFF_UPDATE_DELETE
    // Return all the features that have been updated in the parent (source) state
    // but deleted in the child (differences) state. 
    checkConflict (conn, name, rowid, parent, child, SE_STATE_DIFF_UPDATE_DELETE, list);

    // SE_STATE_DIFF_INSERT
    // Return all the features that were inserted into the parent (source) state
    // and that never existed in the differences state.
    // OK, these don't conflict.

    // SE_STATE_DIFF_UPDATE_DELETE
    // Return all the features that have been updated in the child (source) state
    // but deleted in the parent (differences) state. 
    checkConflict (conn, name, rowid, child, parent, SE_STATE_DIFF_UPDATE_DELETE, list);

    if (0 != list->GetCount ())
      mDispositions->AddConflictList (list);
    else
        delete list;
}

// copy all rows from the diff (compared to source) into the target
// if list is not null, copy only those rows with resolution FdoLongTransactionConflictResolution_Child
void ArcSDECommitLongTransactionCommand::copyRows (
    SE_CONNECTION connection,
    CHAR* table,
    CHAR* rowid,
    LONG target,
    LONG from,
    LONG source,
    LONG diff,
    LONG type,
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list)
{
    SE_STREAM stream;
    CHAR* tables[1];
    const CHAR* columns[1];
    SE_SQL_CONSTRUCT  sql_construct;
    LONG result;
    LONG row;
    SHORT row_ind;
    SE_STREAM copy;
    LONG ids[100]; // ToDo: User-defined identity columns
    int index;
    int limit;
    ArcSDELongTransactionConflictDirectiveEnumerator::Conflict** conflict;

    tables[0] = table;
    columns[0] = rowid;
    sql_construct.tables = tables;
    sql_construct.num_tables = ELEMENTS (tables);
    sql_construct.where = NULL;
    result = SE_stream_create (connection, &stream);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
    result = SE_stream_set_state (stream, source, diff, type);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
    result = SE_stream_query (stream, ELEMENTS (columns), columns, &sql_construct);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_QUERY, "Stream query failed.");
    result = SE_stream_bind_output_column (stream, 1, &row, &row_ind);
    wchar_t* wRowId = NULL;
    sde_multibyte_to_wide(wRowId, rowid);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_BIND_COLUMN_FAILED, "Failed to bind column '%1$ls'.", wRowId);
    result = SE_stream_execute (stream);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
    result = SE_stream_create (connection, &copy);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
    result = SE_stream_set_state (copy, target, from, SE_STATE_DIFF_INSERT);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
    index = 0;
    limit = ELEMENTS(ids);
    while (SE_FINISHED != (result = SE_stream_fetch (stream)))
    {
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
        if (NULL != list)
        {
            conflict = list->Find (row);
            if ((NULL == conflict) || (FdoLongTransactionConflictResolution_Child == (*conflict)->mResolution))
                ids[index++] = row;
        }
        else
            ids[index++] = row;
        if (index >= limit)
        {
            result = SE_stream_copy_state_rows (copy, table, ids, limit);
            index = 0;
        }
    }
    if (0 != index)
        result = SE_stream_copy_state_rows (copy, table, ids, index);
    result = SE_stream_close (copy, true);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");
    result = SE_stream_free (copy);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    result = SE_stream_close (stream, true);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
}

void ArcSDECommitLongTransactionCommand::deleteRows (
    SE_CONNECTION connection,
    CHAR* table,
    CHAR* rowid,
    LONG target,
    LONG source,
    LONG diff,
    LONG type,
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list)
{
    SE_STREAM stream;
    CHAR* tables[1];
    const CHAR* columns[1];
    SE_SQL_CONSTRUCT  sql_construct;
    LONG result;
    LONG row;
    SHORT row_ind;
    SE_STREAM del;
    LONG ids[100]; // ToDo: User-defined identity columns
    int index;
    int limit;
    ArcSDELongTransactionConflictDirectiveEnumerator::Conflict** conflict;

    tables[0] = table;
    columns[0] = rowid;
    sql_construct.tables = tables;
    sql_construct.num_tables = ELEMENTS (tables);
    sql_construct.where = NULL;
    result = SE_stream_create (connection, &stream);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
    result = SE_stream_set_state (stream, source, diff, type);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
    result = SE_stream_query (stream, ELEMENTS (columns), columns, &sql_construct);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_QUERY, "Stream query failed.");
    result = SE_stream_bind_output_column (stream, 1, &row, &row_ind);
    wchar_t* wRowId = NULL;
    sde_multibyte_to_wide(wRowId, rowid);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_BIND_COLUMN_FAILED, "Failed to bind column '%1$ls'.", wRowId);
    result = SE_stream_execute (stream);
    handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
    result = SE_stream_create (connection, &del);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
    result = SE_stream_set_state (del, target, SE_NULL_STATE_ID, SE_STATE_DIFF_NOCHECK);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
    index = 0;
    limit = ELEMENTS(ids);
    while (SE_FINISHED != (result = SE_stream_fetch (stream)))
    {
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");

        if (NULL != list)
        {
            conflict = list->Find (row);
            if ((NULL == conflict) || (FdoLongTransactionConflictResolution_Child == (*conflict)->mResolution))
                ids[index++] = row;
        }
        else
            ids[index++] = row;
        if (index >= limit)
        {
            result = SE_stream_delete_by_id_list (del, table, ids, limit);
            index = 0;
        }
    }
    if (0 != index)
        result = SE_stream_delete_by_id_list (del, table, ids, index);
    result = SE_stream_close (del, true);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");
    result = SE_stream_free (del);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    result = SE_stream_close (stream, true);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_CLOSE, "Stream close failed.");
    result = SE_stream_free (stream);
    handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
}

void ArcSDECommitLongTransactionCommand::commitTable (
    SE_REGINFO table,
    LONG target,
    LONG parent,
    LONG child)
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    CHAR name[SE_QUALIFIED_TABLE_NAME];
    LONG result;
    CHAR rowid[SE_MAX_COLUMN_LEN];
    LONG type;
    wchar_t* wname;
    FdoPtr <FdoClassDefinition> definition;
    ArcSDELongTransactionConflictDirectiveEnumerator::ConflictList* list;

    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    conn = connection->GetConnection ();
    result = SE_reginfo_get_table_name (table, name);
    handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"table name");
    result = SE_reginfo_get_rowid_column (table, rowid, &type);
    handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"rowid column");
    sde_multibyte_to_wide (wname, name);
    definition = connection->TableToClass (wname);
    list = mDispositions->GetConflictList (definition->GetQualifiedName ());
    if (NULL != list)
        list->Sort ();

    // SE_STATE_DIFF_NOCHANGE_UPDATE (parent, child)
    // Return all the features that haven't changed in the parent (source) state,
    // but have been updated in the child (diff) state.
    // Copy all to target from child.
    copyRows (conn, name, rowid, target, child, parent, child, SE_STATE_DIFF_NOCHANGE_UPDATE, list);

    // SE_STATE_DIFF_NOCHANGE_DELETE (parent, child)
    // Return all the features that haven't changed in the parent (source) state,
    // but have been deleted in the child (diff) state.
    // Delete all from parent.
    deleteRows (conn, name, rowid, target, parent, child, SE_STATE_DIFF_NOCHANGE_DELETE, list);

    // SE_STATE_DIFF_UPDATE_NOCHANGE (parent, child)
    // Return all the features that have been updated in the parent (source) state,
    // but unchanged in the child (diff) state.
    // Do nothing.

    // SE_STATE_DIFF_UPDATE_UPDATE (parent, child)
    // Return all the features that have been updated in
    // both the parent (source) and child (diff) state.
    // Copy to target from child only those whose resolution says child wins.
    copyRows (conn, name, rowid, target, child, parent, child, SE_STATE_DIFF_UPDATE_UPDATE, list);

    // SE_STATE_DIFF_UPDATE_DELETE (parent, child)
    // Return all the features that have been updated in the parent (source) state
    // but deleted in the child (diff) state. 
    // Delete from target only those whose resolution says child wins.
    deleteRows (conn, name, rowid, target, parent, child, SE_STATE_DIFF_UPDATE_DELETE, list);

    // SE_STATE_DIFF_INSERT (parent, child)
    // Return all the features that were inserted into the parent (source) state
    // and that never existed in the child (diff) state.
    // Do nothing.

    // SE_STATE_DIFF_INSERT (child, parent)
    // Return all the features that were inserted into the child (source) state
    // and that never existed in the parent (diff) state.
    // Copy all to target from child.
    copyRows (conn, name, rowid, target, child, child, parent, SE_STATE_DIFF_INSERT, list);

    // SE_STATE_DIFF_UPDATE_DELETE (child, parent)
    // Return all the features that have been updated in the child (source) state
    // but deleted in the parent (diff) state. 
    // Copy to target from child only those whose resolution says child wins.
    copyRows (conn, name, rowid, target, child, child, parent, SE_STATE_DIFF_UPDATE_DELETE, list);

}

// Executes the commit long transaction command and returns an
// FdoILongTransactionConflictDirectiveEnumerator.
// If the returned enumerator's count is 0 then
// no conflicts were detected and the command was executed successfully.
// Otherwise conflicts were detected and the user must specify how to
// resolve them using the enumerator and call Execute again.
FdoILongTransactionConflictDirectiveEnumerator* ArcSDECommitLongTransactionCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    wchar_t buffer[50];
    LONG result;
    SE_VERSIONINFO version;
    CHAR where[50];
    LONG child;
    SE_VERSIONINFO ancestor;
    LONG parent;
    SE_REGINFO* tables;
    LONG count;
    LONG id;
    LONG target;
    SE_VERSIONINFO* info;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
    conn = connection->GetConnection ();

    // finish edit session
    connection->Flush ();

    // create the empty version objects
    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");
    result = SE_versioninfo_create (&ancestor);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

    // get the version
    ArcSDELongTransactionUtility::GetVersionByName (conn, GetName (), version);

    // get the version id
    result = SE_versioninfo_get_id (version, &id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"id");

    // check for dependant versions:
    sde_sprintf (sde_pus2wc(where), 50, _TXT("PARENT_VERSION_ID = %d"), id);
    result = SE_version_get_info_list (conn, where, &info, &count);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_LIST, "Version info list could not be retrieved.");
    SE_version_free_info_list (count,info);
    if (0 != count)
    {
        // free the version objects
        SE_versioninfo_free (ancestor);
        SE_versioninfo_free (version);
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_VERSION_HAS_CHILDREN, "Cannot commit a version that has child versions."));
    }

    // get the child state id
    result = SE_versioninfo_get_state_id (version, &child);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");

    // get the parent state id
    result = SE_versioninfo_get_parent_id (version, &parent); // temporarily use parent as version id
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"ParentId");
    result = SE_version_get_info_by_id (conn, parent, ancestor);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (parent, buffer, ELEMENTS(buffer)));
    result = SE_versioninfo_get_state_id (ancestor, &parent);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");

    // if the parent and child states are the same, we just need to delete the version
    // without manipulating or merging any state information:
    if (parent == child)
    {
        // delete the long transaction if we own it:
        if (ArcSDELongTransactionUtility::IsOurVersion (conn, version))
            ArcSDELongTransactionUtility::VersionDelete (conn, GetName ());

        mDispositions->empty ();
    }
    else
    {
        // determine which tables need to be committed
        result = SE_registration_get_difference_list (conn, child, parent, &tables, &count);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_GET_TABLE_DIFFERENCE_LIST, "Failed to retrieve ArcSDE table difference list between two states.");

        // unmark the conflict list
        for (int i = 0; i < mDispositions->mConflicts->GetCount (); i++)
            (*mDispositions->mConflicts)[i]->mMarked = false;

        // run through the modified tables
        for (int i = 0; i < count; i++)
            checkTable (tables[i], child, parent);

        // remove unmarked lists (they're old)
        for (int i = 0; i < mDispositions->mConflicts->GetCount (); i++)
            if (!(*mDispositions->mConflicts)[i]->mMarked)
            {
                for (int j = i + 1; j < mDispositions->mConflicts->GetCount (); j++)
                    (*mDispositions->mConflicts)[j - 1] = (*mDispositions->mConflicts)[j];
                ArcSDELongTransactionConflictDirectiveEnumerator::ConflictCollection::SetSize (mDispositions->mConflicts, mDispositions->mConflicts->GetCount () - 1);
                i--; // Decrement the loop variable to redo this slot
            }

        // check for conflicts and if found don't do anything
        if (!mDispositions->HasConflicts ())
        {
            // OK, no conflicts, let's do it

            // make a new state to handle the merge
            target = ArcSDELongTransactionUtility::CreateChildState (conn, parent);

            // open it, only open states can be written to
            result = SE_state_open (conn, target);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_OPEN, "Cannot open state '%1$d'.", target);

            // merge each table into the new state
            for (int i = 0; i < count; i++)
                commitTable (tables[i], target, parent, child);

            // move the parent version to the new state
            result = SE_version_change_state (conn, ancestor, target);
            handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_VERSION_STATE_CHANGE_FAILURE, "Could not set state '%1$d' for version '%2$ls'.", target, GetName ());

            // refetch the version info for the parent version
            result = SE_version_get_info_by_id (conn, id, ancestor);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (parent, buffer, ELEMENTS(buffer)));

            // close off the target state
            result = SE_state_close (conn, target);
            handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_STATE_CLOSE, "Cannot close state '%1$d'.", child);

            // if the active version is the one being commited, reset the active version
            result = SE_versioninfo_get_id (version, &id);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"id");
            if (connection->GetActiveVersion () == id)
                connection->SetActiveVersion (SDE_DEFAULT);

            // delete the long transaction if we own it, otherwise move it to the new state
            if (ArcSDELongTransactionUtility::IsOurVersion (conn, version))
                ArcSDELongTransactionUtility::VersionDelete (conn, GetName ());
            else
            {
                result = SE_version_change_state (conn, version, target);
                handle_sde_err<FdoCommandException>(conn, result, __FILE__, __LINE__, ARCSDE_VERSION_STATE_CHANGE_FAILURE, "Could not set state '%1$d' for version '%2$ls'.", target, GetName ());
                // refetch the version info
                result = SE_version_get_info_by_id (conn, id, version);
                handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (id, buffer, ELEMENTS(buffer)));
            }

            // return an empty reader to the user
            mDispositions->empty ();
        }

        SE_registration_free_info_list (count, tables);
    }

    // free the version objects
    SE_versioninfo_free (ancestor);
    SE_versioninfo_free (version);

    return (FDO_SAFE_ADDREF (mDispositions.p));
}

/// <summary>Gets a reference to the lock conflict reader.</summary>
/// <returns>Returns a reference to the lock conflict reader</returns> 
FdoILockConflictReader* ArcSDECommitLongTransactionCommand::GetLockConflictReader()
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}


