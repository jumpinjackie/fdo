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
#include "GeometricPropertyDefinition.h"
#include <Sm/Lp/FeatureClass.h>

FdoSmLpPostGisGeometricPropertyDefinition::FdoSmLpPostGisGeometricPropertyDefinition(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpGrdGeometricPropertyDefinition(reader, parent)
{
    // idle
}

FdoSmLpPostGisGeometricPropertyDefinition::FdoSmLpPostGisGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* fdoProp, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
    : FdoSmLpGrdGeometricPropertyDefinition(fdoProp, ignoreStates, parent)
{
    // idle
}

FdoSmLpPostGisGeometricPropertyDefinition::FdoSmLpPostGisGeometricPropertyDefinition(
    FdoSmLpGeometricPropertyP baseProperty,
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool inherit,
    FdoPhysicalPropertyMapping* propOverrides)
    : FdoSmLpGrdGeometricPropertyDefinition(
        baseProperty,
        targetClass,
        logicalName,
        physicalName,
        inherit,
        propOverrides)
{
    // idle
}

bool FdoSmLpPostGisGeometricPropertyDefinition::AddSchemaMappings(
    FdoPhysicalClassMappingP classMapping,
    bool includeDefaults) const
{
    bool hasMappings = false;
    
    FdoPostGISOvClassDefinition* pgClassMapping = NULL;
    pgClassMapping = static_cast<FdoPostGISOvClassDefinition*>(classMapping.p);

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping(GetSchemaMappings(includeDefaults));

    FdoPostGISOvPropertyDefinition* pgPropMapping = NULL;
    pgPropMapping = 
        dynamic_cast<FdoPostGISOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if (pgPropMapping)
    {
        FdoPostGISOvPropertiesP properties(pgClassMapping->GetProperties());
        properties->Add(pgPropMapping);
        hasMappings = true;
    }

    return hasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpPostGisGeometricPropertyDefinition::GetSchemaMappings(
    bool includeDefaults) const
{
    bool hasMappings = false;

    FdoPostGISOvGeometricPropertyDefinition* propMapping = NULL;
    propMapping = FdoPostGISOvGeometricPropertyDefinition::Create(GetName());

    FdoPhysicalPropertyMappingP retPropMapping;
    retPropMapping = static_cast<FdoPhysicalPropertyMapping*>(propMapping);

    FdoSmLpPostGisGeometricPropertyDefinition* thisClassDef = NULL;
    thisClassDef = 
        const_cast<FdoSmLpPostGisGeometricPropertyDefinition*>(this);
    thisClassDef->Finalize();

    FdoPostGISOvGeometricColumnP columnMapping(
        GetColumnMapping(includeDefaults));

    if (columnMapping)
    {
        propMapping->SetColumn(columnMapping);
        hasMappings = true;
    }

    // return NULL if no mappings.
    if (!hasMappings) 
        retPropMapping = NULL;

    return retPropMapping;
}

FdoPostGISOvGeometricColumnP FdoSmLpPostGisGeometricPropertyDefinition::GetColumnMapping(
    bool includeDefaults) const
{
    bool hasMappings = false;
    FdoPostGISOvGeometricColumnP geomCol(FdoPostGISOvGeometricColumn::Create());

    // For foreign schemas, column is root column name, which is the 
    // name of the column in the foreign table.
    FdoString* columnName = 
        (wcslen(GetRootColumnName()) > 0)
            ? GetRootColumnName() : GetColumnName();

    FdoStringP ovColName;

    // Set the column name only if including default mappings or this is 
    // a fixed column mapping.
    if (includeDefaults || GetIsFixedColumn()) 
        ovColName = columnName;

    // Create column mapping if overriding the column name.
    if (ovColName != L"")
    {
        geomCol->SetName(columnName);
        hasMappings = true;
    }

    // return NULL if no mappings found:
    if (!hasMappings)
        geomCol = NULL;

    return geomCol;
}


void FdoSmLpPostGisGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* fdoProp,
    FdoSchemaElementState state,
    FdoPhysicalPropertyMapping* propOverrides,
    bool ignoreStates)
{
    // Do updates from generic physical mappings:
    FdoSmLpGrdGeometricPropertyDefinition::Update(fdoProp, state, 
        propOverrides, ignoreStates);
}

void FdoSmLpPostGisGeometricPropertyDefinition::Update(
    FdoRdbmsOvGeometricColumn* columnOverrides)
{
    FdoSmLpGrdGeometricPropertyDefinition::Update(columnOverrides);
}

bool FdoSmLpPostGisGeometricPropertyDefinition::CheckGeomPropShapeType(
    FdoGeometryType type) const
{
    bool typeChecked = false;
    typeChecked = 
        FdoSmLpGeometricPropertyDefinition::CheckGeomPropShapeType(type);

    if (typeChecked)
    {
        FdoGeometryType allowedGeomType = FdoGeometryType_MultiGeometry;

        long allowedHexTypes = GetSpecificGeometryTypes() ;

        // Call FdoCommonGeometryUtil::MapHexCodeToGeometryType only if
        // we have a single geometry allowed otherwise skip the test if
        // we have more than one geometries (e.g. point and multipoint)

        if (FdoCommonGeometryUtil::GetCountGeometryTypesFromHex(
            allowedHexTypes) <= 1)
        {
            allowedGeomType = 
                FdoCommonGeometryUtil::MapHexCodeToGeometryType(allowedHexTypes);

            typeChecked = 
                ((FdoGeometryType_MultiGeometry == allowedGeomType)
                  || (allowedGeomType == type));
        }
    }

    return typeChecked;
}

FdoSmLpPropertyP FdoSmLpPostGisGeometricPropertyDefinition::NewInherited(
    FdoSmLpClassDefinition* subClass) const
{
    // Remove const qualifier
    FdoSmLpPostGisGeometricPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisGeometricPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpGeometricPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpGeometricPropertyDefinition*>(thisProp);
    
    return (new FdoSmLpPostGisGeometricPropertyDefinition(
            FDO_SAFE_ADDREF(propDef),
            subClass, L"", L"", true));
}

FdoSmLpPropertyP FdoSmLpPostGisGeometricPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* targetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* propOverrides) const
{
    // Remove const qualifier
    FdoSmLpPostGisGeometricPropertyDefinition* thisProp = NULL;
    thisProp = const_cast<FdoSmLpPostGisGeometricPropertyDefinition*>(this);

    // Cast down the inheritance hierarchy
    FdoSmLpGeometricPropertyDefinition* propDef = NULL;
    propDef = static_cast<FdoSmLpGeometricPropertyDefinition*>(thisProp);

    FdoSmLpPostGisGeometricPropertyDefinition* geomProp = NULL;

    geomProp = (new FdoSmLpPostGisGeometricPropertyDefinition(
                FDO_SAFE_ADDREF(propDef),
                targetClass, logicalName, physicalName,
                false, propOverrides));

    return geomProp;
}
