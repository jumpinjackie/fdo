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
#include <Rdbms/Override/ODBC/OdbcOvDataPropertyDefinition.h>
#include <Rdbms/Override/ODBC/OdbcOvClassDefinition.h>
#include "../Ph/Column.h"

FdoSmLpOdbcDataPropertyDefinition::FdoSmLpOdbcDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) :
    FdoSmLpGrdDataPropertyDefinition(propReader, parent)
{
}

FdoSmLpOdbcDataPropertyDefinition::FdoSmLpOdbcDataPropertyDefinition(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) :
    FdoSmLpGrdDataPropertyDefinition(pFdoProp, bIgnoreStates, parent)
{
}

FdoSmLpOdbcDataPropertyDefinition::FdoSmLpOdbcDataPropertyDefinition(
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

FdoSmLpPropertyP FdoSmLpOdbcDataPropertyDefinition::NewInherited( FdoSmLpClassDefinition* pSubClass ) const
{
    return new FdoSmLpOdbcDataPropertyDefinition( FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this), pSubClass, L"", L"", true );
}

FdoSmLpPropertyP FdoSmLpOdbcDataPropertyDefinition::NewCopy(
    FdoSmLpClassDefinition* pTargetClass,
    FdoStringP logicalName,
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
    return
        new FdoSmLpOdbcDataPropertyDefinition(
            FDO_SAFE_ADDREF((FdoSmLpDataPropertyDefinition*) this),
            pTargetClass,
            logicalName,
            physicalName,
            false,
            pPropOverrides
        );
}

bool FdoSmLpOdbcDataPropertyDefinition::AddSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoOdbcOvClassDefinition* odbcClassMapping = (FdoOdbcOvClassDefinition*)(FdoPhysicalClassMapping*) classMapping;  

    // Get the mappings for this property
    FdoPhysicalPropertyMappingP propMapping = GetSchemaMappings( bIncludeDefaults );
    FdoOdbcOvPropertyDefinition* odbcPropMapping =
        dynamic_cast<FdoOdbcOvPropertyDefinition*>(propMapping.p);

    // Add them to the given class.
    if ( odbcPropMapping ) {

        FdoOdbcOvPropertiesP(odbcClassMapping->GetProperties())->Add( odbcPropMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalPropertyMappingP FdoSmLpOdbcDataPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoOdbcOvDataPropertyDefinition* propMapping = FdoOdbcOvDataPropertyDefinition::Create( GetName() );
    FdoPhysicalPropertyMappingP retPropMapping = (FdoPhysicalPropertyMapping*) propMapping;  
    bool bHasMappings = false;

    ((FdoSmLpOdbcDataPropertyDefinition*) this)->Finalize();

    // Add column mappings if including default mappings or mapping is 
    // non-default (property and column names are different).
    if ( bIncludeDefaults ||
        GetIsFixedColumn() ||
        (mComputedExpression.GetLength() > 0)
    ) {
        // For foreign schemas, column is root column name, which is the 
        // name of the column in the foreign table.
        FdoOdbcOvColumnP columnMapping = FdoOdbcOvColumn::Create( 
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

void FdoSmLpOdbcDataPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
    // Update according to Generic RDBMS overrides.
    FdoSmLpGrdDataPropertyDefinition::Update( pFdoProp, elementState, pPropOverrides, bIgnoreStates );
}

FdoSmPhColumnP FdoSmLpOdbcDataPropertyDefinition::NewColumn( 
        FdoSmPhDbObjectP dbObject, 
        FdoStringP columnName,
        bool nullable,
        FdoStringP rootColumnName
)
{
    FdoSmPhColumnP ret = FdoSmLpDataPropertyDefinition::NewColumn(dbObject, columnName, nullable, rootColumnName);

    // Get corresponding physical column:
    FdoSmPhOdbcColumn* odbcCol = dynamic_cast<FdoSmPhOdbcColumn*>(ret.p);

    // Pass ODBC-specific overrides from LogicalPhysical layer to physical layer:
    if (odbcCol != NULL)
    {
        odbcCol->SetComputedExpression(mComputedExpression);
    }

    return ret;
}


void FdoSmLpOdbcDataPropertyDefinition::Finalize()
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
            FdoPtr<FdoSmPhOdbcColumn> col2 = FDO_SAFE_ADDREF(dynamic_cast<FdoSmPhOdbcColumn*>(col.p));
            mComputedExpression = col2->GetComputedExpression();
        }
    }
}
