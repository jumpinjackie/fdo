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

ArcSDEGetLockOwnersCommand::ArcSDEGetLockOwnersCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIGetLockOwners> (connection)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEGetLockOwnersCommand::ArcSDEGetLockOwnersCommand (const ArcSDEGetLockOwnersCommand &right) { }
ArcSDEGetLockOwnersCommand::~ArcSDEGetLockOwnersCommand (void)
{
}

//
// FdoIGetLockOwners overrides
//

/// <summary>Executes the GetLockOwners command, returning an FdoILockOwnersReader.</summary>
/// <returns>Returns the lock info reader.</returns> 
FdoILockOwnersReader* ArcSDEGetLockOwnersCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    CHAR user_name[SE_MAX_OWNER_LEN];
    LONG result;
    SE_REGINFO *registrations;
    LONG count;
    CHAR table_name[SE_QUALIFIED_TABLE_NAME];
    LONG number;
    LONG *ids;
    CHAR **users;
    FdoPtr<ArcSDELockOwnersReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // establish an empty lock owners reader
    ret = new ArcSDELockOwnersReader ();

    // process the list of registered arcsde tables, checking for locks by user (or not)
    // Read all registered arcsde tables, adding them into their schema:
    result = SE_registration_get_info_list (connection->GetConnection (), &registrations, &count);
    handle_sde_err<FdoCommandException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
    user_name[0] = '\0'; // cache to speed up processing
    for (int i = 0; i < count; i++)
    {
        if (SE_reginfo_allow_rowlocks (registrations[i]))
        {
            result = SE_reginfo_get_table_name (registrations[i], table_name);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"table_name");
            result = SE_table_get_rowlocks (connection->GetConnection (), table_name, &number, &ids, &users);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_GET_ROW_LOCK_LIST_FAILED, "Failed to get the row lock list.");
            for (int j = 0; j < number; j++)
            {
                if (0 != strcmp (user_name, users[j]))
                {
                    wchar_t* owner;
                    strcpy (user_name, users[j]);
                    multibyte_to_wide (owner, user_name);
                    ret->AddOwner (owner);
                }
            }
            SE_table_free_rowlocks_list (number, ids, users);
        }
    }
    SE_registration_free_info_list (count, registrations);

    return (FDO_SAFE_ADDREF (ret.p));
}

