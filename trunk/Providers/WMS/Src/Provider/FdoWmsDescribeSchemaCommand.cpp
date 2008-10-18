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

#include "stdafx.h"

#include <malloc.h>
#include <string.h>

#include "FdoWmsDescribeSchemaCommand.h"
#include "FdoWmsUtils.h"
#include "FdoWmsGlobals.h"

FdoWmsDescribeSchemaCommand::FdoWmsDescribeSchemaCommand (FdoIConnection *connection) :
    FdoWmsCommand<FdoIDescribeSchema> (connection)
{
    mClassNames = NULL;
}

FdoWmsDescribeSchemaCommand::~FdoWmsDescribeSchemaCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void FdoWmsDescribeSchemaCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the name of the schema to describe. This function is optional;
/// if not specified, execution of the command will describe all schemas.</summary>
/// <returns>Returns the schema name</returns> 
const wchar_t* FdoWmsDescribeSchemaCommand::GetSchemaName ()
{
    return (mSchemaName);
}


/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void FdoWmsDescribeSchemaCommand::SetSchemaName (const wchar_t* value)
{
    mSchemaName = value;
}

/// <summary>Gets the names of the classes to retrieve. This is optional,
/// if not specified execution of the command will describe all classes.
/// If the class name is not qualified, and the schema name is not specified,
/// the requested class from all schemas will be described.
/// The class names specified serve only as a hint.  Use of the hint
/// during command execution is provider dependent.  Providers that 
/// will not use the hint will describe the schema for all classes.</summary>
/// <returns>Returns the collection of class names</returns>
FdoStringCollection* FdoWmsDescribeSchemaCommand::GetClassNames()
{
    return mClassNames;
}

/// <summary>Sets the name of the classes to retrieve. This is optional, if not
/// specified execution of the command will describe all classes.
/// If the class name is not qualified, and the schema name is not specified,
/// the requested class from all schemas will be described.
/// The class names specified serve only as a hint.  Use of the hint
/// during command execution is provider dependent.  Providers that 
/// will not use the hint will describe the schema for all classes.</summary>
/// <param name="value">Input the collection of class names</parm>
/// <returns>Returns nothing</returns>
void FdoWmsDescribeSchemaCommand::SetClassNames(FdoStringCollection* value)
{
    // Do nothing.
    // This method is not implemented.  DescribeSchema command
    // will describe all classes.
}

/// <summary>Executes the DescribeSchema command and returns a 
/// FdoFeatureSchemaCollection. If a schema name is given that has 
/// references to another schema, the dependent schemas will 
/// be returned as well. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema collection representing the schema created.</returns> 
FdoFeatureSchemaCollection* FdoWmsDescribeSchemaCommand::Execute ()
{
	FdoFeatureSchemasP schemas = mConnection->GetSchemas ();
	if (mSchemaName.GetLength() > 0)
	{
		// Since the WMS provider doesn't support multiple schemas, we need only
		// to compare the name with the first schema in the collection.
		FdoFeatureSchemaP schema = schemas->GetItem (0);
		if (wcscmp (mSchemaName, schema->GetName ()) != 0)
		{
		    throw FdoSchemaException::Create (NlsMsgGet(FDOWMS_NAMED_SCHEMA_NOT_FOUND, "FDO Schema '%1$ls' was not found.", (FdoString*)mSchemaName ));
		}
	}

	return FDO_SAFE_ADDREF (schemas.p);
}
