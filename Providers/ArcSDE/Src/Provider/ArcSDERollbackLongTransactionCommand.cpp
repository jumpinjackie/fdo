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

ArcSDERollbackLongTransactionCommand::ArcSDERollbackLongTransactionCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoIRollbackLongTransaction> (connection),
    mName ()
{
    FdoPtr<ArcSDEConnection> conn;

    conn = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
}

ArcSDERollbackLongTransactionCommand::~ArcSDERollbackLongTransactionCommand (void)
{
}

/// <summary>Gets the name of the long transaction to commit as a string.</summary>
/// <returns>Returns the name of the long transaction</returns> 
FdoString* ArcSDERollbackLongTransactionCommand::GetName ()
{
    return (mName);
}

/// <summary>Sets the name of the long transaction to create as a string.</summary>
/// <param name="value">Input the name of the long transaction</param> 
/// <returns>Returns nothing</returns> 
void ArcSDERollbackLongTransactionCommand::SetName (FdoString* value)
{
    mName = ArcSDELongTransactionUtility::CheckName (value);
}

void ArcSDERollbackLongTransactionCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    LONG result;
    SE_VERSIONINFO version;
    SE_VERSIONINFO parent;
    wchar_t buffer[50];
    LONG id;
    CHAR owner[2*SE_MAX_VERSION_LEN];
    CHAR user_name[SE_MAX_OWNER_LEN];
    LONG state;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
    conn = connection->GetConnection ();

    // finish edit session
    connection->Flush ();

    // create the empty version object
    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

    // get the version
    ArcSDELongTransactionUtility::GetVersionByName (conn, GetName (), version);

    // get the id
    result = SE_versioninfo_get_id (version, &id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");

    // if the active version is the one being rolled back, reset the active version
    if (id == connection->GetActiveVersion ())
        connection->SetActiveVersion (SDE_DEFAULT);

    // delete the long transaction if we own it, otherwise move it to the parent version's state
    result = SE_versioninfo_get_name (version, owner);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Name");
    *(sde_strchr (sde_pus2wc(owner), '.')) = '\0';
    result = SE_connection_get_user_name (conn, user_name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
    if (0 == sde_strcmp (sde_pcus2wc(owner), sde_pcus2wc(user_name)))
        ArcSDELongTransactionUtility::VersionDelete (conn, GetName ());
    else
    {
        result = SE_versioninfo_get_parent_id (version, &id);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"ParentId");
        result = SE_versioninfo_create (&parent);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");
        result = SE_version_get_info_by_id (conn, id, parent);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (id, buffer, ELEMENTS(buffer)));
        result = SE_versioninfo_get_state_id (parent, &state);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");
        result = SE_version_change_state (conn, version, state);
        SE_versioninfo_free (parent);
    }

    SE_versioninfo_free (version);
}

/// <summary>Gets a reference to the lock conflict reader.</summary>
/// <returns>Returns a reference to the lock conflict reader</returns> 
FdoILockConflictReader* ArcSDERollbackLongTransactionCommand::GetLockConflictReader()
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}


