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
#include "ClassDefinition.h"
#include "DataPropertyDefinition.h"
#include "../Ph/Table.h"
#include <Sm/Lp/Schema.h>

FdoSmLpGrdClassDefinition::FdoSmLpGrdClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
    FdoSmLpClassBase( classReader, parent )
{
    FdoStringP ovTableMapping = classReader->GetTableMapping();
    SetOvTableMapping( ovTableMapping );
    if ( ovTableMapping != L"" ) 
        SetTableMapping( FdoSmOvTableMappingTypeMapper::String2Type(ovTableMapping) );
}

FdoSmLpGrdClassDefinition::FdoSmLpGrdClassDefinition(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
    FdoSmLpClassBase( pFdoClass, bIgnoreStates, parent )
{
}

FdoSmLpGrdClassDefinition::FdoSmLpGrdClassDefinition(
	FdoString* name, 
	FdoString* description,
	FdoSmLpSchemaElement* parent,
	FdoSmLpClassDefinitionP pSrcClass,
	FdoSmOvTableMappingType tableMapping,
	FdoString* tableName,
	FdoSmPhDbObjectP table,
	FdoSchemaElementState elementState
) : 
	FdoSmLpClassBase(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState, parent->GetIsFromFdo())
{
}

FdoSmLpGrdClassDefinition::~FdoSmLpGrdClassDefinition(void)
{
}

bool FdoSmLpGrdClassDefinition::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    const FdoSmLpSchema*     pSchema = RefLogicalPhysicalSchema();
    FdoRdbmsOvClassDefinition* grdClassMapping = (FdoRdbmsOvClassDefinition*)(FdoPhysicalClassMapping*)classMapping;
    FdoInt32                    i;
    FdoSmOvTableMappingType  tableMapping = GetTableMapping();
    FdoStringP ovTableMapping = GetOvTableMapping();
    FdoStringP database = GetDatabase();
    FdoStringP owner = GetOwner();
    bool bHasMappings = false;


    if ( bIncludeDefaults ) {
        // When including default mappings, write out the 
        // resultant table mapping. 
        grdClassMapping->SetTableMapping( tableMapping );
        bHasMappings = true;
    }
    else {
        // Write the table mapping only if not defaulting to the 
        // feature schema table mapping.
        if ( (ovTableMapping.GetLength() > 0) && (ovTableMapping != L"Default") ) {
            grdClassMapping->SetTableMapping( FdoSmOvTableMappingTypeMapper::String2Type(ovTableMapping) );
            bHasMappings = true;
        }
    }

    // Add the schema mappings for each property
    for ( i = 0; i < RefProperties()->GetCount(); i++ ) {
        const FdoSmLpPropertyDefinition* pProp = RefProperties()->RefItem(i);

        // Skip inherited properties. Their base properties carry the schema mappings.
        if ( !pProp->RefBaseProperty() ) {
            bHasMappings = pProp->AddSchemaMappings( classMapping, bIncludeDefaults ) || bHasMappings ;
        }
    }
    return bHasMappings;
}

void FdoSmLpGrdClassDefinition::SetPkeyMapping( FdoRdbmsOvTable* tableMapping, FdoSmPhDbObjectP dbObject ) const
{
    FdoSmPhTableP phTable = dbObject->GetLowestRootObject().p->SmartCast<FdoSmPhTable>();

    if ( phTable ) 
        tableMapping->SetPKeyName( phTable->GetPkeyName() );
}

void FdoSmLpGrdClassDefinition::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoRdbmsOvClassP pClassOverrides;

    // Set element state here since Update functions invoked from
    // here sometimes call GetElementState().
    SetElementState(elementState);

    // Extract the class overrides from the schema override set
    if ( pOverrides ) {
        FdoRdbmsOvPhysicalSchemaMapping* pRdbmsOverrides = (FdoRdbmsOvPhysicalSchemaMapping*) pOverrides;
        pClassOverrides = FdoRdbmsOvReadOnlyClassesP(pRdbmsOverrides->GetClasses())->FindItem( GetName() );
    }

    // Update from the class overrides
    Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );
}

void FdoSmLpGrdClassDefinition::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoSmLpSchemaP pSchema = GetLogicalPhysicalSchema();
    FdoSmOvTableMappingType tableMappingType = FdoSmOvTableMappingType_Default;

    if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
        // Extract any class overrides
        if ( pClassOverrides  ) {
            FdoRdbmsOvClassDefinition* pRdbmsOverrides = (FdoRdbmsOvClassDefinition*) pClassOverrides;

            tableMappingType = pRdbmsOverrides->GetTableMapping();
            // Save the table mapping specified by overrides, since mDbObjectMapping
            // may get defaulted.
            SetOvTableMapping( FdoSmOvTableMappingTypeMapper::Type2String( tableMappingType ) );
        }

        // TODO: the following is inconsistent with the way table name is handle
        // In both cases should report modification errors or ignore on modification.
        SetTableMapping( tableMappingType );
	}
}

