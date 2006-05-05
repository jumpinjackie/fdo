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

#ifndef ArcSDEDestroySchemaCommand_H
#define ArcSDEDestroySchemaCommand_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDEDestroySchemaCommand :
    public ArcSDECommand<FdoIDestroySchema>
{
    friend class ArcSDEConnection;

protected:
    FdoIdentifier* mSchemaName;

private:
    ArcSDEDestroySchemaCommand (FdoIConnection *connection);
    virtual ~ArcSDEDestroySchemaCommand (void);

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    ArcSDEDestroySchemaCommand & operator= (const ArcSDEDestroySchemaCommand &right);

    //
    // FdoIDestroySchema interface
    //

    ///<summary>Gets the name of the schema to destroy as a string.</summary>
    /// <returns>Returns the name of the schema to destroy</returns> 
    virtual FdoString* GetSchemaName ();

    ///<summary>Sets the name of the schema to destroy as a string.</summary>
    /// <param name="value">Input the name of the schema to destroy</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetSchemaName (FdoString* value);

    ///<summary>Executes the destroy schema command, which removes the schema, class
    ///definitions, relation definitions, and all instance data from the DataStore.
    ///If elements in other schemas refer to the schema to be destroyed
    ///an exception is thrown.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Execute ();
};

#endif // ArcSDEDestroySchemaCommand_H

