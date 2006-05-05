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

ArcSDELockOwnersReader::ArcSDELockOwnersReader () :
    mOwners (OwnerArray::Create ()),
    mIndex (-1)
{
}

ArcSDELockOwnersReader::~ArcSDELockOwnersReader (void)
{
    for (int i = 0; i < mOwners->GetCount (); i++)
        delete[] (*mOwners)[i];
}

/// <summary>Add an owner to the reader's list.</summary>
/// <param name="owner">The owner to add to the list.</param>
void ArcSDELockOwnersReader::AddOwner (wchar_t* owner)
{
    wchar_t* p;

    for (int i = 0; i < mOwners->GetCount (); i++)
        if (0 == wcscmp ((*mOwners)[i], owner))
            return;

    p = new wchar_t[wcslen (owner) + 1];
    wcscpy (p, owner);
    mOwners.p = mOwners->Append (mOwners, p);
}

/// <summary>Checks that the reader is in a valid state.</summary>
void ArcSDELockOwnersReader::ValidateIndices ()
{
    if (-1 == mIndex)
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));
    if (-2 == mIndex)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    if (mIndex >= mOwners->GetCount ())
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ArcSDELockOwnersReader::Dispose ()
{
    delete this;
}

// FdoILockOwnersReader interface

/// <summary>Gets the name of an owner that currently holds a lock on on one or more
/// objects.</summary>
/// <returns>Returns the lock owner</returns> 
FdoString* ArcSDELockOwnersReader::GetLockOwner ()
{
    ValidateIndices ();

    return (((*mOwners)[mIndex]));
}

/// <summary>Advances the reader to the next item. The default position of the 
/// reader is prior to the first item. Thus, you must call ReadNext
/// to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDELockOwnersReader::ReadNext ()
{
    bool ret;

    ret = false;

    if (-1 == mIndex)
        mIndex = 0;
    else if (-2 == mIndex)
        throw FdoException::Create (NlsMsgGet (ARCSDE_READER_CLOSED, "Reader is closed."));
    else
        mIndex++;
    ret = (mIndex < mOwners->GetCount ());

    return (ret);
}

/// <summary>Closes the FdoILockOwnersReader object, freeing any resources it may be
/// holding.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDELockOwnersReader::Close ()
{
    mIndex = -2;
}

