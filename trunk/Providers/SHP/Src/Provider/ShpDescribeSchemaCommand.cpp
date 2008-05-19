/*
 * 
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

#include <ShapeFile.h>
#include <ColumnInfo.h>

ShpDescribeSchemaCommand::ShpDescribeSchemaCommand (FdoIConnection *connection) :
    FdoCommonCommand<FdoIDescribeSchema, ShpConnection> (connection)
{
}

/** Do not implement the copy constructor. **/
//ShpDescribeSchemaCommand::ShpDescribeSchemaCommand (const ShpDescribeSchemaCommand &right) { }

ShpDescribeSchemaCommand::~ShpDescribeSchemaCommand (void)
{
}

/** Do not implement the assignment operator. **/
//ShpDescribeSchemaCommand & operator= (const ShpDescribeSchemaCommand &right);

/// <summary>Gets the name of the schema to describe. This function is optional;
/// if not specified, execution of the command will describe all schemas.</summary>
/// <returns>Returns the schema name</returns> 
const wchar_t* ShpDescribeSchemaCommand::GetSchemaName ()
{
    return (mSchemaName);
}


/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void ShpDescribeSchemaCommand::SetSchemaName (const wchar_t* value)
{
    mSchemaName = value;
}


/// <summary>Executes the DescribeSchema command and returns a 
/// FdoFeatureSchemaCollection. If a schema name is given that has 
/// references to another schema, the dependent schemas will 
/// be returned as well. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema collection representing the schema created.</returns> 
FdoFeatureSchemaCollection* ShpDescribeSchemaCommand::Execute ()
{
    FdoPtr<FdoFeatureSchemaCollection> ret;

    // Get the Logical schema collection:
    FdoPtr<ShpConnection> shpConn = (ShpConnection*)GetConnection ();
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = shpConn->GetLpSchemas();
    if (lpSchemas != NULL)
    {
        FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = lpSchemas->GetLogicalSchemas();
        VALIDATE_POINTER(logicalSchemas);

        // handle missing schema better than the FdoCommonSchemaUtil message of FDO_4_UNREADY
        if (0 != mSchemaName.GetLength ())
        {
            FdoPtr<FdoFeatureSchema> schema = logicalSchemas->FindItem (mSchemaName);
            if (schema == NULL)
                throw FdoException::Create(NlsMsgGet(SHP_SCHEMA_NOT_FOUND, "Schema '%1$ls' not found.", (FdoString*)mSchemaName));
        }

        // Make a copy of the requested schema (or all schemas if no schemaname is provided), and return it:
        ret = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(logicalSchemas, mSchemaName);
        VALIDATE_POINTER(ret);
    }
    else
        if (0 != mSchemaName.GetLength ())
            throw FdoException::Create(NlsMsgGet(SHP_SCHEMA_NOT_FOUND, "Schema '%1$ls' not found.", (FdoString*)mSchemaName));
        else
            ret = FdoFeatureSchemaCollection::Create (NULL);

    return FDO_SAFE_ADDREF(ret.p);
}

