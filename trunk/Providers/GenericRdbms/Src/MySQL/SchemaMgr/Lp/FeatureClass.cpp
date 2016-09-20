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
#include "FeatureClass.h"
#include "GeometricPropertyDefinition.h"
//TODO: #include <Rdbms/Override/MySql/MySqlOvClassDefinition.h>


FdoSmLpMySqlFeatureClass::FdoSmLpMySqlFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) :
    FdoSmLpFeatureClass(classReader, parent),
    FdoSmLpMySqlClassDefinition(classReader, parent),
    FdoSmLpClassBase(classReader, parent)
{
}

FdoSmLpMySqlFeatureClass::FdoSmLpMySqlFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) :
    FdoSmLpFeatureClass(pFdoClass, bIgnoreStates, parent),
    FdoSmLpMySqlClassDefinition(pFdoClass, bIgnoreStates, parent),
    FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
}

FdoSmLpMySqlFeatureClass::~FdoSmLpMySqlFeatureClass()
{
}

void FdoSmLpMySqlFeatureClass::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    // Apply the schema overrides first  since these are needed to update properties
    FdoSmLpMySqlClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Do Generic feature class updates next
    FdoSmLpFeatureClass::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );
}


bool FdoSmLpMySqlFeatureClass::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bSetMapping = false;

    FdoMySQLOvClassDefinition* pMySqlMapping = 
        (FdoMySQLOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;

    // Add the Generic RDBMS physical mappings
    bSetMapping = (FdoSmLpMySqlClassDefinition::SetSchemaMappings( classMapping, bIncludeDefaults ) || bSetMapping);

    return bSetMapping;
}

FdoSmPhClassWriterP FdoSmLpMySqlFeatureClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP writer = FdoSmLpFeatureClass::GetPhysicalAddWriter();
    FdoSmLpMySqlClassDefinition::SetPhysicalAddWriter( writer );

    return writer;
}
