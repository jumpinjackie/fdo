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
#include "GeometricPropertyDefinition.h"
#include <Sm/Lp/FeatureClass.h>

FdoSmLpSqsGeometricPropertyDefinition::FdoSmLpSqsGeometricPropertyDefinition(
    FdoSmPhClassPropertyReaderP propReader,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(propReader, parent)
{
}

FdoSmLpSqsGeometricPropertyDefinition::FdoSmLpSqsGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdGeometricPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpSqsGeometricPropertyDefinition::FdoSmLpSqsGeometricPropertyDefinition(
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

bool FdoSmLpSqsGeometricPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoSqlServerOvClassDefinition* sqsClassMapping = (FdoSqlServerOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoSqlServerOvPropertyDefinition* sqsPropMapping = 
        dynamic_cast<FdoSqlServerOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( sqsPropMapping ) {
        FdoSqlServerOvPropertiesP(sqsClassMapping->GetProperties())->Add( sqsPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpSqsGeometricPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoSqlServerOvGeometricPropertyDefinition* propMapping = FdoSqlServerOvGeometricPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  
    bool bHasMappings = false;

    ((FdoSmLpSqsGeometricPropertyDefinition*)this)->Finalize();

    FdoSqlServerOvGeometricColumnP columnMapping = GetColumnMapping( bIncludeDefaults );

    if ( columnMapping ) {
        propMapping->SetColumn(columnMapping);
        bHasMappings = true;
    }

    // return NULL if no mappings.
    if ( !bHasMappings ) 
        retPropMapping = NULL;

    return retPropMapping;
}

FdoSqlServerOvGeometricColumnP FdoSmLpSqsGeometricPropertyDefinition::GetColumnMapping( bool bIncludeDefaults ) const
{
    FdoSqlServerOvGeometricColumnP geomCol = FdoSqlServerOvGeometricColumn::Create();
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

    // Get SQL Server-specific mappings:

    // (none currently)

    // return NULL if no mappings found:
    if (!bHasMappings)
        geomCol = NULL;

    return geomCol;
}


void FdoSmLpSqsGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,       
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Do updates from generic physical mappings:
    FdoSmLpGrdGeometricPropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

    //FdoSqlServerOvGeometricPropertyDefinition* sqsGeomPropOverrides = (FdoSqlServerOvGeometricPropertyDefinition*)(FdoRdbmsOvGeometricPropertyDefinition*)(FdoRdbmsOvPropertyDefinition*)pPropOverrides;
    FdoSqlServerOvGeometricPropertyDefinition* sqsGeomPropOverrides = dynamic_cast<FdoSqlServerOvGeometricPropertyDefinition*>(pPropOverrides);
    if (sqsGeomPropOverrides)
    {
        FdoSqlServerOvGeometricColumn* sqsGeomColOverrides = sqsGeomPropOverrides->GetColumn();
        if (sqsGeomColOverrides)
        {
            if ( (elementState == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
                // no SQL Server-specific mappings to update, currently.
            }
            else
            {
                // TODO: log an exception via GetErrors()->Add()  ?
            }
        }
    }
}

FdoSmLpPropertyP FdoSmLpSqsGeometricPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpSqsGeometricPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpSqsGeometricPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpSqsGeometricPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpGeometricPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}
