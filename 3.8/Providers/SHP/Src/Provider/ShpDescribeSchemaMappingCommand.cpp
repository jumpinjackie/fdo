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


ShpDescribeSchemaMappingCommand::ShpDescribeSchemaMappingCommand (FdoIConnection *connection) :
    FdoCommonCommand<FdoIDescribeSchemaMapping, ShpConnection> (connection),
    mIncludeDefaults(true)
{
}

/** Do not implement the copy constructor. **/
//ShpDescribeSchemaMappingCommand::ShpDescribeSchemaMappingCommand (const ShpDescribeSchemaMappingCommand &right) { }

ShpDescribeSchemaMappingCommand::~ShpDescribeSchemaMappingCommand (void)
{
}

/** Do not implement the assignment operator. **/
//ShpDescribeSchemaMappingCommand & operator= (const ShpDescribeSchemaMappingCommand &right);


/// <summary>Gets the name of the schema for which to describe
/// schema mappings.</summary>
/// <returns>Returns the schema name</returns> 
FdoString* ShpDescribeSchemaMappingCommand::GetSchemaName()
{
    return (mSchemaName);
}

/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe the mappings for
/// all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void ShpDescribeSchemaMappingCommand::SetSchemaName(FdoString* value)
{
    mSchemaName = value;
}

/// <summary>Gets the current "include default mappings" setting.</summary>
/// <returns>Returns True if Execute() will include default mappings.</returns> 
FdoBoolean ShpDescribeSchemaMappingCommand::GetIncludeDefaults()
{
    return mIncludeDefaults;
}

/// <summary>Sets the "include default mappings" setting.</summary>
/// <param name="includeDefaults">Input True: Execute() will return
/// all mappings for the feature schema(s).
/// False: Execute() will not include default logical to physical
/// mappings, only those mappings that have been overridden will 
/// be returned.</param> 
/// <returns>Returns nothing</returns> 
void ShpDescribeSchemaMappingCommand::SetIncludeDefaults( FdoBoolean includeDefaults )
{
    mIncludeDefaults = includeDefaults;
}

/// <summary>Executes the DescribeSchemaMapping command and returns a 
/// FdoPhysicalSchemaMappingCollection. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema mapping collection for the requested feature schemas.</returns> 
FdoPhysicalSchemaMappingCollection* ShpDescribeSchemaMappingCommand::Execute()
{
    FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = FdoPhysicalSchemaMappingCollection::Create();

    // Get the LogicalPhysical schema collection:
    FdoPtr<ShpConnection> shpConn = (ShpConnection*)GetConnection ();
    FdoPtr<ShpLpFeatureSchemaCollection> lpSchemas = shpConn->GetLpSchemas();
    VALIDATE_POINTER(lpSchemas);

    // Get all schema mappings requested by caller:
    for (FdoInt32 i=0; i<lpSchemas->GetCount(); i++)
    {
        FdoPtr<ShpLpFeatureSchema> lpSchema = lpSchemas->GetItem(i);

        if ((mSchemaName==NULL) || (0==wcslen(mSchemaName)) || (0==wcscmp(mSchemaName,lpSchema->GetName())))
        {
            FdoPtr<FdoShpOvPhysicalSchemaMapping> schemaMapping = lpSchema->GetSchemaMappings(mIncludeDefaults);
            if (schemaMapping != NULL)
                schemaMappings->Add(schemaMapping);
        }
    }

    return FDO_SAFE_ADDREF(schemaMappings.p);
}

