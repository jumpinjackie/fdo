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

#include <sdeerno.h>

// set to 1 to delete an existing version of the same name
#define DRACONIAN 0

ArcSDECreateLongTransactionCommand::ArcSDECreateLongTransactionCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoICreateLongTransaction> (connection),
    mName (),
    mDescription ()
{
}

ArcSDECreateLongTransactionCommand::~ArcSDECreateLongTransactionCommand (void)
{
}

/// <summary>Gets the name of the long transaction to create as a string.</summary>
/// <returns>Returns the name of the long transaction</returns> 
FdoString* ArcSDECreateLongTransactionCommand::GetName ()
{
    return (mName);
}

/// <summary>Sets the name of the long transaction to create as a string.</summary>
/// <param name="value">Input the name of the long transaction</param> 
/// <returns>Returns nothing</returns> 
void ArcSDECreateLongTransactionCommand::SetName (FdoString* value)
{
    FdoString* name;

    name = ArcSDELongTransactionUtility::CheckName (value);
    // check for special constants
    if (0 == wcscmp (FDO_ROOTLONGTRANSACTION, name))
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_ILLEGAL_VERSION_NAME, "'%1$ls' is an illegal version name.", name));
    if (0 == wcscmp (FDO_ACTIVELONGTRANSACTION, name))
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_ILLEGAL_VERSION_NAME, "'%1$ls' is an illegal version name.", name));
    if (NULL != wcschr (value, '.'))
        throw FdoException::Create (NlsMsgGet (ARCSDE_INVALID_VERSION_NAME, "A version name cannot contain the character '.'."));
    mName = name;
}

/// <summary>Gets the description of the long transaction to create as a string.</summary>
/// <returns>Returns the description of the long transaction</returns> 
FdoString* ArcSDECreateLongTransactionCommand::GetDescription ()
{
    return (mDescription);
}

/// <summary>Sets the description of the long transaction to create as a string.</summary>
/// <param name="value">Input the description of the long transaction</param> 
/// <returns>Returns nothing</returns> 
void ArcSDECreateLongTransactionCommand::SetDescription (FdoString* value)
{
    mDescription = ArcSDELongTransactionUtility::CheckDescription (value);
}

/// <summary>Executes the create long transaction command.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDECreateLongTransactionCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    CHAR* name;
    CHAR* description;
    SE_VERSIONINFO version;
    CHAR parent_name[SE_MAX_VERSION_LEN];
    LONG id;
    LONG result;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
    conn = connection->GetConnection ();

    // get the name and description
    if (0 == wcslen (GetName ()))
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));
    name = NULL;
    wide_to_multibyte (name, (wchar_t*)GetName ());
    description = NULL;
    if (NULL != GetDescription ())
        wide_to_multibyte (description, (wchar_t*)GetDescription ());

    // create the empty version object
    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

    // if the version already exists, complain bitterly
    if (SE_SUCCESS == SE_version_get_info (conn, name, version))
        if (DRACONIAN)
            ArcSDELongTransactionUtility::DeleteVersion (conn, name);
        else
        {
            SE_versioninfo_free (version);
            throw FdoException::Create (NlsMsgGet1 (ARCSDE_VERSION_EXISTS, "Version '%1$ls' already exists.", GetName ()));
        }

    // find the parent name
    if (-1 != connection->GetActiveVersion ())
        ArcSDELongTransactionUtility::GetVersionName (conn, connection->GetActiveVersion (), parent_name);
    else
        strcpy (parent_name, SE_QUALIFIED_DEFAULT_VERSION_NAME);

    // fetch the version info: we need it's state id
    result = SE_version_get_info (conn, parent_name, version);
    wchar_t *wQualifiedDefaultVersionName = NULL;
    multibyte_to_wide(wQualifiedDefaultVersionName, SE_QUALIFIED_DEFAULT_VERSION_NAME);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", wQualifiedDefaultVersionName);

    // create a child state from the state of the parent version
    result = SE_versioninfo_get_state_id (version, &id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_COULD_NOT_FETCH_STATE, "State id could not be retrieved.");
    id = ArcSDELongTransactionUtility::CreateChildState (conn, id);

    // change our copy of the parent version into the version we want to create

    // give it the new name
    result = SE_versioninfo_set_name (version, name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be set.", L"name");

    // set the new state as the state of this version
    result = SE_versioninfo_set_state_id (version, id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be set.", L"state_id");

    // optionally set the description
    if (NULL != description)
    {
        result = SE_versioninfo_set_description (version, description);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be set.", L"description");
    }

    // ToDo: public or private?
    result = SE_versioninfo_set_access (version, SE_VERSION_ACCESS_PUBLIC);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be set.", L"access");

    // set the parent name
    result = SE_versioninfo_set_parent_name (version, parent_name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be set.", L"parent");

    /* SE_version_create creates a child version from an existing version.
        The parent version must be closed and owned by the current user or public,
        or the current user is the ArcSDE DBA. You can require a unique name by
        setting unique_name to TRUE. If the name isn't unique, the ArcSDE software
        will append ‘01’ or whatever value creates a unique version name.
        If the already existing version name is My_version05, the new version
        would be My_version06. If unique_name is false and the version name already
        exists, the function will fail. */
    // ToDo: unique flag set to TRUE?
    result = SE_version_create (conn, version, FALSE, version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_CREATE_FAILED, "Cannot create version '%1$ls'.", GetName());

    SE_versioninfo_free (version);
}


