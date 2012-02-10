//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "FeatureClass.h"
#include "GeometricPropertyDefinition.h"

FdoSmLpPostGisFeatureClass::FdoSmLpPostGisFeatureClass(
    FdoSmPhClassReaderP classReader,
    FdoSmLpSchemaElement* parent)
    : FdoSmLpFeatureClass(classReader, parent),
      FdoSmLpPostGisClassDefinition(classReader, parent),
      FdoSmLpClassBase(classReader, parent)
{
    // idle
}

FdoSmLpPostGisFeatureClass::FdoSmLpPostGisFeatureClass(
    FdoFeatureClass* fdoClass,
    bool ignoreStates,
    FdoSmLpSchemaElement* parent)
    : FdoSmLpFeatureClass(fdoClass, ignoreStates, parent),
      FdoSmLpPostGisClassDefinition(fdoClass, ignoreStates, parent),
      FdoSmLpClassBase(fdoClass, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisFeatureClass::~FdoSmLpPostGisFeatureClass()
{
    // idle
}

void FdoSmLpPostGisFeatureClass::Update(FdoClassDefinition* fdoClass,
    FdoSchemaElementState state,
    FdoPhysicalClassMapping* classOverrides,
    bool ignoreStates)
{
    // Apply the schema overrides first  since these are
    // needed to update properties
    FdoSmLpPostGisClassDefinition::Update(fdoClass, state,
        classOverrides, ignoreStates);

    // Do Generic feature class updates next
    FdoSmLpFeatureClass::Update(fdoClass, state, classOverrides, ignoreStates);
}


bool FdoSmLpPostGisFeatureClass::SetSchemaMappings(
    FdoPhysicalClassMappingP classMapping,
    bool includeDefaults) const
{
    bool setMapping = false;

    FdoPostGISOvClassDefinition* pgMapping = NULL;
    pgMapping = static_cast<FdoPostGISOvClassDefinition*>(classMapping.p);

    // Add the Generic RDBMS physical mappings
    setMapping = (FdoSmLpPostGisClassDefinition::SetSchemaMappings(
                  classMapping, includeDefaults) || setMapping);

    return setMapping;
}

FdoSmPhClassWriterP FdoSmLpPostGisFeatureClass::GetPhysicalAddWriter()
{
    FdoSmPhClassWriterP writer(FdoSmLpFeatureClass::GetPhysicalAddWriter());
    FdoSmLpPostGisClassDefinition::SetPhysicalAddWriter(writer);

    return writer;
}
