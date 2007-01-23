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

ArcSDEAcquireLockCommand::ArcSDEAcquireLockCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIAcquireLock> (connection),
    mType (FdoLockType_None),
    mStrategy (FdoLockStrategy_All)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEAcquireLockCommand::ArcSDEAcquireLockCommand (const ArcSDEAcquireLockCommand &right) { }
ArcSDEAcquireLockCommand::~ArcSDEAcquireLockCommand (void)
{
}

/// <summary>Gets the FdoLockType value.</summary>
/// <returns>Returns the desired lock type.</returns> 
FdoLockType ArcSDEAcquireLockCommand::GetLockType ()
{
    return (mType);
}

/// <summary>Sets the FdoLockType value.</summary>
/// <param name="value">Input the lock type</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEAcquireLockCommand::SetLockType (FdoLockType value)
{
    switch (value)
    {
        case FdoLockType_Exclusive:
            // Indicates an exclusive lock type.
            break;
        case FdoLockType_LongTransactionExclusive:
            // Indicates a long transacation exclusive lock type.
            throw FdoCommandException::Create (NlsMsgGet (ARCSDE_LONG_TRANSACTION_EXCLUSIVE_LOCKS_NOT_SUPPORTED, "Lock type FdoLockType_LongTransactionExclusive is not supported."));
        case FdoLockType_None: // indicates no lock should be applied to objects, nonsensical for acquire lock
        default:
            throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_INVALID_LOCK_TYPE, "Invalid lock type ('%1$d').", (int)value));
    }
    mType = value;
}

/// <summary>Gets the FdoLockStrategy value.</summary>
/// <returns>Returns the desired lock strategy.</returns> 
FdoLockStrategy ArcSDEAcquireLockCommand::GetLockStrategy ()
{
    return (mStrategy);
}

/// <summary>Sets the FdoLockStrategy value.</summary>
/// <param name="value">Input the lock strategy used for locking objects</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEAcquireLockCommand::SetLockStrategy (FdoLockStrategy value)
{
    switch (value)
    {
        case FdoLockStrategy_All:
            // Declares that either all of the locks must be acquired or none should be acquired.
            break;
        case FdoLockStrategy_Partial:
            // Declares that all of the locks that can be acquired should be acquired.
            break;
        default:
            throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_INVALID_LOCK_STRATEGY, "Invalid lock strategy ('%1$d').", (int)value));
    }
    mStrategy = value;
}

/// <summary>Executes the AcquireLock command, returning an FdoILockConflictReader.</summary>
/// <returns>Returns the list of conflicts.</returns> 
FdoILockConflictReader* ArcSDEAcquireLockCommand::Execute ()
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
    FdoPtr<ArcSDELockConflictReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    if (FdoLockType_None == GetLockType ())
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_LOCK_TYPE_NOT_SET, "The lock type must be set to a valid type."));
    else
    {
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
        ArcSDELockUtility::LockEnable (connection->GetConnection (), table, column);

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

        // The documentation says that when SE_stream_set_rowlocking is enabled,
        // the SE_stream_execute will report SE_LOCK_CONFLICT immediately if some
        // rows are already locked.
        // This is not the case.
        // So we go through a two phase approach, first determining if there are
        // conflicts and which id values these are, and secondly, if there are no
        // conflicts, lock the unlocked rows.
        // TODO: unless this is in a transaction, these sequential SE_stream_executes are not atomic
        // Do we start a transaction when beginning this command if none is in force?
        // SDE says it commits one when SE_stream_execute is used with a locking operation
        // (otherwise it wouldn't be in the LXXXX table).

        // query for other users locks
        result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_OTHER_LOCKS);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

        // apply attribute and spatial query to stream
        columns[0] = column;
        ApplyFilterInfoToStream (connection, stream, table, where, 1, columns, count, filters);

        // actually execute the query
        result = SE_stream_execute (stream);
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

        // gather up the conflicts
        ArcSDELockUtility::GatherConflicts (stream, column, 1, ret);

        // check for any conflicts
        if ((0 == ret->mIds->GetCount ()) || (FdoLockStrategy_Partial == GetLockStrategy ()))
        {
            // no conflicts, lock em down, reuse the same stream... may not need all this again
            result = SE_stream_close (stream, TRUE); 

            // if necessary, version enable the stream
            ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

            // query for all rows, should only get unlocked rows
            result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_UNLOCKED | SE_ROWLOCKING_LOCK_ON_QUERY | SE_ROWLOCKING_LOCK_ONLY);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

            // apply attribute and spatial query to stream
            ApplyFilterInfoToStream (connection, stream, table, where, 1, columns, count, filters);

            // actually execute the query
            result = SE_stream_execute (stream);
            handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
        }

        // clean up
        result = SE_stream_free (stream);
        handle_sde_err<FdoCommandException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_FREE, "Stream free failed.");
    }

    // clean up
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

