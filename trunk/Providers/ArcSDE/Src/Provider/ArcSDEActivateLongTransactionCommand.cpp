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

ArcSDEActivateLongTransactionCommand::ArcSDEActivateLongTransactionCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIActivateLongTransaction> (connection),
    mName ()
{
}

ArcSDEActivateLongTransactionCommand::~ArcSDEActivateLongTransactionCommand (void)
{
}

/// <summary>Gets the name of the long transaction to activate as a string.</summary>
/// <returns>Returns the name of the long transaction</returns>
FdoString* ArcSDEActivateLongTransactionCommand::GetName ()
{
    return (mName);
}

/// <summary>Sets the name of the long transaction to activate as a string.</summary>
/// <param name="value">Input the name of the long transaction</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEActivateLongTransactionCommand::SetName (FdoString* value)
{
    mName = ArcSDELongTransactionUtility::CheckName (value);
}

/// <summary>Executes the activate long transaction command. An exception is
/// thrown if the long transaction does not exist or the user does not
/// have access privileges to the long transaction.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDEActivateLongTransactionCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    SE_VERSIONINFO version;
    LONG id;
    LONG result;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
    conn = connection->GetConnection ();

    if (0 != wcscmp (GetName (), FDO_ACTIVELONGTRANSACTION))
    {
        if (0 == wcscmp (GetName (), FDO_ROOTLONGTRANSACTION))
            id = SDE_DEFAULT;
        else
        {
            // create the empty version object
            result = SE_versioninfo_create (&version);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

            // fetch the version
            ArcSDELongTransactionUtility::GetVersionByName (conn, GetName (), version);

            // get the version id
            result = SE_versioninfo_get_id (version, &id);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"id");

            SE_versioninfo_free (version);
        }

        // OK to activate
        connection->SetActiveVersion (id);
    }
    // else
    //     nothing to do
}

