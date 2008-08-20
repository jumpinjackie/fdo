#ifndef FDORDBMSDESCRIBESCHEMA_H
#define FDORDBMSDESCRIBESCHEMA_H        1
#ifdef _WIN32
#pragma once
#endif
 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
 // 
 // This library is free software; you can redistribute it and/or
 // modify it under the terms of version 2.1 of the GNU Lesser
 // General Public License as published by the Free Software Foundation.
 // 
 // This library is distributed in the hope that it will be useful,
 // but WITHOUT ANY WARRANTY; without even the implied warranty of
 // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 // Lesser General Public License for more details.
 // 
 // You should have received a copy of the GNU Lesser General Public
 // License along with this library; if not, write to the Free Software
 // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <map>
#include "Inc/Util/string.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsConnection.h"

// The DescribeSchema command describes the feature schemas available
// from the connection. The describe schema command can describe a
// single schema or all schemas available from the connection. The
// Execute operation returns a FeatureSchemaCollection object which
// is described in the FDO Feature Schema specification.
class FdoRdbmsDescribeSchemaCommand : public FdoRdbmsCommand<FdoIDescribeSchema>
{
    friend class FdoRdbmsConnection;
    friend class FdoRdbmsFeatureReader;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaCommand(const FdoRdbmsDescribeSchemaCommand &right);

    // Constructs a default instance of a DescribeSchema command.
    FdoRdbmsDescribeSchemaCommand();

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaCommand(FdoIConnection* connection);

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaCommand(DbiConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoRdbmsDescribeSchemaCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaCommand & operator=(const FdoRdbmsDescribeSchemaCommand &right);

    // Gets the name of the schema to describe. This is optional,
    // if not specified execution of the command will describe all schemas.
    virtual const wchar_t* GetSchemaName();

    // Sets the name of the schema to describe. This is optional, if not
    // specified execution of the command will describe all schemas.
    virtual void SetSchemaName(const wchar_t* value);

    // Gets the names of the classes to retrieve. This is optional,
    // if not specified execution of the command will describe all classes.
    // If the class name is not qualified, and the schema name is not specified,
    // the requested class from all schemas will be described.
    // The class names specified serve only as a hint.  Use of the hint
    // during command execution is provider dependent.  Providers that 
    // will not use the hint will describe the schema for all classes.
    virtual FdoStringCollection* GetClassNames();

    // Sets the name of the classes to retrieve. This is optional, if not
    // specified execution of the command will describe all classes.
    // If the class name is not qualified, and the schema name is not specified,
    // the requested class from all schemas will be described.
    // The class names specified serve only as a hint.  Use of the hint
    // during command execution is provider dependent.  Providers that 
    // will not use the hint will describe the schema for all classes.
    virtual void SetClassNames(FdoStringCollection* value);

    // Executes the describe schema command and returns a
    // FeatureSchemaCollection. If a schema name is given that has
    // references to another schema, the dependent schemas will
    // be returned as well. If the specified schema name does not exist
    // the Execute method will throw an exception.
    virtual FdoFeatureSchemaCollection* Execute();

protected:
    DbiConnection*                        mRdbmsConnection;
    FdoStringP                            mSchemaName;
    FdoStringsP                           mClassNames;

	FdoPtr<FdoIConnection>			mFdoConnection;
};

#endif  // FDORDBMSDESCRIBESCHEMA_H
