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

#ifndef SHPDELETECOMMAND_H
#define SHPDELETECOMMAND_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class ShpDeleteCommand :
    public FdoCommonFeatureCommand<FdoIDelete, ShpConnection>
{
    friend class ShpConnection;

private:
    ShpDeleteCommand (FdoIConnection *connection);
    virtual ~ShpDeleteCommand (void);

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ShpDeleteCommand & operator= (const ShpDeleteCommand &right);

    //
    // FdoIDelete interface
    //

    /// <summary>Executes the delete command and returns the number of instances
    /// deleted</summary>
    /// <returns>Returns the number of instances deleted.</returns> 
    virtual FdoInt32 Execute ();

    /// <summary> Deleting objects might result in lock conflicts if objects
    /// to be deleted are not exclusively locked for the user attempting to
    /// delete the object. A lock conflict report is generated.
    /// The function GetLockConflicts returns a lock conflict reader that provides
    /// access to the list of lock conflicts that occurred during the execution
    /// of the delete operation.</summary>
    /// <returns>Returns a lock conflict reader.</returns> 
    virtual FdoILockConflictReader* GetLockConflicts ();
};

#endif // SHPDELETECOMMAND_H

