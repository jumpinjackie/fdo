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
#include <Rdbms/Override/RdbmsOvDataPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>

const FdoStringP FdoSmLpGrdDataPropertyDefinition::LtIdName(L"LtId",true);
const FdoStringP FdoSmLpGrdDataPropertyDefinition::NextLtIdName(L"NextLtId",true);
const FdoStringP FdoSmLpGrdDataPropertyDefinition::LockIdName(L"LockId",true);
const FdoStringP FdoSmLpGrdDataPropertyDefinition::LockTypeName(L"LockType",true);

FdoSmLpGrdDataPropertyDefinition::FdoSmLpGrdDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpDataPropertyDefinition(propReader, parent)
{
}

FdoSmLpGrdDataPropertyDefinition::FdoSmLpGrdDataPropertyDefinition(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpDataPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpGrdDataPropertyDefinition::FdoSmLpGrdDataPropertyDefinition(
    FdoSmLpDataPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
    FdoSmLpDataPropertyDefinition(
        pBaseProperty,
        pTargetClass,
        logicalName,
        physicalName,
        bInherit,
        pPropOverrides
    )
{
    // Apply the Schema Overrides.
    Update( pPropOverrides );
}

void FdoSmLpGrdDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoRdbmsOvPropertyP pPropOverrides;

    // Get property overrides
    if ( pClassOverrides ) {
        FdoRdbmsOvClassDefinition* pRdbmsOverrides = (FdoRdbmsOvClassDefinition*) pClassOverrides;
        pPropOverrides = FdoRdbmsOvReadOnlyPropertiesP(pRdbmsOverrides->GetProperties())->FindItem( GetName() );
    }

    // Update from property overrides
    Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
}

void FdoSmLpGrdDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Do base update first.
    FdoSmLpDataPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
    // Update from Overrides second
    Update( pPropOverrides );
}

void FdoSmLpGrdDataPropertyDefinition::Update(
    FdoPhysicalPropertyMapping* pPropOverrides
)
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoRdbmsOvDataPropertyDefinition* pDataPropOverrides = 
        dynamic_cast<FdoRdbmsOvDataPropertyDefinition*>(pPropOverrides);

    // Error if the overrides are not for a data property
    if ( pPropOverrides && !pDataPropOverrides ) 
        AddWrongOverrideTypeError();

    FdoStringP ovColName;

    if ( pDataPropOverrides ) {
        // Get the column name from the physical overrides, if specified.
        FdoRdbmsOvColumnP columnOverrides = pDataPropOverrides->GetColumn();

        if ( columnOverrides ) {
            SetIsFixedColumn( true );
            if ( wcslen(columnOverrides->GetName()) > 0 )
                ovColName = columnOverrides->GetName();
        }
    }
	
    if ( (ovColName == L"") && (GetElementState() == FdoSchemaElementState_Unchanged) && GetIsFromFdo() ) {
        // Always make column name fixed when this object is from an FDO config document.
        // In this case this property must attach to an existing column so must 
        // prevent the column name from being mangled.
        // Property always attaches to a column in the providers neutral case.
        // An explicit override must be supplied to attach to column whose
        // name is of another case.
        ovColName =GetName();
        SetIsFixedColumn( true );
    }

    if ( (!RefBaseProperty()) && ((GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo()) ) {
        // Set column name from override when property is not inherited and it is
        // new or from a config doc.
        if ( ovColName.GetLength() > 0 ) {
            // For foreign columns, root column is always the one 
            // specified in the overrides.
            if ( ColumnIsForeign() )
                SetRootColumnName( ovColName );

            SetColumnName( ovColName );
        }
    }
    else if ( GetElementState() == FdoSchemaElementState_Modified ) {			 
        if ( (ovColName.GetLength() > 0) && (ovColName.ICompare(GetColumnName()) != 0) ) 
            AddColNameChangeError( ovColName );
	}
}

