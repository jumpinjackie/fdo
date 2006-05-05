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

#include <ArcSDELongTransactionReader.h>

ArcSDEGetLongTransactionsCommand::ArcSDEGetLongTransactionsCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIGetLongTransactions> (connection),
    mName ()
{
}

ArcSDEGetLongTransactionsCommand::~ArcSDEGetLongTransactionsCommand (void)
{
}

/// <summary>Sets the name of the long transaction for which information
/// should be retrieved. The function can also be used to nullify the
/// property by using a NULL pointer as the function's parameter. If the
/// the property is not set, the function returns information for all
/// available long transactions.</summary>
/// <returns>Returns nothing</returns>
void ArcSDEGetLongTransactionsCommand::SetName (FdoString* value)
{
    mName = ArcSDELongTransactionUtility::CheckName (value);
}

/// <summary>Gets the currently set name of the long transaction for which
/// the long transaction information should be retrieved. If the property
/// is not set, the function returns a NULL pointer.</summary>
/// <returns>Returns a long transaction name.</returns>
FdoString* ArcSDEGetLongTransactionsCommand::GetName ()
{
    return (mName);
}

/// <summary>Executes the GetLongTransactions command returning a reference to an
/// FdoILongTransactionReader.</summary>
/// <returns>Returns the long transaction reader</returns> 
FdoILongTransactionReader* ArcSDEGetLongTransactionsCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    SE_CONNECTION conn;
    SE_VERSIONINFO* info;
    SE_VERSIONINFO version;
    CHAR *where;
    char temp[SE_MAX_VERSION_LEN + SE_MAX_OWNER_LEN + 25];
    CHAR user_name [SE_MAX_OWNER_LEN];
    LONG id;
    LONG count;
    LONG result;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));
    conn = connection->GetConnection ();

    //    - if set to NULL, get all LT's
    //    - if set to FDO_ROOTLONGTRANSACTION, get only the root long transaction
    //    - if set to FDO_ACTIVELONGTRANSACTION, get only the active long transaction
    //    - if set to anything else, get the long transaction of the given name

    where = NULL;
    // if no name specified (NULL or empty), get all LT's by using a NULL where clause
    if (GetName () != NULL && (0 != wcslen (GetName ())))
    {
        if (0 == wcscmp (GetName (), FDO_ROOTLONGTRANSACTION))
        {
            // get the version number for the DEFAULT version
            if (0)
            {
                result = SE_versioninfo_create (&version);
                handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

                ArcSDELongTransactionUtility::GetDefaultVersion (conn, version);

                result = SE_versioninfo_get_id (version, &id);
                handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"id");

                SE_versioninfo_free (version);
            }
            else
                // the above is a lot of work, we could just use the constant '1' since SDE.DEFAULT is version 1
                id = SDE_DEFAULT;

            sprintf (temp, "VERSION_ID = %d", id);
            where = temp;
        }
        else if (0 == wcscmp (GetName (), FDO_ACTIVELONGTRANSACTION))
        {
            // if set to FDO_ACTIVELONGTRANSACTION, get only the active long transaction
            // Note: this returns no versions if the active version is not set (i.e. has a value of -1)
            sprintf (temp, "VERSION_ID = %d", connection->GetActiveVersion ());
            where = temp;
        }
        else
        {
            // if set to anything else, get the long transaction of the given name
            // Note: This may return multiple versions owned by different users, so we add the user criteria
            CHAR* version_owner;
            CHAR* version_name;

            wide_to_multibyte (version_owner, (wchar_t*)GetName ());
            version_name = strchr (version_owner, '.');
            if (NULL != version_name)
            {
                *version_name = '\0';
                version_name++;
            }
            else
            {   // unqualified name provided
                version_name = version_owner;
                result = SE_connection_get_user_name (conn, user_name);
                handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
                version_owner = user_name;
            }
            sprintf (temp, "NAME = '%s' and OWNER = '%s'", version_name, version_owner);
            where = temp;
        }
    }

    result = SE_version_get_info_list (conn, where, &info, &count);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_LIST, "Version info list could not be retrieved.");

    return (new ArcSDELongTransactionReader (connection, count, info));
}

