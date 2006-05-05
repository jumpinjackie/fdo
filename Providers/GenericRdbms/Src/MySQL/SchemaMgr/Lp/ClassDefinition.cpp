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
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"
#include "Schema.h"
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/MySQL/MySqlOvClassDefinition.h>
#include "../Ph/DbObject.h"
#include "../Ph/Table.h"
#include "DataPropertyDefinition.h"

FdoSmLpMySqlClassDefinition::FdoSmLpMySqlClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpGrdClassDefinition(classReader, parent),
	FdoSmLpClassBase(classReader, parent)
{
    Init();
}

FdoSmLpMySqlClassDefinition::FdoSmLpMySqlClassDefinition(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
	FdoSmLpGrdClassDefinition(pFdoClass, bIgnoreStates, parent),
	FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
    Init();
}

FdoSmLpMySqlClassDefinition::FdoSmLpMySqlClassDefinition(
	FdoString* name, 
	FdoString* description,
	FdoSmLpSchemaElement* parent,
	FdoSmLpClassDefinitionP pSrcClass,
	FdoSmOvTableMappingType tableMapping,
	FdoString* tableName,
	FdoSmPhDbObjectP table,
	FdoSchemaElementState elementState
) : 
	FdoSmLpGrdClassDefinition(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState),
	FdoSmLpClassBase(name, description, parent, pSrcClass, tableMapping, tableName, table, elementState, parent->GetIsFromFdo())
{
    Init();
}

FdoSmLpMySqlClassDefinition::~FdoSmLpMySqlClassDefinition(void)
{
}

