/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>

FdoSmLpSqsFeatureClass::FdoSmLpSqsFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) :
    FdoSmLpFeatureClass(classReader, parent),
    FdoSmLpSqsClassDefinition(classReader, parent),
    FdoSmLpClassBase(classReader, parent)
{
}

FdoSmLpSqsFeatureClass::FdoSmLpSqsFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) :
    FdoSmLpFeatureClass(pFdoClass, bIgnoreStates, parent),
    FdoSmLpSqsClassDefinition(pFdoClass, bIgnoreStates, parent),
    FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
}

FdoSmLpSqsFeatureClass::~FdoSmLpSqsFeatureClass()
{
}

bool FdoSmLpSqsFeatureClass::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bSetMapping = false;
    FdoSqlServerOvClassDefinition* pSqsMapping = 
        (FdoSqlServerOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;

    // Add the Generic RDBMS physical mappings
    bSetMapping = (FdoSmLpSqsClassDefinition::SetSchemaMappings( classMapping, bIncludeDefaults ) || bSetMapping);

    return bSetMapping;
}

void FdoSmLpSqsFeatureClass::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    // Apply the schema overrides first since these are needed to update properties.
    FdoSmLpSqsClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Do Generic feature class updates next
    FdoSmLpFeatureClass::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );
}

FdoSmPhClassWriterP FdoSmLpSqsFeatureClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP writer = FdoSmLpFeatureClass::GetPhysicalAddWriter();
    FdoSmLpSqsClassDefinition::SetPhysicalAddWriter( writer );

    return writer;
}

void FdoSmLpSqsFeatureClass::PostFinalize()
{
    FdoSmLpFeatureClass::PostFinalize();
    FdoSmLpSqsClassDefinition::PostFinalize();
}
