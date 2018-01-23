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
#include <Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>
#include <Sm/Error.h>

FdoSmLpGrdGeometricPropertyDefinition::FdoSmLpGrdGeometricPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpGeometricPropertyDefinition(propReader, parent)
{
}

FdoSmLpGrdGeometricPropertyDefinition::FdoSmLpGrdGeometricPropertyDefinition(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGeometricPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpGrdGeometricPropertyDefinition::FdoSmLpGrdGeometricPropertyDefinition(
    FdoSmLpGeometricPropertyP pBaseProperty,
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
    FdoSmLpGeometricPropertyDefinition(
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

void FdoSmLpGrdGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoRdbmsOvPropertyP pPropOverrides;

    // Find the property overrides
    if ( pClassOverrides ) {
        FdoRdbmsOvClassDefinition* pRdbmsOverrides = (FdoRdbmsOvClassDefinition*) pClassOverrides;
        pPropOverrides = FdoRdbmsOvReadOnlyPropertiesP(pRdbmsOverrides->GetProperties())->FindItem( GetName() );
    }

    // Update from property overrides.
    Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
}

void FdoSmLpGrdGeometricPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // do base update first
    FdoSmLpGeometricPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
    // Update from Overrides second
    Update( pPropOverrides );
}

void FdoSmLpGrdGeometricPropertyDefinition::Update(
    FdoPhysicalPropertyMapping* pPropOverrides
)
{
    FdoRdbmsOvGeometricPropertyDefinition* pGeomPropOverrides = 
        dynamic_cast<FdoRdbmsOvGeometricPropertyDefinition*>(pPropOverrides);

    // Error if the overrides are not for a geometric property
    if ( pPropOverrides && !pGeomPropOverrides ) 
        AddWrongOverrideTypeError();

    FdoRdbmsOvGeometricColumnP columnOverrides;

    if ( pGeomPropOverrides ) 
    {
        // Get the column name from the physical overrides, if specified.
        columnOverrides = pGeomPropOverrides->GetColumn();
        FdoSmOvGeometricColumnType columnType = pGeomPropOverrides->GetGeometricColumnType();
        FdoSmOvGeometricContentType contentType = pGeomPropOverrides->GetGeometricContentType();

        // Check that the overrides are compatible.
        if ( (columnOverrides != NULL && FdoSmOvGeometricColumnType_Double == columnType) ||
             (FdoSmOvGeometricColumnType_Double == columnType && FdoSmOvGeometricContentType_Ordinates != contentType))
        {
            FdoSmLpGeometricPropertyDefinition::AddOverrideConflictError();
        }

        if (FdoSmOvGeometricColumnType_Default != columnType)
            SetGeometricColumnType(pGeomPropOverrides->GetGeometricColumnType());
        if (FdoSmOvGeometricContentType_Default != contentType)
            SetGeometricContentType(pGeomPropOverrides->GetGeometricContentType());

        if (FdoSmOvGeometricColumnType_Double == this->GetGeometricColumnType() &&
            FdoSmOvGeometricContentType_Ordinates == this->GetGeometricContentType())
        {
            this->SetColumnNameX(pGeomPropOverrides->GetXColumnName());
            this->SetColumnNameY(pGeomPropOverrides->GetYColumnName());
            this->SetColumnNameZ(pGeomPropOverrides->GetZColumnName());
        }
    }

    if (FdoSmOvGeometricColumnType_Double != this->GetGeometricColumnType())
        Update( columnOverrides );
}

void FdoSmLpGrdGeometricPropertyDefinition::Update(
    FdoRdbmsOvGeometricColumn* pColumnOverrides
)
{
    FdoStringP ovColName;
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    if ( pColumnOverrides ) {
        SetIsFixedColumn( true );

        if ( wcslen(pColumnOverrides->GetName()) > 0 )
            ovColName = pColumnOverrides->GetName();
    }

    if ( (ovColName == L"") && (GetElementState() == FdoSchemaElementState_Unchanged) && GetIsFromFdo() ) {
        // Always make column name fixed when this object is from an FDO config document.
        // In this case this property must attach to an existing column so must 
        // prevent the column name from being mangled.
        // Property always attaches to a column in the providers neutral case.
        // An explicit override must be supplied to attach to column whose
        // name is of another case.
        SetIsFixedColumn( true );
        ovColName = GetName();
    }

    if ( (!RefBaseProperty()) && ((GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo()) ) {
        // Set column name from override when property is not inherited and it is
        // new or from a config doc.
        if ( ovColName.GetLength() > 0 ) {
            if ( ColumnIsForeign() )
                SetRootColumnName( ovColName );

            SetColumnName( ovColName );
        }
	}
}