bool FdoSmLpMySqlClassDefinition::AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;

    FdoMySQLOvPhysicalSchemaMapping* mqlSchemaMapping = (FdoMySQLOvPhysicalSchemaMapping*)(FdoPhysicalSchemaMapping*) schemaMapping;

    // Get the physical mappings for this class
    FdoPhysicalClassMappingP classMapping = GetSchemaMappings( bIncludeDefaults );
    FdoMySQLOvClassDefinition* mqlClassMapping = 
        dynamic_cast<FdoMySQLOvClassDefinition*>(classMapping.p);

    // Add them to the schema override set.
    if ( mqlClassMapping ) {
        FdoMySQLOvClassesP(mqlSchemaMapping->GetClasses())->Add( mqlClassMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalClassMappingP FdoSmLpMySqlClassDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoMySQLOvClassDefinition*      classMapping = FdoMySQLOvClassDefinition::Create( GetName() );
    FdoPhysicalClassMappingP        retClassMapping = (FdoMySQLOvClassDefinition*) classMapping;

    // Set class type specific mappings.
    bool bHasMappings = SetSchemaMappings( retClassMapping, bIncludeDefaults );

    // return NULL if there are no mappings. This prevents generating empty objects.
    if ( !bHasMappings ) 
        retClassMapping = NULL;

    return retClassMapping;
}

bool FdoSmLpMySqlClassDefinition::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    ((FdoSmLpMySqlClassDefinition*)this)->Finalize();

    const FdoSmLpMySqlSchema*         pSchema = (const FdoSmLpMySqlSchema*) RefLogicalPhysicalSchema();
    FdoMySQLOvClassDefinition*      mqlClassMapping = dynamic_cast<FdoMySQLOvClassDefinition*>( (FdoPhysicalClassMapping*) classMapping );
 
    // Default the table mapping to the one for the Feature Schema.
    FdoSmOvTableMappingType      defaultTableMapping = pSchema->GetTableMapping();
    FdoSmOvTableMappingType      tableMapping = GetTableMapping();
    FdoStringP                      database = GetDatabase();
    FdoSmLpDbObjectP                dbObject = ((FdoSmLpMySqlClassDefinition*)this)->GetDbObject();
    bool                            bHasMappings = FdoSmLpGrdClassDefinition::SetSchemaMappings( classMapping, bIncludeDefaults );


    // Set the table physical mappings of including default mappings or there are any
    // non-default schema mappings.
    if ( bIncludeDefaults || 
         ((tableMapping != FdoSmOvTableMappingType_BaseTable) && GetIsFixedDbObject()) ||
         (database.GetLength() > 0 && database != pSchema->GetDatabase()) ||
         ( mTableDataDirectory.GetLength() > 0 && mTableDataDirectory != pSchema->GetTableDataDirectory()) ||
         ( mTableIndexDirectory.GetLength() > 0 && mTableIndexDirectory != pSchema->GetIndexDataDirectory() ) ||
         ( mTableStorageEngine != MySQLOvStorageEngineType_Default && mTableStorageEngine != StorageEngine_StringToEnum(pSchema->GetTableStorageEngine()) ) ||
         ( mAutoIncrementPropertyName.GetLength() > 0 ) ||
         ( mAutoIncrementSeed != 1 )
    ) {
        // For foreign schemas override table name is the root (foreign) table.
        FdoMySQLOvTableP tableMapping = FdoMySQLOvTable::Create( 
            wcslen(GetRootDbObjectName()) > 0 ?
                GetRootDbObjectName() :
                GetDbObjectName()    
        );
        
        if ( dbObject ) {
            FdoSmPhDbObjectP phDbObject = dbObject->GetDbObject();
            FdoSmPhTableP phTable = phDbObject->SmartCast<FdoSmPhTable>();

            if ( phTable ) 
                tableMapping->SetPKeyName( phTable->GetPkeyName() );

            if ( database.GetLength() > 0 && database != pSchema->GetDatabase())
                tableMapping->SetDatabase( database );

            if ( mTableDataDirectory.GetLength() > 0 && mTableDataDirectory != pSchema->GetTableDataDirectory() )
                tableMapping->SetDataDirectory(mTableDataDirectory);

            if ( mTableIndexDirectory.GetLength() > 0 && mTableIndexDirectory != pSchema->GetIndexDataDirectory() )
                tableMapping->SetIndexDirectory(mTableIndexDirectory);

            if ( mTableStorageEngine != MySQLOvStorageEngineType_Default && mTableStorageEngine != StorageEngine_StringToEnum(pSchema->GetTableStorageEngine()) )
                tableMapping->SetStorageEngine(mTableStorageEngine);

            if ( mAutoIncrementPropertyName.GetLength() > 0 )
                mqlClassMapping->SetAutoIncrementPropertyName(mAutoIncrementPropertyName);

            if ( mAutoIncrementSeed != 1 )
                mqlClassMapping->SetAutoIncrementSeed(mAutoIncrementSeed);
        }

        mqlClassMapping->SetTable(tableMapping);

        bHasMappings = true;
    }

    return bHasMappings;
}


void FdoSmLpMySqlClassDefinition::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoMySQLOvTableP tableOverrides;
    FdoMySQLOvClassDefinition* pMySqlOverrides = NULL;

    // Do updates from generic physical mappings
    FdoSmLpGrdClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Extract the class and class table Physical Mapping overrides.

    FdoSmLpSchemaP pSchema = GetLogicalPhysicalSchema();
    // Extract any class overrides
    if ( pClassOverrides  ) {
        pMySqlOverrides = (FdoMySQLOvClassDefinition*) pClassOverrides;
        tableOverrides = pMySqlOverrides->GetTable();
    }

    // Handle generic table overrides
    UpdateTable( L"", tableOverrides ? tableOverrides->GetDatabase() : L"", tableOverrides );

    if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
        // Store MySQL-specific overrides when this object is new or originated
        // from FDO config doc.
        if ( tableOverrides ) {
            mTableDataDirectory = tableOverrides->GetDataDirectory();
            mTableIndexDirectory = tableOverrides->GetIndexDirectory();
            mTableStorageEngine = tableOverrides->GetStorageEngine();
        }

        if ( pMySqlOverrides ) {
            mAutoIncrementPropertyName = pMySqlOverrides->GetAutoIncrementPropertyName();
            mAutoIncrementSeed = pMySqlOverrides->GetAutoIncrementSeed();
        }
    }

    // Note: we ignore any MySQL-specific overrides that have changed;
    // this is consistent with Oracle in that modifying overrides is generally not supported.
}

