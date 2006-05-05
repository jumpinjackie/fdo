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

ArcSDEGetLockInfoCommand::ArcSDEGetLockInfoCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIGetLockInfo> (connection)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEGetLockInfoCommand::ArcSDEGetLockInfoCommand (const ArcSDEGetLockInfoCommand &right) { }
ArcSDEGetLockInfoCommand::~ArcSDEGetLockInfoCommand (void)
{
}

//
// FdoIGetLockInfo overrides
//

/// <summary>Executes the GetLockInfo command, returning an FdoILockInfoReader.</summary>
/// <returns>Returns the lock info reader.</returns> 
FdoILockedObjectReader* ArcSDEGetLockInfoCommand::Execute ()
{
    const CHAR* columns[1];
    FdoPtr<ArcSDEConnection> connection;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* wcolumn;
    bool lockable;
    CHAR *where;
    SHORT count;
    SE_FILTER* filters;
    FdoString* property;
    LONG result;
    SE_STREAM stream;
    LONG id;
    FdoPtr<ArcSDELockedObjectReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // get the class definition which reflects the requested feature class name
    FdoPtr<FdoClassDefinition> definition = connection->GetRequestedClassDefinition (mClassName);

    // get the filter, if any
    FdoPtr<FdoFilter> filter = GetFilter ();

    // get SQL query's "from" table list
    mConnection->ClassToTable (table, definition);

    // ensure lockable table
    lockable = ArcSDELockUtility::IsLockable (connection->GetConnection (), table, column);

    // get the property name that is the row_id
    multibyte_to_wide (wcolumn, column);
    property = connection->ColumnToProperty (definition, wcolumn);

    // establish an empty locked object reader
    ret = new ArcSDELockedObjectReader (connection);

    if (lockable)
    {
        // get SQL query's "where" clause & spatial filters
        where = NULL;
        count = 0;
        filters = NULL;
        GetFilterInfo (connection, filter, definition, where, count, filters);

        // initialize the stream query
        result = SE_stream_create (connection->GetConnection (), &stream);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

        // if necessary, version enable the stream
        ArcSDELongTransactionUtility::VersionStream (connection, stream, table, false);

        // query mine and others locks
        result = SE_stream_set_rowlocking (stream, SE_ROWLOCKING_FILTER_MY_LOCKS | SE_ROWLOCKING_FILTER_OTHER_LOCKS);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

        // apply attribute and spatial query to stream
        columns[0] = column;
        ApplyFilterInfoToStream (connection, stream, table, where, 1, columns, count, filters);

        // actually execute the query
        result = SE_stream_execute (stream);
        handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
        while (SE_SUCCESS == result)
        {
            result = SE_stream_fetch (stream);
            if (SE_FINISHED == result)
                break;
            else
            {
                handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
                // get the row id
                result = SE_stream_get_integer (stream, 1, &id);
                handle_sde_err<FdoCommandException>(stream, result, __FILE__, __LINE__, ARCSDE_STREAM_GET, "Stream get ('%1$ls') failed for column '%2$ls'.", L"SE_stream_get_integer", wcolumn);
                ret->AddIdentity (table, id);
            }
        }

        // clean up
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
    }

    return (FDO_SAFE_ADDREF (ret.p));
}

