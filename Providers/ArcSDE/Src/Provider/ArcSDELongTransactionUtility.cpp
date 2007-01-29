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

/**
 * Remove blanks from the two ends of a string.
 */
wchar_t* ArcSDELongTransactionUtility::trim (wchar_t *str)
{
    wchar_t *p;
    wchar_t *q;

    p = str;
    q = str;
    while (iswspace (*p))
        p++;
    if (p != str)
    {
        while ('\0' != *p)
            *q++ = *p++;
        *q = *p;
    }
    else
        while ('\0' != *q)
            q++;
    // q now at terminating 0 of str
    if (q > str)
        while (iswspace (*(q - 1)) && ((q - 1) >= str))
            q--;
    *q = '\0';

    return (str);
}

/// <summary>Checks the name of the long transaction.</summary>
/// <param name="value">Input the name of the long transaction to check.</param> 
/// <returns>Returns the value. Throws an exception is the value is invalid.</returns> 
FdoString* ArcSDELongTransactionUtility::CheckName (FdoString* value)
{
    wchar_t* temp;
    CHAR* name;

    if (NULL == value)
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));
    temp = (wchar_t*)alloca ((wcslen (value) + 1) * sizeof (wchar_t));
    wcscpy (temp, value);
    temp = trim (temp);
    if (0 == wcslen (temp))
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));
    wide_to_multibyte (name, value);
    if (strlen (name) > SE_MAX_VERSION_INPUT_LEN)
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_VERSION_NAME_TOO_LONG, "Version name exceeds SE_MAX_VERSION_INPUT_LEN characters."));

    return (value);
}

/// <summary>Checks the description of the long transaction.</summary>
/// <param name="value">Input the description of the long transaction to check.</param> 
/// <returns>Returns the value. Throws an exception is the value is invalid.</returns> 
FdoString* ArcSDELongTransactionUtility::CheckDescription (FdoString* value)
{
    CHAR* description;
    static FdoString* blank = L"";

    if (NULL == value)
        value = blank;
    wide_to_multibyte (description, value);
    if (strlen (description) > SE_MAX_DESCRIPTION_LEN)
        throw FdoCommandException::Create (NlsMsgGet (ARCSDE_VERSION_DESCRIPTION_TOO_LONG, "Version description exceeds SE_MAX_DESCRIPTION_LEN characters."));
    
    return (value);
}

/// <summary>Check if a table is versioned.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="table">Input the table name to check for versioning.</param>
bool ArcSDELongTransactionUtility::IsVersioned (SE_CONNECTION connection, CHAR* table)
{
    LONG result;
    SE_REGINFO  registration;
    bool ret;
        
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (connection, table, registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
    ret = 0 != SE_reginfo_is_multiversion (registration);
    SE_reginfo_free (registration);

    return (ret);
}

/// <summary>Check if the version is owned by the current user.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="version">Input the version structure to check for ownership.</param>
bool ArcSDELongTransactionUtility::IsOurVersion (SE_CONNECTION connection, SE_VERSIONINFO version)
{
    LONG result;
    CHAR owner[SE_MAX_VERSION_LEN];
    CHAR user_name[SE_MAX_OWNER_LEN];
    bool ret;

    result = SE_versioninfo_get_name (version, owner);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"name");
    *(strchr (owner, '.')) = '\0';
    result = SE_connection_get_user_name (connection, user_name);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
    ret = (0 == strcmp (owner, user_name));

    return (ret);
}

/// <summary>Make sure the given table is version enabled.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="table">Input the table name to version enable.</param>
void ArcSDELongTransactionUtility::VersionEnable (SE_CONNECTION connection, CHAR* table)
{
    LONG result;
    SE_REGINFO  registration;
        
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (connection, table, registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
    result = SE_reginfo_set_multiversion (registration, TRUE);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"multiversion");
    result = SE_registration_alter (connection, registration);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ALTER, "Registration info could not be altered.");
    SE_reginfo_free (registration);
}

