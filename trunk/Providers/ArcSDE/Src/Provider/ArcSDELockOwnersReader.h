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

#ifndef ARCSDELOCKOWNERSREADER_H
#define ARCSDELOCKOWNERSREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDELockOwnersReader :
    public FdoILockOwnersReader
{
    typedef FdoArray<wchar_t*> OwnerArray;

    friend class ArcSDEGetLockOwnersCommand;

    FdoPtr<OwnerArray> mOwners; // collection of owners

    int mIndex; // current index into the owners list

protected:

    ArcSDELockOwnersReader ();
    virtual ~ArcSDELockOwnersReader (void);

    /// <summary>Add an owner to the reader's list.</summary>
    /// <param name="owner">The owner to add to the list.</param>
    virtual void AddOwner (wchar_t* owner);

    /// <summary>Checks that the reader is in a valid state.</summary>
    virtual void ValidateIndices ();

public:

    // FdoIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoILockOwnersReader interface

    /// <summary>Gets the name of an owner that currently holds a lock on on one or more
    /// objects.</summary>
    /// <returns>Returns the lock owner</returns> 
    FdoString* GetLockOwner ();

    /// <summary>Advances the reader to the next item. The default position of the 
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    bool ReadNext ();

    /// <summary>Closes the FdoILockOwnersReader object, freeing any resources it may be
    /// holding.</summary>
    /// <returns>Returns nothing</returns> 
    void Close ();
};

#endif // ARCSDELOCKOWNERSREADER_H

