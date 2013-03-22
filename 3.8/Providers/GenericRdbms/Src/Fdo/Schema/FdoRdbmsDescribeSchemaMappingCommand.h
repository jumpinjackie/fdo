#ifndef FDORDBMSDESCRIBESCHEMAMAPPING_H
#define FDORDBMSDESCRIBESCHEMAMAPPING_H     1
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

// The FdoRdbmsDescribeSchemaMappingCommand describes the logical
// to physical schema mappings for feature schemas
// available from the connection. The DescribeSchemaMapping
// command can describe the mappings for a single schema or all schemas available from
// the connection. The Execute operation returns an FdoPhysicalSchemaMappingCollection
// object.
class FdoRdbmsDescribeSchemaMappingCommand : public FdoRdbmsCommand<FdoIDescribeSchemaMapping>
{
    friend class FdoRdbmsConnection;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaMappingCommand(const FdoRdbmsDescribeSchemaMappingCommand &right);

    // Constructs a default instance of a DescribeSchema command.
    FdoRdbmsDescribeSchemaMappingCommand();

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaMappingCommand(FdoIConnection* connection);

    // Constructs an instance of a DescribeSchema command using
    // the specified arguments.
    FdoRdbmsDescribeSchemaMappingCommand(DbiConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoRdbmsDescribeSchemaMappingCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsDescribeSchemaMappingCommand & operator=(const FdoRdbmsDescribeSchemaMappingCommand &right);

    //Gets the name of the schema for which to describe
    // schema mappings.
    virtual FdoString* GetSchemaName();

    // Sets the name of the schema to describe. This function is optional; if not
    // specified execution of the command will describe the mappings
    // all schemas.
    virtual void SetSchemaName(FdoString* value);

    // Gets the current "include default mappings" setting.
    virtual FdoBoolean GetIncludeDefaults();

    // Sets the "include default mappings" setting.
    //
    // Parameters:
    //  includeDefaults
    //      True: Execute() will return
    //      all mappings for the feature schema(s).
    //      False: Execute() will not include default logical to physical
    //      mappings, only those mappings that have been overridden will
    //      be returned.
    virtual void SetIncludeDefaults( FdoBoolean includeDefaults );

    // Executes the DescribeSchemaMapping command and returns a
    // FdoPhysicalSchemaMappingCollection. If the specified schema name does not exist,
    // the Execute method throws an exception.
    virtual FdoPhysicalSchemaMappingCollection* Execute();

protected:
    DbiConnection*                     mRdbmsConnection;
    FdoStringP                         mSchemaName;
    FdoBoolean                         mIncludeDefaults;

};

#endif  // FDORDBMSDESCRIBESCHEMAMAPPING_H
