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

#ifndef ArcSDERollbackLongTransactionCommand_H
#define ArcSDERollbackLongTransactionCommand_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDERollbackLongTransactionCommand :
    public ArcSDEFeatureCommand<FdoIRollbackLongTransaction>
{
    // name of the long transaction
    FdoStringP mName;

protected:
    void DeleteLongTransaction (SE_CONNECTION conn, CHAR* name);

public:
    ArcSDERollbackLongTransactionCommand (FdoIConnection *connection);
    virtual ~ArcSDERollbackLongTransactionCommand (void);

    /// <summary>Gets the name of the long transaction to roll back as a string.</summary>
    /// <returns>Returns the name of the long transaction.</returns> 
    FdoString* GetName ();

    /// <summary>Sets the name of the long transaction to roll back as a string.</summary>
    /// <param name="value">Input the name of the long transaction.</param> 
    /// <returns>Returns nothing</returns> 
    void SetName (FdoString* value);

    /// <summary>Gets a reference to the lock conflict reader.</summary>
    /// <returns>Returns a reference to the lock conflict reader</returns> 
    FdoILockConflictReader* GetLockConflictReader();

    /// <summary>Executes the RollbackLongTransaction command.</summary>
    /// <returns>Returns nothing</returns> 
    void Execute ();
};

#endif // ArcSDERollbackLongTransactionCommand_H