void FdoSmLpMySqlClassDefinition::SetPhysicalAddWriter( FdoSmPhClassWriterP writer )
{
    FdoSmLpGrdClassDefinition::SetPhysicalAddWriter( writer );

    writer->SetDatabase( GetDatabase() );
    writer->SetOwner( GetOwner() );
}

void FdoSmLpMySqlClassDefinition::Init()
{
    mAutoIncrementSeed = 1;
    mTableStorageEngine = MySQLOvStorageEngineType_Default;
}


FdoSmPhDbObjectP FdoSmLpMySqlClassDefinition::NewTable( 
    FdoString* tableName, 
    FdoString* pkeyName
)
{
    // Let superclass do its job:
    FdoSmPhDbObjectP phDbObject = FdoSmLpClassBase::NewTable(tableName, pkeyName);

    // Pass MySQL-specific overrides to physical object:
    FdoSmPhMySqlTable* phMySqlTable = dynamic_cast<FdoSmPhMySqlTable*>(phDbObject.p);
	GetOverridesFromClass(phMySqlTable);

    // Return it:
    return phDbObject;
}


void FdoSmLpMySqlClassDefinition::PostFinalize()
{
    // Let superclass do it's job:
    FdoSmLpGrdClassDefinition::PostFinalize();

    // Grab overrides from existing table:
    FdoSmPhMySqlDbObjectP dbObj = FDO_SAFE_ADDREF(dynamic_cast<FdoSmPhMySqlDbObject*>(FindPhDbObject().p));
    if (dbObj != NULL)
    {
        mTableStorageEngine  = dbObj->GetStorageEngine();
        mAutoIncrementSeed   = dbObj->GetAutoIncrementColumnSeed();

        // Convert auto-increment column name to property name:
        mAutoIncrementPropertyName = L"";
        FdoString* idColName = dbObj->GetAutoIncrementColumnName();
        if ((idColName != NULL) && (wcslen(idColName) > 0))
        {
            FdoSmLpPropertiesP lpProperties = GetProperties();
            const FdoSmLpDataPropertyDefinition* lpDataProperty = 
                FdoSmLpDataPropertyDefinitionCollection::ColName2Property(lpProperties.p, idColName);
            if (lpDataProperty != NULL)
                mAutoIncrementPropertyName = lpDataProperty->GetName();
        }

        mTableDataDirectory = dbObj->GetDataDirectory();
        mTableIndexDirectory = dbObj->GetIndexDirectory();
    }
}



void FdoSmLpMySqlClassDefinition::GetOverridesFromClass(FdoSmPhMySqlTable* phTable)
{
    phTable->SetAutoIncrementColumnSeed(GetAutoIncrementSeed());
    phTable->SetDataDirectory(GetTableDataDirectory());
    phTable->SetIndexDirectory(GetTableIndexDirectory());
    phTable->SetStorageEngine(GetTableStorageEngine());

    // Convert identity property name to column name:
    FdoString* idColName = L"";
    if (GetAutoIncrementPropertyName().GetLength() > 0)
    {
        FdoSmLpPropertiesP lpProperties = GetProperties();

        FdoPtr<FdoSmLpPropertyDefinition> lpProperty = lpProperties->GetItem(GetAutoIncrementPropertyName());
        FdoSmLpMySqlDataPropertyDefinition* lpMySqlDataProperty = static_cast<FdoSmLpMySqlDataPropertyDefinition*>(lpProperty.p);
        FdoSmPhColumnP col = lpMySqlDataProperty->GetColumn();
        idColName = col->GetName();
    }
    phTable->SetAutoIncrementColumnName(idColName);
}
