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
#include <Rdbms/Override/ODBC/OdbcOvClassDefinition.h>

FdoSmLpOdbcFeatureClass::FdoSmLpOdbcFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) :
    FdoSmLpFeatureClass(classReader, parent),
    FdoSmLpOdbcClassDefinition(classReader, parent),
    FdoSmLpClassBase(classReader, parent)
{
}

FdoSmLpOdbcFeatureClass::FdoSmLpOdbcFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) :
    FdoSmLpFeatureClass(pFdoClass, bIgnoreStates, parent),
    FdoSmLpOdbcClassDefinition(pFdoClass, bIgnoreStates, parent),
    FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
}

FdoSmLpOdbcFeatureClass::~FdoSmLpOdbcFeatureClass()
{
}

bool FdoSmLpOdbcFeatureClass::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bSetMapping = false;
    FdoOdbcOvClassDefinition* pOdbcMapping = 
        (FdoOdbcOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;

    // Add the Generic RDBMS physical mappings
    bSetMapping = (FdoSmLpOdbcClassDefinition::SetSchemaMappings( classMapping, bIncludeDefaults ) || bSetMapping);

    return bSetMapping;
}

void FdoSmLpOdbcFeatureClass::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    // Apply the schema overrides first since these are needed to update properties.
    FdoSmLpOdbcClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Do Generic feature class updates next
    FdoSmLpFeatureClass::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );
}

FdoSmPhClassWriterP FdoSmLpOdbcFeatureClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP writer = FdoSmLpFeatureClass::GetPhysicalAddWriter();
    FdoSmLpOdbcClassDefinition::SetPhysicalAddWriter( writer );

    return writer;
}

void FdoSmLpOdbcFeatureClass::PostFinalize()
{
    FdoSmLpFeatureClass::PostFinalize();
    FdoSmLpOdbcClassDefinition::PostFinalize();
}
