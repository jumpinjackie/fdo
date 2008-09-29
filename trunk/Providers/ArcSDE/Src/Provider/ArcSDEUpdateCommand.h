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

#ifndef ArcSDEUpdateCommand_H
#define ArcSDEUpdateCommand_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "ArcSDEFeatureCommand.h"


class ArcSDEUpdateCommand :
    public ArcSDEFeatureCommand<FdoIUpdate>
{
    friend class ArcSDEConnection;

protected:
    // we need to hang on to the conflicts since GetLockConflicts() applies to the last execute
    // any (external) operation other than GetLockConflicts() invalidates this
    FdoPtr<ArcSDELockConflictReader> mConflictReader;

private:
    ArcSDEUpdateCommand (FdoIConnection *connection);
    virtual ~ArcSDEUpdateCommand (void);

protected:
    void reset ();

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEUpdateCommand & operator= (const ArcSDEUpdateCommand &right);

    //
    // FdoIUpdate interface
    //

    /// <summary>Gets the FdoPropertyValueCollection that specifies the names and values of the 
    /// properties to be updated.</summary>
    /// <returns>Returns the list of properties and their values.</returns> 
    virtual FdoPropertyValueCollection* GetPropertyValues ();

    /// <summary>Executes the update command and returns the number of modified 
    /// instances.</summary>
    /// <returns>Returns nothing</returns> 
    virtual FdoInt32 Execute ();

    /// <summary> Updating objects might result in lock conflicts if objects
    /// to be updated are not exclusively locked for the user attempting to
    /// update the object. If objects to be updated are not exclusively locked for the 
    /// user attempting to update the object, a lock conflict report is generated.
    /// The function GetLockConflicts returns a lock conflict reader providing
    /// access to the list of lock conflicts that occurred during the execution
    /// of the update operation.</summary>
    /// <returns>Returns a lock conflict reader.</returns> 
    virtual FdoILockConflictReader* GetLockConflicts ();
};

#endif // ArcSDEUpdateCommand_H

