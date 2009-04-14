#ifndef FDORDBMSDESTROYSCHEMA_H
#define FDORDBMSDESTROYSCHEMA_H     1
#ifdef _WIN32
#pragma once
#endif
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


#include <FdoRdbmsCommand.h>
#include <FdoRdbmsConnection.h>


// The DestroySchema command deletes a feature schema from the current database.
class FdoRdbmsDestroySchemaCommand : public FdoRdbmsCommand<FdoIDestroySchema>
{
    friend class FdoRdbmsConnection;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDestroySchemaCommand(const FdoRdbmsDestroySchemaCommand &right);

    // Constructs a default instance of a DestroySchema command.
    FdoRdbmsDestroySchemaCommand();

    // Constructs an instance of an DestroySchema command using
    // the specified arguments.
    FdoRdbmsDestroySchemaCommand(FdoIConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoRdbmsDestroySchemaCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDestroySchemaCommand & operator=(const FdoRdbmsDestroySchemaCommand &right);

    // Gets the name of the schema to destroy.
    virtual FdoString* GetSchemaName();

    // Sets the name of the schema to destroy.
    virtual void SetSchemaName(FdoString* value);

    // Executes the destroy schema command. An exception is thrown if there are any
    // errors when destroying the schema. A typical error condition is when other
    // schemas references classes in the schema to destroy.
    virtual void Execute();


protected:

    FdoPtr<FdoRdbmsConnection>          mRdbmsConnection;
    FdoStringP                          mSchemaName;
};


#endif  // FDORDBMSDESTROYESCHEMA_H