void FdoSmLpGrdClassDefinition::SetPhysicalAddWriter( FdoSmPhClassWriterP writer )
{
    if ( wcslen(GetOvTableMapping()) > 0 ) 
        writer->SetTableMapping( GetOvTableMapping() );
}

void FdoSmLpGrdClassDefinition::UpdateTable( FdoStringP database, FdoStringP owner, FdoRdbmsOvTable* tableOverrides )
{
    FdoStringP ovTableName;
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Check for a mapping override for table name.
    if ( Get_TableMapping() != FdoSmOvTableMappingType_BaseTable ) {

        bool bFromConfigFile = GetIsFromConfig();

        if ( tableOverrides && (wcslen(tableOverrides->GetName()) > 0) ) {
            ovTableName = tableOverrides->GetName();
        }
        else if ( bFromConfigFile ) {
            // Always override table name when this object is from an FDO config document.
            // This prevents the table name from being later adjusted. Classes from config docs
            // always attach to existing table so must not mangle table name.
            ovTableName = GetName();
        }

        if ( ovTableName != L"" ) {
            if ( !VldDbObjectName(ovTableName) ) {
                ovTableName = L"";
            }
        }
    }

    if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
        // New object or from FDO config doc so set table from overrides.
        
        // Owner and Database must be set before table,
        // since SetDbObjectName sometimes generates a name unique
        // within owner.
        SetDatabase( database );
        SetOwner( owner );
        if ( tableOverrides ) {
            SetNewPkeyName( tableOverrides->GetPKeyName() );
        }

        // Set actual table name if overridden.
        if ( (ovTableName != L"") && (Get_TableMapping() != FdoSmOvTableMappingType_BaseTable) ) {
            SetDbObjectName( ovTableName );
            SetIsFixedDbObject( true );
        }
    }
    else if ( GetElementState() == FdoSchemaElementState_Modified ) {

        // Changing class table not yet supported.
        if ( Get_TableMapping() != FdoSmOvTableMappingType_BaseTable ) {
            if ( (ovTableName.GetLength() > 0) && (ovTableName.ICompare(GetDbObjectName()) != 0) ) 
                AddTableNameChangeError(ovTableName);
        }
    }
}

void FdoSmLpGrdClassDefinition::PostFinalize()
{
    FdoSmPhGrdTableP table = FindPhDbObject().p->SmartCast<FdoSmPhGrdTable>();

    // For new tables, lt and locking modes are set when the table is instantiated.
    if ( (table != NULL) && (table->GetElementState() != FdoSchemaElementState_Added) )
    {
        FdoSmPhOwner* owner = static_cast<FdoSmPhOwner*>((FdoSmPhDbElement*) table->GetParent());
        
        const FdoSmLpDataPropertyDefinition* pLtProp = 
            FdoSmLpDataPropertyDefinition::Cast( GetProperties()->RefItem(FdoSmLpGrdDataPropertyDefinition::LtIdName) );
        
        const FdoSmLpDataPropertyDefinition* pLockProp = 
            FdoSmLpDataPropertyDefinition::Cast( GetProperties()->RefItem(FdoSmLpGrdDataPropertyDefinition::LockIdName) );

        // Set the LT mode to FDO only if the datastore's mode is FDO and 
        // the class has one of the LT system properties.
        // InitFdoLt does a further check to ensure that the LT property's column
        // is in the table.
        if ( (owner->GetLtMode() == FdoMode) && pLtProp && pLtProp->GetIsSystem() ) 
            table->InitFdoLt( pLtProp->GetColumnName() );

        // Set the Locking mode to FDO only if the datastore's mode is FDO and 
        // the class has one of the Locking system properties.
        // InitFdoLt does a further check to ensure that the Locking property's column
        // is in the table.
        if ( (owner->GetLckMode() == FdoMode) && pLockProp && pLockProp->GetIsSystem() ) 
            table->InitFdoLocking( pLockProp->GetColumnName() );
    }
}

void FdoSmLpGrdClassDefinition::NewPkey( FdoSmPhTableP table )
{
    // When the class has an LtId property, its column must be added
    // to the class table primary key.
    const FdoSmLpDataPropertyDefinition* pLtIdProp = 
        FdoSmLpDataPropertyDefinition::Cast( GetProperties()->RefItem(FdoSmLpGrdDataPropertyDefinition::LtIdName) );

    if ( pLtIdProp ) 
        table->AddPkeyCol( pLtIdProp->GetColumnName() );
}

void FdoSmLpGrdClassDefinition::NewUkey( FdoSmPhTableP table, FdoSmLpDataPropertiesP pProps )
{
    FdoSmLpClassBase::NewUkey( table, pProps );

	FdoSmPhBatchColumnsP    ukeys = table->GetUkeyColumns(); 
    
    // When the class has an LtId property, its column must be added
    // to the unique key.
    const FdoSmLpDataPropertyDefinition* pLtIdProp = 
        FdoSmLpDataPropertyDefinition::Cast( GetProperties()->RefItem(FdoSmLpGrdDataPropertyDefinition::LtIdName) );

    if ( pLtIdProp ) 
        table->AddUkeyCol( ukeys->GetCount() - 1, pLtIdProp->GetColumnName() );
}

