/*
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
 */

#ifndef ARCSDELONGTRANSACTIONUTILITY_H
#define ARCSDELONGTRANSACTIONUTILITY_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Fdo.h"

class ArcSDELongTransactionUtility
{
protected:
    static wchar_t* ArcSDELongTransactionUtility::trim (wchar_t *str);

public:

    /// <summary>Checks the name of the long transaction.</summary>
    /// <param name="value">Input the name of the long transaction to check.</param> 
    /// <returns>Returns the value. Throws an exception is the value is invalid.</returns> 
    static FdoString* CheckName (FdoString* value);

    /// <summary>Checks the description of the long transaction.</summary>
    /// <param name="value">Input the description of the long transaction to check.</param> 
    /// <returns>Returns the value. Throws an exception is the value is invalid.</returns> 
    static FdoString* CheckDescription (FdoString* value);

    /// <summary>Fills in the SE_VERSIONINFO for the named version.</summary>
    /// ArcSDE documentation says <em>The version name and ID are unique.</em>
    /// This is partially inaccurate, the owner plus version name is unique.
    /// For qualified names, this just just fetches it directly.
    /// For unqualified names, searches public versions for that name.
    /// Throws an exception if there are more than one public version by that name found.
    /// <param name="connection">The connection to use.</param>
    /// <param name="name">Input the name of the long transaction to get.</param> 
    /// <param name="version">Output the allocated structure that will receive the version info.</param> 
    /// <returns>Returns nothing.</returns> 
    static void GetVersionByName (SE_CONNECTION connection, FdoString* name, SE_VERSIONINFO version);

    /// <summary>Fills in the SE_VERSIONINFO for the default version.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="version">Output the already allocated structure that will receive the version info.</param> 
    /// <returns>Returns nothing.</returns> 
    static void GetDefaultVersion (SE_CONNECTION connection, SE_VERSIONINFO version);

    /// <summary>Check if a table is versioned.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to check for versioning.</param>
    static bool IsVersioned (SE_CONNECTION connection, CHAR* table);

    /// <summary>Check if the version is owned by the current user.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="version">Input the version structure to check for ownership.</param>
    static bool IsOurVersion (SE_CONNECTION connection, SE_VERSIONINFO version);

    /// <summary>Make sure the given table is version enabled.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to version enable.</param>
    static void VersionEnable (SE_CONNECTION connection, CHAR* table);

    /// <summary>Delete a version and it's associated state.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="name">Input the name of the version to delete.</param>
    static void VersionDelete (SE_CONNECTION conn, FdoString* name);

    /// <summary>Set up the stream to be version aware.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="stream">Input the stream to adjust.</param>
    /// <param name="state">Input the state to apply.</param>
    static void ApplyStateToStream (SE_CONNECTION connection, SE_STREAM stream, LONG state);

    /// <summary>Make the stream agree with the connection regarding versioning.</summary>
    /// Checks if the connection has an active version, and if so version enables the table.
    /// Otherwise it checks if the table is versioned to see if versioning should
    /// be applied to the stream.
    /// The stream is then set to the state of the (active or default) version if needed.
    /// <param name="connection">The connection to use, with possibly an active version.</param>
    /// <param name="stream">Input the stream to adjust.</param>
    /// <param name="table">Input the table to check for versioning being enabled.</param>
    /// <param name="force">Input if true and the connection has an active version,
    /// the table is forced to be version enabled, otherwise the stream is not versioned.
    /// If true and there is no active state, an open state is created and made active,
    /// otherwise the base state for the version is used.</param>
    /// <returns>Returns true if there is versioning happening, false otherwise.
    /// This can be used to avoid calls to this method within the same Execute(),
    /// since if it returns false both the connection and the table agree that
    /// no versioning is being used.</returns> 
    static bool VersionStream (ArcSDEConnection* connection, SE_STREAM stream, CHAR* table, bool bLockVersion = false);

    /// <summary>Checks for children of a state.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="state">Input the state to check.</param> 
    /// <returns>Returns true if the state has child states.</returns> 
    static bool StateHasChildren (ArcSDEConnection* providerConnection, LONG state);

    /// <summary>Locks the state for the version given.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="version">Input the version structure that has the state to lock.</param> 
    /// <param name="open">Input If true, an open state is created ready for edit.</param> 
    /// <returns>Returns the active state.</returns> 
    static LONG LockVersion (ArcSDEConnection* providerConnection, SE_VERSIONINFO version, bool open = false);

    /// <summary>Unlocks the state for the version given.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="version">Input the version structure that has the state to unlock.</param> 
    /// <param name="state">Input the active state for the version.</param> 
    /// <returns>Returns nothing.</returns> 
    static void UnlockVersion (SE_CONNECTION connection, SE_VERSIONINFO version, LONG state);

    /// <summary>Get the name of the version given by the id number.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="id">The version id.</param>
    /// <param name="name">Input the buffer for the returned name.
    /// This buffer should be at least SE_MAX_VERSION_LEN characters long.</param> 
    /// <returns>Returns nothing.</returns> 
    static void GetVersionName (SE_CONNECTION connection, LONG id, CHAR* name);

    /// <summary>Create a child state of the given state.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="parent">The parent version id.</param>
    /// <returns>Returns the new state id.</returns> 
    static LONG CreateChildState (SE_CONNECTION connection, LONG parent);

    /// <summary>Delete the version given by the name.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="name">The version to delete.</param>
    /// <returns>Returns nothing.</returns> 
    static void DeleteVersion (SE_CONNECTION connection, CHAR* name);
};

#endif // ARCSDELONGTRANSACTIONUTILITY_H

