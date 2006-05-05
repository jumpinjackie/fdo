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
 *
 */

#ifndef FDOWFSDESCRIBESCHEMACOMMAND_H
#define FDOWFSDESCRIBESCHEMACOMMAND_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class FdoWfsDescribeSchemaCommand :
    public FdoCommonCommand<FdoIDescribeSchema, FdoWfsConnection>
{
    friend class FdoWfsConnection;

    FdoStringP mSchemaName;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWfsDescribeSchemaCommand (const FdoWfsDescribeSchemaCommand &right);

    // Constructs an instance of a DescribeSchema command using the given connection.
    FdoWfsDescribeSchemaCommand (FdoIConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoWfsDescribeSchemaCommand (void);

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoWfsDescribeSchemaCommand & operator= (const FdoWfsDescribeSchemaCommand &right);

    /// <summary>Gets the name of the schema to describe. This function is optional;
    /// if not specified, execution of the command will describe all schemas.</summary>
    /// <returns>Returns the schema name</returns> 
    virtual const wchar_t* GetSchemaName ();

    /// <summary>Sets the name of the schema to describe. This function is optional; if not
    /// specified, execution of the command will describe all schemas.</summary>
    /// <param name="value">Input the schema name</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetSchemaName (const wchar_t* value);

    /// <summary>Executes the DescribeSchema command and returns a 
    /// FdoFeatureSchemaCollection. If a schema name is given that has 
    /// references to another schema, the dependent schemas will 
    /// be returned as well. If the specified schema name does not exist,
    /// the Execute method throws an exception.</summary>
    /// <returns>Returns the schema collection representing the schema created.</returns> 
    virtual FdoFeatureSchemaCollection* Execute ();
};

#endif // FDOWFSDESCRIBESCHEMACOMMAND_H
