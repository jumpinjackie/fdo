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

#ifndef ARCSDELOCKCONFLICTREADER_H
#define ARCSDELOCKCONFLICTREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDESelectCommand;
class ArcSDEFeatureReader;

class ArcSDELockConflictReader :
    public FdoILockConflictReader
{
    typedef FdoArray<LONG> IdArray;

    friend class ArcSDESelectCommand;
    friend class ArcSDEUpdateCommand;
    friend class ArcSDEDeleteCommand;
    friend class ArcSDEAcquireLockCommand;
    friend class ArcSDEReleaseLockCommand;
    friend class ArcSDEFeatureReader;
    friend class ArcSDELockUtility;

    FdoPtr<ArcSDEConnection> mConnection; // our connection object for accessing ArcSDE API
    FdoStringP mClassName; // class name for objects in this conflict reader
    CHAR* mTable; // ArcSDE table name corresponding to that class
    FdoStringP mIdProperty; // property name for the row id column
    IdArray* mIds; // list of conflicting row ids
    int mIndex; // current index into the conflict list

    // user name determination variables
    LONG mNumLocks; // the number of locks in ArcSDE's list
    LONG* mRowIds; // ArcSDE's list of locks
    CHAR** mUserNames; // ArcSDE's list of users corresponding to those locks
    FdoStringP mUser; // cached user name

    // GetIdentity objects, reused if the refcount allows
    FdoPtr<FdoPropertyValueCollection> mIdentity;

    ArcSDESelectCommand* mSelect; // can't use FdoPtr<> because of circular reference
    ArcSDEFeatureReader* mReader; // can't use FdoPtr<> because of circular reference

protected:
    // 0-argument constructor to please FdoPtr::operator->() :
    ArcSDELockConflictReader() { ArcSDELockConflictReader(NULL, L"", NULL, L""); }

    ArcSDELockConflictReader (ArcSDEConnection* connection, FdoString* cls, CHAR* table, FdoString* property);
    virtual ~ArcSDELockConflictReader (void);

    /// <summary>Add a row id to the reader's list.</summary>
    /// <param name="id">The row id of the conflict.</param>
    virtual void AddIdentity (LONG id);

    /// <summary>Checks that the reader is in a valid state.</summary>
    virtual void ValidateIndices ();

    /// <summary>Checks if we're the only one hanging onto the GetIdentity objects.</summary>
    // <returns>Returns true if we should allocate another set of identity objects.</returns> 
    virtual bool Referenced ();

    virtual void SetSelectCommand (ArcSDESelectCommand* select);
    virtual ArcSDESelectCommand* GetSelectCommand ();
    virtual void SetFeatureReader (ArcSDEFeatureReader* reader);
    virtual ArcSDEFeatureReader* GetFeatureReader ();

public:

    // FdoIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoILockConflictReader overrides

    /// <summary>Gets the class name of the feature currently being read.</summary>
    /// <returns>Returns the class name.</returns> 
    FdoString* GetFeatureClassName ();

    /// <summary>Returns an FdoPropertyValueCollection containing the property values
    /// that uniquely identify feature currently being read.</summary>
    /// <returns>Returns the property collection identifying the feature.</returns> 
    FdoPropertyValueCollection* GetIdentity ();

    /// <summary>Gets the name of the owner that holds a lock on the feature 
    /// currently being read.</summary>
    /// <returns>Returns the name of the owner.</returns> 
    FdoString* GetLockOwner ();

    /// <summary>Gets the conflict type for the conflict currently being read.</summary>
    /// <returns>Returns the conflict type for the conflict currently being read.</returns> 
    FdoConflictType GetConflictType ();

    /// <summary>Gets the name of the long transaction for the feature currently
    /// being read.</summary>
    /// <returns>Returns the name of the long transaction.</returns> 
    FdoString* GetLongTransaction ();

    /// <summary>Advances the reader to the next item. The default position of the 
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    bool ReadNext ();

    /// <summary>Closes the FdoILockConflictReader object, freeing any resources it may
    /// be holding.</summary>
    /// <returns>Returns nothing.</returns> 
    void Close ();
};

#endif // ARCSDELOCKCONFLICTREADER_H