/// <summary>Fills in the SE_VERSIONINFO for the named version.</summary>
/// For qualified names, just fetches checks directly.
/// For unqualified names, searches public versions for that name.
/// <param name="connection">The connection to use.</param>
/// <param name="name">Input the name of the long transaction to get.</param> 
/// <param name="version">Output the allocated structure that will receive the version info.</param> 
/// <returns>Returns nothing.</returns> 
void ArcSDELongTransactionUtility::GetVersionByName (SE_CONNECTION connection, FdoString* name, SE_VERSIONINFO version)
{
    static CHAR query[] = "NAME = '%s' and STATUS = 1";
    CHAR* version_owner;
    CHAR* version_name;
    CHAR user_name[SE_MAX_OWNER_LEN];
    CHAR qualified_name[SE_MAX_VERSION_LEN + 1 + SE_MAX_OWNER_LEN];
    bool qualified;
    SE_VERSIONINFO *list;
    LONG count;
    LONG id;
    CHAR where[SE_MAX_VERSION_LEN + sizeof(query)];
    LONG result;

    if (NULL == name)
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));
    version_owner = NULL;
    wide_to_multibyte (version_owner, name);
    if (NULL == version_owner)
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));

    version_name = strchr (version_owner, '.');
    if (NULL != version_name)
    {
        *version_name = '\0';
        version_name++;
        qualified = true;
    }
    else
    {   // unqualified name provided
        version_name = version_owner;
        result = SE_connection_get_user_name (connection, user_name);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");
        version_owner = user_name;
        qualified = false;
    }

    // fetch the version
    if (!qualified)
    {
        // try unqualified name first (i.e. current users versions)
        // ArcSDE docs say "If more than one version fits, the most recently created version is returned."
        result = SE_version_get_info (connection, version_name, version);
        if (SE_VERSION_NOEXIST == result)
        {   // doesn't exist, get the list
            // Note: SE_VERSION_ACCESS_PUBLIC is defined as 0, but the database contains a
            // status of 1 for public versions (0 for private, 2 for protected),
            // so presumably it's bit coded
            sprintf (where, query, version_name);
            result = SE_version_get_info_list (connection, where, &list, &count);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_LIST, "Version info list could not be retrieved.");
            if (0 == count)
                throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", name));
            else if (1 < count)
            {
                SE_version_free_info_list (count, list);
                throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_UNIQUE_VERSION_NAME, "More than one version is named '%1$ls'.", name));
            }
            else
            {
                result = SE_versioninfo_get_id (list[0], &id);
                handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"id");
                result = SE_version_get_info_by_id (connection, id, version);
                SE_version_free_info_list (count, list);
            }
        }
    }
    else
    {   // try only the exact name
        strcpy (qualified_name, version_owner);
        strcat (qualified_name, ".");
        strcat (qualified_name, version_name);
        result = SE_version_get_info (connection, qualified_name, version);
    }
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", name);
}

/// <summary>Fills in the SE_VERSIONINFO for the default version.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="version">Output the allocated structure that will receive the version info.</param> 
/// <returns>Returns nothing.</returns> 
void ArcSDELongTransactionUtility::GetDefaultVersion (SE_CONNECTION connection, SE_VERSIONINFO version)
{
    wchar_t* name;

    multibyte_to_wide (name, SE_QUALIFIED_DEFAULT_VERSION_NAME);
    GetVersionByName (connection, name, version);
}

/// <summary>Delete a version and it's associated state.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="name">Input the name of the version to delete.</param>
void ArcSDELongTransactionUtility::VersionDelete (SE_CONNECTION conn, FdoString* name)
{
    SE_VERSIONINFO version;
    CHAR* version_name;
    LONG result;
    LONG state;

    // get the name
    if (NULL == name)
        throw FdoException::Create (NlsMsgGet (ARCSDE_VERSION_NULL, "Version name cannot be NULL."));
    version_name = NULL;
    wide_to_multibyte (version_name, name);

    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

    result = SE_version_get_info (conn, version_name, version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", name);
    result = SE_versioninfo_get_state_id (version, &state);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"State Id");

    result = SE_version_delete (conn, version_name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_DELETE_FAILED, "Cannot delete version '%1$ls'.", name);

    // NOTE: the state may be in use if this version's state matches its parent's version state, in which case the delete will fail
    // (with one of a variety of errors, depending on the condition the state is in) and we should leave the state alone without throwing an exception.
    result = SE_state_delete (conn, state);
    // NOTE: we purposefully ignore any errors returned from SE_state_delete() for two reasons:
    // 1) This function's main purpose is to delete the version, not the state. If we get this far, te
    //     version has been deleted successfully.
    // 2) SE_state_delete() may throw many different exceptions if the state still in use after the version is
    // deleted, since states can be shared by multiple versions (e.g. SE_STATE_USED_BY_VERSION, SE_STATE_INUSE,
    // SE_STATE_HAS_CHILDREN, SE_NO_PERMISSIONS, etc)

    SE_versioninfo_free (version);
}

