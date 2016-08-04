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
#include <Sm/Lp/ClassDefinition.h>
#include "ObjectPropertyDefinition.h"
#include "ObjectPropertyClass.h"
#include "../../../SchemaMgr/Lp/PropertyMappingConcrete.h"
#include "../../../SchemaMgr/Lp/PropertyMappingSingle.h"
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingSingle.h>
#include <Rdbms/Override/PostGis/PostGisOvObjectPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvTable.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>
#include "../Ph/Table.h"
#include "Schema.h"

FdoSmLpPostGisObjectPropertyDefinition::FdoSmLpPostGisObjectPropertyDefinition(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpGrdObjectPropertyDefinition(reader, parent)
{
    ReadPrefix(reader, NewMappingOverrides());
}

FdoSmLpPostGisObjectPropertyDefinition::FdoSmLpPostGisObjectPropertyDefinition(
    FdoObjectPropertyDefinition* fdoProp, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpGrdObjectPropertyDefinition(fdoProp, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisObjectPropertyDefinition::FdoSmLpPostGisObjectPropertyDefinition(
    FdoPtr<FdoSmLpObjectPropertyDefinition> base, 
    FdoSmLpClassDefinition* targetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool inherit,
    FdoPhysicalPropertyMapping* overrides)
    : FdoSmLpGrdObjectPropertyDefinition(base, targetClass, logicalName,
        physicalName, inherit, overrides)
{
    // idle
}

FdoSmLpPostGisObjectPropertyDefinition::~FdoSmLpPostGisObjectPropertyDefinition()
{
    // idle
}

bool FdoSmLpPostGisObjectPropertyDefinition::AddSchemaMappings(
    FdoPhysicalClassMappingP mapping,
    bool includeDefaults) const
{
    bool hasMappings = false;
    
    FdoPostGISOvClassDefinition* pgClassMapping = NULL;
    pgClassMapping = static_cast<FdoPostGISOvClassDefinition*>(mapping.p);

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping(GetSchemaMappings(includeDefaults));
    FdoPostGISOvPropertyDefinition* pgPropDef = NULL;
    pgPropDef = dynamic_cast<FdoPostGISOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if (pgPropDef)
    {
        FdoPostGISOvPropertiesP properties(pgClassMapping->GetProperties());
        properties->Add(pgPropDef);
        hasMappings = true;
    }

    return hasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpPostGisObjectPropertyDefinition::GetSchemaMappings(
    bool includeDefaults) const
{
    bool hasMappings = false;

    FdoPostGISOvObjectPropertyDefinition* propMapping = NULL;
    propMapping = FdoPostGISOvObjectPropertyDefinition::Create(GetName());

    FdoPhysicalPropertyMappingP retSchemaMapping(
        static_cast<FdoPhysicalPropertyMapping*>(propMapping));

    FdoSmLpPostGisObjectPropertyDefinition* thisClassDef = NULL;
    thisClassDef = const_cast<FdoSmLpPostGisObjectPropertyDefinition*>(this);
    thisClassDef->Finalize();

    hasMappings = SetSchemaMappings(retSchemaMapping, includeDefaults);

    // Return NULL if no mappings.
    if (!hasMappings)
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

FdoSmLpObjectPropertyClassP FdoSmLpPostGisObjectPropertyDefinition::NewClass(
    FdoSmLpObjectPropertyDefinition* parent,
	FdoSmLpClassDefinition* parentType,
	FdoSmLpPropertyMappingDefinition* mapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* overrides)
{
    FdoSmLpPostGisObjectPropertyClass* propClass = NULL;

    propClass = new FdoSmLpPostGisObjectPropertyClass(parent, parentType,
                mapping, mappingType, overrides);

    return (static_cast<FdoSmLpObjectPropertyClass*>(propClass));
}

FdoSmLpObjectPropertyClassP FdoSmLpPostGisObjectPropertyDefinition::NewClass(
   	FdoSmLpObjectPropertyClassP base,
	FdoSmLpObjectPropertyDefinition* parent,
	FdoSmLpClassDefinition* parentType,
	FdoSmLpPropertyMappingDefinition* mapping,
	FdoSmLpPropertyMappingType mappingType,
    FdoPhysicalClassMapping* overrides)
{
    FdoSmLpPostGisObjectPropertyClass* propClass = NULL;

    propClass = new FdoSmLpPostGisObjectPropertyClass(base, parent, parentType,
                mapping, mappingType, overrides);
        

    return (static_cast<FdoSmLpObjectPropertyClass*>(propClass));
}

FdoSmLpPropertyP FdoSmLpPostGisObjectPropertyDefinition::NewInherited(
    FdoSmLpClassDefinition* subClass) const
{
    // Remove const qualifier
    FdoSmLpPostGisObjectPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisObjectPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpObjectPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpObjectPropertyDefinition*>(thisProp);

    return (new FdoSmLpPostGisObjectPropertyDefinition(
            FDO_SAFE_ADDREF(propDef), subClass, L"", L"", true));
}

FdoSmLpPropertyP FdoSmLpPostGisObjectPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* overrides) const
{
    // Remove const qualifier
    FdoSmLpPostGisObjectPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisObjectPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpObjectPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpObjectPropertyDefinition*>(thisProp);

    return (new FdoSmLpPostGisObjectPropertyDefinition(
            FDO_SAFE_ADDREF(propDef),
            targetClass, logicalName, physicalName,
            false, overrides));
}

FdoRdbmsOvObjectPropertyDefinition* FdoSmLpPostGisObjectPropertyDefinition::NewMappingOverrides()
{
    FdoRdbmsOvObjectPropertyDefinition* propDef = NULL;
    propDef = FdoPostGISOvObjectPropertyDefinition::Create(GetName());
    return propDef;
}