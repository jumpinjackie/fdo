#ifndef FDORDBMSAPPLYSCHEMA_H
#define FDORDBMSAPPLYSCHEMA_H       1
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


#include <Inc/Util/string.h>
#include <FdoRdbmsCommand.h>
#include <FdoRdbmsConnection.h>


// The ApplySchema command creates or updates a feature schema
// within the current datababase. Input to the apply schema command
// is the feature schema instance to be created or updated.
class FdoRdbmsApplySchemaCommand : public FdoRdbmsCommand<FdoIApplySchema>
{
    friend class FdoRdbmsConnection;

private:
    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsApplySchemaCommand(const FdoRdbmsApplySchemaCommand &right);

    // Constructs a default instance of a ApplySchema command.
    FdoRdbmsApplySchemaCommand();

    // Constructs an instance of an ApplySchema command using
    // the specified arguments.
    FdoRdbmsApplySchemaCommand(FdoIConnection* connection);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~FdoRdbmsApplySchemaCommand();

public:
    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsApplySchemaCommand & operator=(const FdoRdbmsApplySchemaCommand &right);

    // Gets the Feature Schema to apply.
    virtual FdoFeatureSchema* GetFeatureSchema();

    // Sets the Feature Schema to apply.
    virtual void SetFeatureSchema(FdoFeatureSchema* value);

    // Gets the PhysicalSchemaMapping used to specify how the schema definition
    // gets mapped into physical storage.
    virtual FdoPhysicalSchemaMapping* GetPhysicalMapping();

    // Sets the PhysicalSchemaMapping used to specify how the schema definition
    // gets mapped into physical storage.
    virtual void SetPhysicalMapping(FdoPhysicalSchemaMapping* value);

    // Indicates whether Execute() will ignore element states
    virtual FdoBoolean GetIgnoreStates();

    // Changes the handling of element states by Execute()
    // Parameters:
    //  ignoreStates
    //      True: Execute() will ignore the
    //      element states on the Feature Schema elements. Instead, it
    //      will additively merge the Feature Schema into the current DataStore.
    //      False: Execute() will use the element states to determine whether
    //      each element is inserted, modified or deleted.</param>
    virtual void SetIgnoreStates( FdoBoolean ignoreStates );

    // Executes the apply schema command which updates meta-data and physical
    // storage for the schema. An exception is thrown if there are any errors
    // when applying the schema. If schema changes include deletion of classes
    // or the schema itself, then those classes (or the schema if it is marked
    // as deleted) must not contain any instance data. If they do, an exception
    // will be thrown.
    virtual void Execute();


protected:

    FdoPtr<FdoRdbmsConnection>      mRdbmsConnection;
    FdoFeatureSchemaP               mpSchema;
    FdoPhysicalSchemaMappingP       mpSchemaMappings;
    FdoBoolean                      mIgnoreStates;
};


#endif  // FDORDBMSDESCRIBESCHEMA_H