/// <summary>Set up the stream to be version aware.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="stream">Input the stream to adjust.</param>
/// <param name="state">Input the state to apply.</param>
void ArcSDELongTransactionUtility::ApplyStateToStream (SE_CONNECTION connection, SE_STREAM stream, LONG state)
{
    LONG result;

    result = SE_stream_set_state (stream, state, SE_NULL_STATE_ID, SE_STATE_DIFF_NOCHECK);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STREAM_NOT_VERSIONED, "Stream could not be set to version state.");
}

bool ArcSDELongTransactionUtility::VersionStream (ArcSDEConnection* connection, SE_STREAM stream, CHAR* table, bool bLockVersion)
{
    LONG version;
    LONG state;
    bool ret;

    version = connection->GetActiveVersion ();

    // check if table is already versioned
    ret = ArcSDELongTransactionUtility::IsVersioned (connection->GetConnection (), table);

    // if the table is versioned, apply the state to the stream
    if (ret)
    {
        // make sure there is an active state
        state = connection->GetActiveState ();
        if (-1L == state)
        {
            LONG result;
            SE_VERSIONINFO info;
            wchar_t buffer[50];

            // create the empty version object
            result = SE_versioninfo_create (&info);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

            // get version info
            result = SE_version_get_info_by_id (connection->GetConnection (), connection->GetActiveVersion (), info);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (connection->GetActiveVersion (), buffer, ELEMENTS(buffer)));

            // get the state id
            result = SE_versioninfo_get_state_id (info, &state);
            handle_sde_err<FdoCommandException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"StateId");

            if (bLockVersion || (0 == state)) // if state = 0, SE_stream_set_state doesn't work
            {
                connection->SetActiveState (LockVersion (connection, info, true));
                state = connection->GetActiveState ();
            }

            SE_versioninfo_free (info);
        }
        ArcSDELongTransactionUtility::ApplyStateToStream (connection->GetConnection (), stream, state);
    }

    return (ret);
}

/// <summary>Checks for children of a state.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="state">Input the state to check.</param> 
/// <returns>Returns true if the state has child states.</returns> 
bool ArcSDELongTransactionUtility::StateHasChildren (ArcSDEConnection* providerConnection, LONG state)
{
    CHAR where[512];
    SE_STATEINFO* infos;
    LONG count;
    LONG result;
    bool ret;
    SE_CONNECTION connection = providerConnection->GetConnection();

    // Build qualified system table name:
    wchar_t wcsStateLineagesTable[50];  // should be more than enough to hold longest name
    FdoCommonOSUtil::swprintf(wcsStateLineagesTable, ELEMENTS(wcsStateLineagesTable), L"%ls%ls", providerConnection->RdbmsSystemTablePrefix(), SDE_SYSTEM_TABLE_STATE_LINEAGES);

    // assumes states numbers are allocated in increasing order (Oracle sequence: SDE.STATE_ID_GENERATOR_NC)
    sprintf (where, "STATE_ID in (select LINEAGE_ID from %ls where LINEAGE_NAME in (select LINEAGE_NAME from %ls where LINEAGE_ID = %ld)) and STATE_ID > %ld", (const wchar_t*)wcsStateLineagesTable, (const wchar_t*)wcsStateLineagesTable, state, state);

    result = SE_state_get_info_list (connection, where, &infos, &count);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_LIST, "State info list could not be retrieved.");

    ret = 0 != count;
    if (ret)
        SE_state_free_info_list (count, infos);

    return (ret);
}

