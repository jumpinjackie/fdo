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
#include <string.h>

#include <ArcSDELongTransactionReader.h>

ArcSDELongTransactionReader::ArcSDELongTransactionReader (ArcSDEConnection* connection, LONG count, SE_VERSIONINFO* info) :
    mConnection (connection),
    mCount (count),
    mInfo (info),
    mIndex (-1),
    mName (NULL),
    mDescription (NULL),
    mOwner (NULL)
{
}

ArcSDELongTransactionReader::~ArcSDELongTransactionReader (void)
{
    Close ();
}

/** Do not implement the copy constructor. **/
//ArcSDELongTransactionReader::ArcSDELongTransactionReader (const ArcSDELongTransactionReader &right) { }

void ArcSDELongTransactionReader::Dispose ()
{
    delete this;
}

void ArcSDELongTransactionReader::reset ()
{
    if (NULL != mName)
    {
        delete[] mName;
        mName = NULL;
    }

    if (NULL != mDescription)
    {
        delete[] mDescription;
        mDescription = NULL;
    }

    if (NULL != mOwner)
    {
        delete[] mOwner;
        mOwner = NULL;
    }
}

void ArcSDELongTransactionReader::validate ()
{
    if (NULL == mInfo)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    if (mIndex >= mCount)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
    if (mIndex == -1)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));
}

/// <summary>Gets the name of the long transaction currently being read.</summary>
/// <returns>Returns the name of the long transaction.</returns> 
FdoString* ArcSDELongTransactionReader::GetName ()
{
    char name[SE_MAX_VERSION_LEN];
    wchar_t* temp;

    validate ();
    if (NULL == mName)
        if (SE_SUCCESS == SE_versioninfo_get_name (mInfo[mIndex], name))
        {
            multibyte_to_wide (temp, name);
            mName = new wchar_t[wcslen (temp) + 1];
            wcscpy ((wchar_t*)mName, temp);
        }

    return (mName);
}

/// <summary>Gets the description of the long transaction currently being read.</summary>
/// <returns>Returns the description of the long transaction.</returns> 
FdoString* ArcSDELongTransactionReader::GetDescription ()
{
    char description[SE_MAX_DESCRIPTION_LEN];
    wchar_t* temp;

    validate ();
    if (NULL == mDescription)
        if (SE_SUCCESS == SE_versioninfo_get_description (mInfo[mIndex], description))
        {
            multibyte_to_wide (temp, description);
            mDescription = new wchar_t[wcslen (temp) + 1];
            wcscpy ((wchar_t*)mDescription, temp);
        }

    return (mDescription);
}

/// <summary>Gets the owner of the long transaction currently being read.</summary>
/// <returns>Returns the owner name.</returns> 
FdoString* ArcSDELongTransactionReader::GetOwner ()
{
    char owner[SE_MAX_VERSION_LEN];
    char* q;
    wchar_t* temp;

    validate ();
    if (NULL == mOwner)
        if (SE_SUCCESS == SE_versioninfo_get_name (mInfo[mIndex], owner))
        {
            // truncate after the owner
            q = strchr (owner, '.');
            if (NULL == q)
                strcpy (owner, "");
            else
                *q = '\0';
            multibyte_to_wide (temp, owner);
            mOwner = new wchar_t[wcslen (temp) + 1];
            wcscpy ((wchar_t*)mOwner, temp);
        }

    return (mOwner);
}

/// <summary>Gets the creation date of the long transaction currently being read.</summary>
/// <returns>Returns the date when the long transaction was created.</returns> 
FdoDateTime ArcSDELongTransactionReader::GetCreationDate ()
{
    struct tm time;
    FdoDateTime ret;

    validate ();
    if (SE_SUCCESS == SE_versioninfo_get_creation_time (mInfo[mIndex], &time))
    {
        ret.year = time.tm_year + 1900;
        ret.month = time.tm_mon + 1;
        ret.day = time.tm_mday;
        ret.hour = time.tm_hour;
        ret.minute = time.tm_min;
        ret.seconds = (float)time.tm_sec;
    }

    return (ret);
}

