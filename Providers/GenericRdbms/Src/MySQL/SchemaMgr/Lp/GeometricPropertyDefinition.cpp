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
#include "GeometricPropertyDefinition.h"
#include <Sm/Lp/FeatureClass.h>

FdoSmLpMySqlGeometricPropertyDefinition::FdoSmLpMySqlGeometricPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpGrdGeometricPropertyDefinition(propReader, parent)
{
}

FdoSmLpMySqlGeometricPropertyDefinition::FdoSmLpMySqlGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpMySqlGeometricPropertyDefinition::FdoSmLpMySqlGeometricPropertyDefinition(
    FdoSmLpGeometricPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
    FdoSmLpGrdGeometricPropertyDefinition(
        pBaseProperty,
        pTargetClass,
        logicalName,
        physicalName,
        bInherit,
        pPropOverrides
    )
{
}

bool FdoSmLpMySqlGeometricPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoMySQLOvClassDefinition* mqlClassMapping = (FdoMySQLOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoMySQLOvPropertyDefinition* mqlPropMapping = 
        dynamic_cast<FdoMySQLOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( mqlPropMapping ) {
        FdoMySQLOvPropertiesP(mqlClassMapping->GetProperties())->Add( mqlPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpMySqlGeometricPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoMySQLOvGeometricPropertyDefinition* propMapping = FdoMySQLOvGeometricPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  
    bool bHasMappings = false;

    ((FdoSmLpMySqlGeometricPropertyDefinition*)this)->Finalize();

    FdoMySQLOvGeometricColumnP columnMapping = GetColumnMapping( bIncludeDefaults );

    if ( columnMapping ) {
        propMapping->SetColumn(columnMapping);
        bHasMappings = true;
    }

    // return NULL if no mappings.
    if ( !bHasMappings ) 
        retPropMapping = NULL;

    return retPropMapping;
}

FdoMySQLOvGeometricColumnP FdoSmLpMySqlGeometricPropertyDefinition::GetColumnMapping( bool bIncludeDefaults ) const
{
    FdoMySQLOvGeometricColumnP geomCol = FdoMySQLOvGeometricColumn::Create();
    bool bHasMappings = false;
    // For foreign schemas, column is root column name, which is the 
    // name of the column in the foreign table.
    FdoString* columnName = ( wcslen(GetRootColumnName()) > 0) ? GetRootColumnName() : GetColumnName();

    FdoStringP ovColName;

    // Set the column name only if including default mappings or this is 
    // a fixed column mapping.
    if ( bIncludeDefaults || GetIsFixedColumn() ) 
        ovColName = columnName;

    // Create column mapping if overriding the column name.
    if (ovColName != L"") {
        if ( ovColName != L"" ) {
            geomCol->SetName( columnName );
            bHasMappings = true;
        }
    }

    // Get MySQL-specific mappings:

    // (none currently)

    // return NULL if no mappings found:
    if (!bHasMappings)
        geomCol = NULL;

    return geomCol;
}


void FdoSmLpMySqlGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,       
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Do updates from generic physical mappings:
    FdoSmLpGrdGeometricPropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

    FdoMySQLOvGeometricPropertyDefinition* mqlGeomPropOverrides = dynamic_cast<FdoMySQLOvGeometricPropertyDefinition*>(pPropOverrides);
    if (mqlGeomPropOverrides)
    {
        FdoMySQLOvGeometricColumnP mqlGeomColOverrides = mqlGeomPropOverrides->GetColumn();
        if (mqlGeomColOverrides)
        {
            // Handle mappings for new objects or ones that originated from FDO config doc.
            if ( (elementState == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
                // no MySQL-specific mappings to update, currently.
            }
            else
            {
                // NOTE: we ignore any MySQL-specific overrides that have changed;
                // This is consistent with Oracle in that modifying overrides is generally not supported.
            }
        }
    }
}

FdoSmLpPropertyP FdoSmLpMySqlGeometricPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpMySqlGeometricPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpMySqlGeometricPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpMySqlGeometricPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}

bool FdoSmLpMySqlGeometricPropertyDefinition::CheckGeomPropShapeType (FdoGeometryType geomType) const
{
    bool retVal = FdoSmLpGeometricPropertyDefinition::CheckGeomPropShapeType (geomType);
    if( retVal )
    {
        FdoGeometryType allowedGeomType = FdoGeometryType_MultiGeometry;
        long allowedHexTypes = GetSpecificGeometryTypes() ;
        // call FdoCommonGeometryUtil::MapHexCodeToGeometryType only if we have a single geometry allowed
        // otherwise skip the test if we have more than one geometries (e.g. point and multipoint)
        if (FdoCommonGeometryUtil::GetCountGeometryTypesFromHex(allowedHexTypes) <= 1)
                allowedGeomType = FdoCommonGeometryUtil::MapHexCodeToGeometryType(allowedHexTypes);
            retVal = ((allowedGeomType == FdoGeometryType_MultiGeometry) || (allowedGeomType == geomType) );
    }
    return retVal;
}
