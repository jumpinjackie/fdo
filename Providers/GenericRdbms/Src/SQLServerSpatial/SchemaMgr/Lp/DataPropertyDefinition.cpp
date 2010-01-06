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
#include "DataPropertyDefinition.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvDataPropertyDefinition.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvClassDefinition.h>
#include "../Ph/Column.h"

FdoSmLpSqsDataPropertyDefinition::FdoSmLpSqsDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpGrdDataPropertyDefinition(propReader, parent)
{
}

FdoSmLpSqsDataPropertyDefinition::FdoSmLpSqsDataPropertyDefinition(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdDataPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpSqsDataPropertyDefinition::FdoSmLpSqsDataPropertyDefinition(
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

FdoSmLpPropertyP FdoSmLpSqsDataPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpSqsDataPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpSqsDataPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpSqsDataPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}

bool FdoSmLpSqsDataPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
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

FdoPhysicalPropertyMappingP FdoSmLpSqsDataPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoSqlServerOvDataPropertyDefinition* propMapping = FdoSqlServerOvDataPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  
    bool bHasMappings = false;

    ((FdoSmLpSqsDataPropertyDefinition*) this)->Finalize();

    // Add column mappings if including default mappings or mapping is 
    // non-default (property and column names are different).
    if ( bIncludeDefaults ||
        GetIsFixedColumn() ||
        (mComputedExpression.GetLength() > 0)
    ) {
        // For foreign schemas, column is root column name, which is the 
        // name of the column in the foreign table.
        FdoSqlServerOvColumnP columnMapping = FdoSqlServerOvColumn::Create( 
            (bIncludeDefaults || GetIsFixedColumn()) ? 
            (wcslen(GetRootColumnName()) > 0) ? GetRootColumnName() : GetColumnName() : L"");

        // Get SQL Server-specific mappings:
        if (mComputedExpression.GetLength() > 0)
            columnMapping->SetFormula(mComputedExpression);

        propMapping->SetColumn(columnMapping);
        bHasMappings = true;
    }

    // return NULL if no mappings.
    if ( !bHasMappings ) 
        retPropMapping = NULL;

    return retPropMapping;
}

void FdoSmLpSqsDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Update according to Generic RDBMS overrides.
    FdoSmLpGrdDataPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );

    FdoSqlServerOvDataPropertyDefinition* pDataPropOverrides = 
        dynamic_cast<FdoSqlServerOvDataPropertyDefinition*>(pPropOverrides);

    // Error if the overrides are not SqlServer overrides for a data property
    if ( pPropOverrides && !pDataPropOverrides ) 
        AddWrongOverrideTypeError();

    if ( (GetElementState() == FdoSchemaElementState_Added) ||
         (GetElementState() == FdoSchemaElementState_Modified) ||
         GetIsFromFdo() // ensures overrides handled for properties from FDO config doc.
    ) {
        if ( pDataPropOverrides ) {
            // Get the column overrides
            FdoSqlServerOvColumnP columnOverrides = pDataPropOverrides->GetColumn();
            if ( columnOverrides ) {

                // Set SQL Server-specific mappings:
                if (wcslen(columnOverrides->GetFormula()) > 0)
                    mComputedExpression = columnOverrides->GetFormula();

                // TODO: make sure *modifying* the formula of an existing column works correctly
            }
        }
	}
}

FdoSmPhColumnP FdoSmLpSqsDataPropertyDefinition::NewColumn( 
        FdoSmPhDbObjectP dbObject, 
        FdoStringP columnName,
        bool nullable,
        FdoStringP rootColumnName
)
{
    FdoSmPhColumnP ret = FdoSmLpDataPropertyDefinition::NewColumn(dbObject, columnName, nullable, rootColumnName);

    // Get corresponding physical column:
    FdoSmPhSqsColumn* sqsCol = dynamic_cast<FdoSmPhSqsColumn*>(ret.p);

    // Pass SQL Server-specific overrides from LogicalPhysical layer to physical layer:
    if (sqsCol != NULL)
    {
        sqsCol->SetComputedExpression(mComputedExpression);
    }

    return ret;
}


void FdoSmLpSqsDataPropertyDefinition::Finalize()
{
    FdoSmObjectState state = GetState();
    if (state == FdoSmObjectState_Initial)  // need to check this to avoid infinite loop with call to GetColumn()
    {
        // Let superclass do the work (including creating the column):
        FdoSmLpGrdDataPropertyDefinition::Finalize();

        // Grab overrides from column:
        FdoSmPhColumnP col = GetColumn();
        if (col != NULL)
        {
            FdoPtr<FdoSmPhSqsColumn> col2 = FDO_SAFE_ADDREF(dynamic_cast<FdoSmPhSqsColumn*>(col.p));
            mComputedExpression = col2->GetComputedExpression();
        }
    }
}