/// <summary>Returns true if the long transaction currently being read is the
/// active long transaction.</summary>
/// <returns>Returns true if the long transaction is active.</returns> 
bool ArcSDELongTransactionReader::IsActive ()
{
    LONG id;
    bool ret;

    ret = false;

    validate ();
    if (SE_SUCCESS == SE_versioninfo_get_id (mInfo[mIndex], &id))
        ret = (id == mConnection->GetActiveVersion ());

    return (ret);
}

/// <summary>Returns true if the long transaction currently being read is locked.</summary>
/// <returns>Returns true if the long transaction is locked by anybody.</returns> 
bool ArcSDELongTransactionReader::IsFrozen ()
{
    char name[SE_MAX_VERSION_LEN];
    LONG lock_count;
    SE_VERSION_LOCK *locks;
    bool ret;

    ret = false;

    validate ();
    if (SE_SUCCESS == SE_versioninfo_get_name (mInfo[mIndex], name))
        if (SE_SUCCESS == SE_version_get_locks (mConnection->GetConnection (), name, &lock_count, &locks))
        {
            for (int i = 0; i < lock_count && !ret; i++)
                if (   (SE_VERSION_EXCLUSIVE_LOCK == locks[i].lock_mode)
                    || (SE_VERSION_EXCLUSIVE_AUTOLOCK == locks[i].lock_mode))
                    ret = true;
            SE_version_free_locks (locks, lock_count);
        }

    return (ret);
}

/// <summary>Advances the reader to the next item. The default position of the
/// reader is prior to the first item. Thus, you must call ReadNext
/// to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDELongTransactionReader::ReadNext ()
{
    reset ();
    mIndex++;
    return (mIndex < mCount);
}

/// <summary>Closes the FdoILongTransactionReader object, freeing any resources it may
/// be holding.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDELongTransactionReader::Close ()
{
    reset ();
    if (NULL != mInfo)
        SE_version_free_info_list (mCount, mInfo);
    mInfo = NULL;
}

/// <summary>Provides access to the direct descendent long transactions 
/// for the long transaction being currently read. The caller can use the
/// returned reader to get to the next level of descendents if required.
/// The returned reader is empty if there are no descendents for the long
/// transction.</summary>
/// <returns>Returns a long transaction reader with the direct descendents
/// for the long transaction currently being read.</returns> 
FdoILongTransactionReader* ArcSDELongTransactionReader::GetChildren ()
{
    SE_CONNECTION conn;
    LONG id;
    CHAR where[50];
    LONG result;
    SE_VERSIONINFO* info;
    LONG count;

    validate ();
    conn = mConnection->GetConnection ();
    result = SE_versioninfo_get_id (mInfo[mIndex], &id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"Id");
    sprintf (where, "PARENT_VERSION_ID = %ld", id);
    result = SE_version_get_info_list (conn, where, &info, &count);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_LIST, "Version info list could not be retrieved.");

    return (new ArcSDELongTransactionReader (mConnection, count, info));
}

/// <summary>Provides access to the direct ascendant long transactions for
/// the long transaction being currently read. The caller can use the 
/// returned reader to get to the next level of ascendants if required.
/// The returned reader is empty if there are no ascendants for the long
/// transaction</summary>
/// <returns>Returns a long transaction reader with the direct ascendants 
/// for the long transaction currently being read.</returns> 
FdoILongTransactionReader* ArcSDELongTransactionReader::GetParents ()
{
    SE_CONNECTION conn;
    LONG id;
    CHAR where[50];
    LONG result;
    SE_VERSIONINFO* info;
    LONG count;
    FdoPtr<FdoILongTransactionReader> ret;

    validate ();
    conn = mConnection->GetConnection ();
    id = 0;
    result = SE_versioninfo_get_parent_id (mInfo[mIndex], &id);
    handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"ParentId");
    if (0 != id)
    {
        sprintf (where, "VERSION_ID = %ld", id);
        result = SE_version_get_info_list (conn, where, &info, &count);
        handle_sde_err<FdoCommandException> (conn, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_LIST, "Version info list could not be retrieved.");
        ret = new ArcSDELongTransactionReader (mConnection, count, info);
    }
    else
        ret = new ArcSDELongTransactionReader (mConnection, 0, NULL);

    return (FDO_SAFE_ADDREF (ret.p));
}

