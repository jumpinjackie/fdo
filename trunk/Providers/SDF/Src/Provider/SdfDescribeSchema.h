// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "SdfCommand.h"

class SdfDescribeSchema : public SdfCommand<FdoIDescribeSchema>
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:
    // constructs a DescribeSchema command using the specified connection
    SDF_API SdfDescribeSchema(SdfConnection* connection);

protected:
    // default destructor
    SDF_API virtual ~SdfDescribeSchema();

    //-------------------------------------------------------
    // FdoIDescribeSchema implementation
    //-------------------------------------------------------

public:
    // Gets the name of the schema to describe.  This is optional,
    // if not specified execution of the command will describe all schemas.
    SDF_API virtual FdoString* GetSchemaName();

    // Sets the name of the schema to describe.  This is optional, if not
    // specified execution of the command will describe all schemas.
    SDF_API virtual void SetSchemaName(FdoString* value);

    // Executes the describe schema command and returns a FeatureSchemaCollection.
    // If a schema name is given that has references to another schema, the dependent
    // schemas will be returned as well.  If the specified schema name does not exist
    // the Execute method will throw an exception.
    SDF_API virtual FdoFeatureSchemaCollection* Execute();

    //-------------------------------------------------------
    // Variables
    //-------------------------------------------------------

private:
    wchar_t* m_schemaName;
};