/// <summary>Locks the state for the version given.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="version">Input the version structure that has the state to lock.</param> 
/// <param name="open">Input If true, an open state is created ready for edit.</param> 
/// <returns>Returns the active state.</returns> 
LONG ArcSDELongTransactionUtility::LockVersion (ArcSDEConnection* providerConnection, SE_VERSIONINFO version, bool open)
{
    CHAR name[SE_MAX_VERSION_LEN];
    wchar_t* wname;
    LONG result;
    LONG state;
    SE_STATEINFO info;
    CHAR owner[SE_MAX_OWNER_LEN];
    CHAR user_name[SE_MAX_OWNER_LEN];
    bool children;
    LONG id;
    LONG ret;
    SE_CONNECTION connection = providerConnection->GetConnection();

    // get the version name... we need it for error messages
    SE_versioninfo_get_name (version, name);
    multibyte_to_wide (wname, name);

    // get the state
    result = SE_versioninfo_get_state_id (version, &state);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"StateId");

    // create the state info object
    result = SE_stateinfo_create (&info);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ALLOC, "Cannot initialize SE_STATEINFO structure.");

    // populate it with the state information
    result = SE_state_get_base_info (connection, state, info);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_INFO, "State info for '%1$d' could not be retrieved.", state);

    // get the state owner
    result = SE_stateinfo_get_owner (info, owner);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ITEM, "State info item '%1$ls' could not be retrieved.", L"owner");

    // get the current user
    result = SE_connection_get_user_name (connection, user_name);
    handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_USER_UNKNOWN, "Cannot determine current user.");

    // check for children of this state
    children = StateHasChildren (providerConnection, state);

    // if they aren't the same people or the state has children, create a new state for the version
    if (open && ((0 != strcmp (owner, user_name)) || children))
    {
        // make a new state
        state = ArcSDELongTransactionUtility::CreateChildState (connection, state);

        // switch the version to the new state
        result = SE_version_change_state (connection, version, state);
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_VERSION_STATE_CHANGE_FAILURE, "Could not set state '%1$d' for version '%2$ls'.", state, wname);

        // refetch the version info
        result = SE_versioninfo_get_id (version, &id);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");
        result = SE_version_get_info_by_id (connection, id, version);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", wname);
    }

    // lock the base state for the version, only the owner can lock a state
    if (SE_BASE_STATE_ID != state) // not even SDE can lock the base state
    {
        result = SE_state_lock (connection, state);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_LOCK_FAILURE, "State '%1$d' for version '%1$ls' could not be locked.", state, wname);
    }

    if (open)
    {
        // open it, only open states can be written to
        result = SE_state_open (connection, state);
        if (SE_STATE_INUSE == result)
        {
            // uh-oh somebody else using our name has locked this state
            // we don't unlock the state because the other person was the one who really locked it

            // make a new state
            state = ArcSDELongTransactionUtility::CreateChildState (connection, state);

            // switch the version to the new state
            result = SE_version_change_state (connection, version, state);
            handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_VERSION_STATE_CHANGE_FAILURE, "Could not set state '%1$d' for version '%2$ls'.", state, wname);

            // refetch the version info
            result = SE_versioninfo_get_id (version, &id);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");
            result = SE_version_get_info_by_id (connection, id, version);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", wname);

            // lock this new state
            result = SE_state_lock (connection, state);
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_LOCK_FAILURE, "State '%1$d' for version '%1$ls' could not be locked.", state, wname);

            // open it, only open states can be written to
            result = SE_state_open (connection, state);
        }
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_OPEN, "Cannot open state '%1$d'.", state);

        ret = state;
    }
    else
        ret = -1L;

    SE_stateinfo_free (info);

    return (ret);
}

