// 
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
//  
#include "stdafx.h"
#include "SdfDescribeSchema.h"
#include "SdfConnection.h"
#include "SchemaDb.h"
#include "FdoCommonSchemaUtil.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs a DescribeSchema command using the specified connection
SdfDescribeSchema::SdfDescribeSchema(SdfConnection* connection)
: SdfCommand<FdoIDescribeSchema>(connection)
{
    m_schemaName = NULL;
}


// default destructor
SdfDescribeSchema::~SdfDescribeSchema()
{
    if (m_schemaName != NULL)
    {
        // use free() since we allocated with malloc via wcsdup()
        free(m_schemaName);
        m_schemaName = NULL;
    }
}

//-------------------------------------------------------
// FdoIDescribeSchema implementation
//-------------------------------------------------------


// Gets the name of the schema to describe.  This is optional,
// if not specified execution of the command will describe all schemas.
FdoString* SdfDescribeSchema::GetSchemaName()
{
    return m_schemaName;
}


// Sets the name of the schema to describe.  This is optional, if not
// specified execution of the command will describe all schemas.
void SdfDescribeSchema::SetSchemaName(FdoString* value)
{
    if (m_schemaName != NULL)
    {
        // use free() since we allocated with malloc via wcsdup()
        free(m_schemaName);
        m_schemaName = NULL;
    }

    if (value != NULL)
#ifdef _WIN32
        m_schemaName = _wcsdup(value);
#else
        m_schemaName = wcsdup(value);
#endif
}


// Executes the describe schema command and returns a FeatureSchemaCollection.
// If a schema name is given that has references to another schema, the dependent
// schemas will be returned as well.  If the specified schema name does not exist
// the Execute method will throw an exception.
FdoFeatureSchemaCollection* SdfDescribeSchema::Execute()
{
    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    // create the collection to return
    FdoPtr<FdoFeatureSchemaCollection> schemaCollection = FdoFeatureSchemaCollection::Create(NULL);

    // Re-read a fresh copy of the schema from the SDF file; this is significant
    // if the caller has changed the spatial contexts in the file since doing ApplySchema:
    FdoFeatureSchema *schema = m_connection->GetSchema(GetSchemaName(), true);
    if (schema)
    {
        schemaCollection->Add(schema);
        schema->AcceptChanges();
    }

    return schemaCollection.Detach();
}
