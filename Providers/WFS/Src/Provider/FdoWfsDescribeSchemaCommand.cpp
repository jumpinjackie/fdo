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
#include "FdoWfsProvider.h"
#include "FdoWfsDelegate.h"

#include <malloc.h>
#include <string.h>

FdoWfsDescribeSchemaCommand::FdoWfsDescribeSchemaCommand (FdoIConnection *connection) :
    FdoCommonCommand<FdoIDescribeSchema, FdoWfsConnection> (connection)
{
    mClassNames = NULL;
}

/** Do not implement the copy constructor. **/
//FdoWfsDescribeSchemaCommand::FdoWfsDescribeSchemaCommand (const FdoWfsDescribeSchemaCommand &right) { }

FdoWfsDescribeSchemaCommand::~FdoWfsDescribeSchemaCommand (void)
{
}

/** Do not implement the assignment operator. **/
//FdoWfsDescribeSchemaCommand & operator= (const FdoWfsDescribeSchemaCommand &right);

/// <summary>Gets the name of the schema to describe. This function is optional;
/// if not specified, execution of the command will describe all schemas.</summary>
/// <returns>Returns the schema name</returns> 
const wchar_t* FdoWfsDescribeSchemaCommand::GetSchemaName ()
{
    return (mSchemaName);
}


/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void FdoWfsDescribeSchemaCommand::SetSchemaName (const wchar_t* value)
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
FdoStringCollection* FdoWfsDescribeSchemaCommand::GetClassNames()
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
void FdoWfsDescribeSchemaCommand::SetClassNames(FdoStringCollection* value)
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
FdoFeatureSchemaCollection* FdoWfsDescribeSchemaCommand::Execute ()
{
    FdoPtr<FdoFeatureSchemaCollection> ret = mConnection->GetSchemas();

	// The following lines check whether the specified schema exists in the 
	// returned schema collection. If not found, an exception will be thrown.
	// Note that since WFS provider now support only one "real" schema and it
	// must be the last one in the collection, here we can simply compare the
	// specified schema with the last one in the collection.
	if (this->mSchemaName.GetLength())
	{
		FdoInt32 cnt = ret->GetCount();
		if (cnt > 0)
		{
			if (mSchemaName != ret->GetItem(cnt - 1)->GetName())
				throw FdoException::Create(L"Schema can not be found in the collection.");
		}
	}

    return (FDO_SAFE_ADDREF(ret.p));
}
