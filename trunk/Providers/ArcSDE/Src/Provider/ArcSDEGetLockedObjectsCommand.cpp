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

ArcSDEGetLockedObjectsCommand::ArcSDEGetLockedObjectsCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIGetLockedObjects> (connection)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEGetLockedObjectsCommand::ArcSDEGetLockedObjectsCommand (const ArcSDEGetLockedObjectsCommand &right) { }
ArcSDEGetLockedObjectsCommand::~ArcSDEGetLockedObjectsCommand (void)
{
}

//
// FdoIGetLockInfo overrides
//

///<summary>Gets the name of the user for which to get a list of locked objects.</summary>
/// <returns>Returns the name of the user</returns> 
FdoString* ArcSDEGetLockedObjectsCommand::GetLockOwner ()
{
    return (mLockOwner);
}

///<summary>Sets the name of the user for which to get a list of locked objects.</summary>
/// <param name="value">Input the name of the user</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEGetLockedObjectsCommand::SetLockOwner (FdoString* value)
{
    mLockOwner = value;
}

///<summary>Executes the GetLockedObjects command, returning an FdoILockedObjectReader.</summary>
/// <returns>Returns FdoILockedObjectReade.r</returns> 
FdoILockedObjectReader* ArcSDEGetLockedObjectsCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    CHAR user_name[SE_MAX_OWNER_LEN];
    FdoStringP user_str;
    CHAR* user;
    LONG result;
    SE_REGINFO *registrations;
    LONG count;
    CHAR table_name[SE_QUALIFIED_TABLE_NAME];
    LONG number;
    LONG *ids;
    FdoPtr<ArcSDELockedObjectReader> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // establish an empty locked object reader
    ret = new ArcSDELockedObjectReader (connection);

    // get the user name
    if (NULL == GetLockOwner () || (0 == wcscmp (L"", GetLockOwner ())))
    {
        result = SE_connection_get_user_name (connection->GetConnection (), user_name);
        handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
        user = user_name;
    }
    else
    {
        user_str = mLockOwner.Upper();
#ifdef SDE_UNICODE
        user = (CHAR*)sde_cstwc(user_str);
#else
        sde_wide_to_multibyte (user, (FdoString*)user_str);
#endif
    }

    // process the list of registered arcsde tables, checking for locks by user (or not)
    // Read all registered arcsde tables, adding user locks on the rows to the FdoILockedObjectReader
    connection->GetArcSDERegistrationList(&registrations, &count);
    for (int i = 0; i < count; i++)
    {
        if (SE_reginfo_allow_rowlocks (registrations[i]))
        {
            result = SE_reginfo_get_table_name (registrations[i], table_name);
            handle_sde_err<FdoCommandException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ITEM, "Table registration info item '%1$ls' could not be retrieved.", L"table_name");
            result = SE_table_get_rowlocks_by_user (connection->GetConnection(), table_name, user, &number, &ids);
            handle_sde_err<FdoCommandException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_GET_ROW_LOCK_LIST_FAILED, "Failed to get the row lock list.");
            for (int j = 0; j < number; j++)
                ret->AddIdentity (table_name, ids[j]);
            SE_table_free_rowlocks_list (number, ids, NULL);
        }
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


