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
#include "DataPropertyDefinition.h"
#include <Rdbms/Override/MySQL/MySqlOvDataPropertyDefinition.h>
#include <Rdbms/Override/MySQL/MySqlOvClassDefinition.h>


FdoSmLpMySqlDataPropertyDefinition::FdoSmLpMySqlDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpGrdDataPropertyDefinition(propReader, parent)
{
}

FdoSmLpMySqlDataPropertyDefinition::FdoSmLpMySqlDataPropertyDefinition(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdDataPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpMySqlDataPropertyDefinition::FdoSmLpMySqlDataPropertyDefinition(
    FdoSmLpDataPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
    FdoSmLpGrdDataPropertyDefinition(
        pBaseProperty,
        pTargetClass,
        logicalName,
        physicalName,
        bInherit,
        pPropOverrides
    )
{
}

FdoSmLpPropertyP FdoSmLpMySqlDataPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpMySqlDataPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpMySqlDataPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpMySqlDataPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}



bool FdoSmLpMySqlDataPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoMySQLOvClassDefinition* mysqlClassMapping = (FdoMySQLOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoMySQLOvPropertyDefinition* mysqlPropMapping =
        dynamic_cast<FdoMySQLOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( mysqlPropMapping ) {

        FdoMySQLOvPropertiesP(mysqlClassMapping->GetProperties())->Add( mysqlPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpMySqlDataPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoPhysicalPropertyMappingP retPropMapping;

    FdoMySQLOvDataPropertyDefinition* propMapping = FdoMySQLOvDataPropertyDefinition::Create( GetName() );
    retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  

    ((FdoSmLpMySqlDataPropertyDefinition*) this)->Finalize();

    // Add column name mapping if including default mappings or mapping is 
    // non-default (property and column names are different).
    if ( bIncludeDefaults || GetIsFixedColumn() ) {
        // For foreign schemas, column is root column name, which is the 
        // name of the column in the foreign table.
        FdoMySQLOvColumnP columnMapping = FdoMySQLOvColumn::Create( 
            (bIncludeDefaults || GetIsFixedColumn()) ? 
            (wcslen(GetRootColumnName()) > 0) ? GetRootColumnName() : GetColumnName() : L"");

        propMapping->SetColumn(columnMapping);
        bHasMappings = true;
    }

    // return NULL if no mappings.
    if ( !bHasMappings ) 
        retPropMapping = NULL;

    return retPropMapping;
}

void FdoSmLpMySqlDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Update according to Generic RDBMS overrides.
    FdoSmLpGrdDataPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );

    FdoMySQLOvDataPropertyDefinition* pDataPropOverrides = 
        dynamic_cast<FdoMySQLOvDataPropertyDefinition*>(pPropOverrides);

    // Error if the overrides are not MySQL overrides for a data property
    if ( pPropOverrides && !pDataPropOverrides ) 
        AddWrongOverrideTypeError();

    if ( (GetElementState() == FdoSchemaElementState_Added) ||
         (GetElementState() == FdoSchemaElementState_Modified) ||
         GetIsFromFdo() // Process overrides when this object from config doc.
    ) {
        if ( pDataPropOverrides ) {
            // Get the column overrides
            FdoMySQLOvColumnP columnOverrides = pDataPropOverrides->GetColumn();
            if ( columnOverrides ) {
                // no MySQL-specific column overrides yet
            }
        }
	}
}

