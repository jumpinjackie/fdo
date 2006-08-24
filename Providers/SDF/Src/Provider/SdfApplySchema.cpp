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
#include "SdfApplySchema.h"
#include "SdfConnection.h"


SdfApplySchema::SdfApplySchema(SdfConnection* connection) : SdfCommand<FdoIApplySchema>(connection)
{
    m_schema = NULL;
    m_bIgnoreStates = false;
}

SdfApplySchema::~SdfApplySchema()
{
    FDO_SAFE_RELEASE(m_schema);
}

FdoFeatureSchema* SdfApplySchema::GetFeatureSchema()
{
    FDO_SAFE_ADDREF(m_schema);

    return m_schema;
}

void SdfApplySchema::SetFeatureSchema(FdoFeatureSchema* value)
{
    //kill old schema, if any
    FDO_SAFE_RELEASE(m_schema);

    //set new schema
    FDO_SAFE_ADDREF(value);
    m_schema = value;
}

FdoPhysicalSchemaMapping* SdfApplySchema::GetPhysicalMapping()
{
    return NULL;
}

void SdfApplySchema::SetPhysicalMapping(FdoPhysicalSchemaMapping* value)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_6_PHYSICAL_SCHEMA_MAPPING_NOT_SUPPORTED)));
}

FdoBoolean SdfApplySchema::GetIgnoreStates()
{   
    return m_bIgnoreStates;
}

void SdfApplySchema::SetIgnoreStates( FdoBoolean ignoreStates )
{
    m_bIgnoreStates = ignoreStates;
}

void SdfApplySchema::Execute()
{
    // verify connection is open - this implicitly verifies the
    // connection string is set and is valid
    if (m_connection->GetConnectionState() != FdoConnectionState_Open)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_26_CONNECTION_CLOSED)));

    if (m_schema == NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_27_NULL_REFERENCE)));

    //note: no smart pointer needed for SdfConnection::GetSchema().
    if (m_connection->GetSchema() != NULL)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_52_SDF_HAS_SCHEMA), "Cannot apply schema. The SDF already has a schema."));

    //tell the connection to update its schema database
    m_connection->SetSchema(m_schema, m_bIgnoreStates);
}

