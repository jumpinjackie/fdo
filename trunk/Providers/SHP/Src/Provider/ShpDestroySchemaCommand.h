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

#ifndef SHPDESTROYSCHEMACOMMAND_H
#define SHPDESTROYSCHEMACOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

class ShpDestroySchemaCommand :
    public FdoCommonCommand<FdoIDestroySchema, ShpConnection>
{
    friend class ShpConnection;

    FdoStringP mSchemaName;

private:
    //
    // Prevent the use of the copy constructor by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ShpDestroySchemaCommand (const ShpDestroySchemaCommand &right);

    // Constructs an instance of a DescribeSchema command using the given connection.
    ShpDestroySchemaCommand (FdoIConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~ShpDestroySchemaCommand (void);

public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ShpDestroySchemaCommand & operator= (const ShpDestroySchemaCommand &right);

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

#endif // SHPDESTROYSCHEMACOMMAND_H