/// <summary>Unlocks the state for the version given.</summary>
/// <param name="connection">The connection to use.</param>
/// <param name="version">Input the version structure that has the state to unlock.</param> 
/// <param name="state">Input the active state for the version.</param> 
/// <returns>Returns nothing.</returns> 
void ArcSDELongTransactionUtility::UnlockVersion (SE_CONNECTION connection, SE_VERSIONINFO version, LONG state)
{
    CHAR name[SE_MAX_VERSION_LEN];
    wchar_t* wname;
    LONG result;
    LONG id;

    if (-1L != state)
    {
        // get the version name... we need it for error messages
        SE_versioninfo_get_name (version, name);
        multibyte_to_wide (wname, name);

        // close the state
        result = SE_state_close (connection, state);
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_STATE_CLOSE, "Cannot close state '%1$d'.", state);

        // get the version state
        result = SE_versioninfo_get_state_id (version, &id);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"StateId");

        // remove the lock on the version state
        result = SE_state_free_lock (connection, id);
        if (SE_NO_LOCKS != result) // we allow a 'no locks' scenario because the SDE.DEFAULT might not be locked
            handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_STATE_UNLOCK_FAILURE, "State '%1$d' for version '%2$ls' could not be unlocked.", id, wname);

        // switch the version to the new state
        result = SE_version_change_state (connection, version, state);
        handle_sde_err<FdoCommandException>(connection, result, __FILE__, __LINE__, ARCSDE_VERSION_STATE_CHANGE_FAILURE, "Could not set state '%1$d' for version '%2$ls'.", state, wname);

        // refetch the version info
        result = SE_versioninfo_get_id (version, &id); // reuse id
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");
        result = SE_version_get_info_by_id (connection, id, version);
        handle_sde_err<FdoCommandException> (connection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", wname);
    }
}

void ArcSDELongTransactionUtility::GetVersionName (SE_CONNECTION conn, LONG id, CHAR* name)
{
    LONG result;
    SE_VERSIONINFO version;
    wchar_t buffer[50];

    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");
    result = SE_version_get_info_by_id (conn,  id, version);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (id, buffer, ELEMENTS(buffer)));
    result = SE_versioninfo_get_name (version, name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"name");
    SE_versioninfo_free (version);
}

LONG ArcSDELongTransactionUtility::CreateChildState (SE_CONNECTION conn, LONG parent)
{
    LONG result;
    SE_STATEINFO state;
    LONG real_parent;
    LONG ret;

    result = SE_stateinfo_create (&state);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ALLOC, "Cannot initialize SE_STATEINFO structure.");
    result = SE_state_get_info (conn, parent, state);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO, "State info for '%1$d' could not be retrieved.", parent);
    real_parent = -1L;
    if (SE_stateinfo_is_open (state)) // only closed states can have child states
    {
        result = SE_state_close (conn, parent);
        if ((SE_NO_PERMISSIONS == result) || (SE_STATE_INUSE == result))
        {
            // it's not our state or somebody's editing it so we need to clone the parent, starting from the parent of the parent
            real_parent = parent;
            result = SE_stateinfo_get_parent (state, &parent);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ITEM, "State info item '%1$ls' could not be retrieved.", L"Parent");
            result = SE_state_get_base_info (conn, parent, state);
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO, "State info for '%1$d' could not be retrieved.", parent);
            // Note: the parent of the parent must be closed
        }
        else
            handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_CLOSE, "Cannot close state '%1$d'.", parent);
    }
    result = SE_state_create (conn, state, parent, state);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_CREATE, "Could not create state.");
    result = SE_stateinfo_get_id (state, &ret);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ITEM, "State info item '%1$ls' could not be retrieved.", L"Id");

    // if the parent was open and couldn't be closed, make a clone
    // by copying all changes from real_parent to the new state
    if (-1L != real_parent)
    {
        // SE_state_merge applies changes to a parent state to create a new merged state.
        // The new state is the child of the parent state with the changes of the second state.
        // Both input states must have the same parent state.
        // When a row has been changed in both  parent and second states,
        // the row from the changes state is used.
        // The parent and changes states must be open or owned by the current user
        // unless the current user is the ArcSDE DBA.
        result = SE_state_merge (conn, ret, real_parent, state);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_MERGE_FAILURE, "Cannot merge changes from state '%1$d' into a new state '%2$d'.", real_parent, ret);
        // necessary? is there really a new state?
        result = SE_stateinfo_get_id (state, &ret);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_STATE_INFO_ITEM, "State info item '%1$ls' could not be retrieved.", L"Id");
    }

    SE_stateinfo_free (state);

    return (ret);
}

void ArcSDELongTransactionUtility::DeleteVersion (SE_CONNECTION conn, CHAR* name)
{
    wchar_t* wname;
    LONG result;

    multibyte_to_wide (wname, name);
    result = SE_version_delete (conn, name);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_DELETE_FAILED, "Cannot delete version '%1$ls'.", wname);
}


