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
#include "SdfSchemaMergeContext.h"
#include "SdfConnection.h"

SdfSchemaMergeContext::SdfSchemaMergeContext( 
    SdfConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
) :
    FdoSchemaMergeContext( oldSchemas )
{
    SetConnection( connection );
    SetUpdSchema( newSchema );
    SetIgnoreStates( ignoreStates );
}

SdfSchemaMergeContext::~SdfSchemaMergeContext(void)
{
}

SdfSchemaMergeContext* SdfSchemaMergeContext::Create( 
    SdfConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
)
{
    return new SdfSchemaMergeContext( connection, oldSchemas, newSchema, ignoreStates );
}

bool SdfSchemaMergeContext::CanAddClass( FdoClassDefinition* classDef )
{
    return true;
}

bool SdfSchemaMergeContext::CanDeleteClass( FdoClassDefinition* classDef )
{
    return false;
}

void SdfSchemaMergeContext::Merge()
{
    CommitSchemas();
}
