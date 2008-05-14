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
#include "ClassDefinition.h"
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"
#include "Schema.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
#include "../Ph/Table.h"
#include "DataPropertyDefinition.h"
#include "Sm/Lp/DataPropertyDefinitionCollection.h"


FdoSmLpSqsClassDefinition::FdoSmLpSqsClassDefinition(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent) : 
	FdoSmLpGrdClassDefinition(classReader, parent),
	FdoSmLpClassBase(classReader, parent)
{
    Init();

    FdoStringP opt = classReader->GetOption( L"indexfilegroup" );
    if ( opt.GetLength() > 0 )
        mTableIndexFilegroup = opt;

    opt = classReader->GetOption( L"tablefilegroup" );
    if ( opt.GetLength() > 0 )
        mTableFilegroup = opt;

    opt = classReader->GetOption( L"textfilegroup" );
    if ( opt.GetLength() > 0 )
        mTableTextFilegroup = opt;

    opt = classReader->GetOption( L"textinrow" );
    if ( opt.GetLength() > 0 )
        mTextInRow = TextInRow_StringToEnum( opt );

    opt = classReader->GetOption( L"identitypropertyname" );
    if ( opt.GetLength() > 0 )
        mIdentityPropertyName = opt;

    opt = classReader->GetOption( L"identityincrement" );
    if ( opt.GetLength() > 0 )
        mIdentityIncrement = opt.ToLong();

    opt = classReader->GetOption( L"identityseed" );
    if ( opt.GetLength() > 0 )
        mIdentitySeed = opt.ToLong();
         
    opt = classReader->GetOption( L"identityisgloballyunique" );
    if ( opt.GetLength() > 0 )
        mIdentityIsGloballyUnique = opt.ToBoolean();
}

FdoSmLpSqsClassDefinition::FdoSmLpSqsClassDefinition(
    FdoClassDefinition* pFdoClass, 
    bool bIgnoreStates,
    FdoSmLpSchemaElement* parent
) : 
	FdoSmLpGrdClassDefinition(pFdoClass, bIgnoreStates, parent),
	FdoSmLpClassBase(pFdoClass, bIgnoreStates, parent)
{
    Init();
}

FdoSmLpSqsClassDefinition::FdoSmLpSqsClassDefinition(
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


FdoSmLpSqsClassDefinition::~FdoSmLpSqsClassDefinition(void)
{
}

FdoStringP FdoSmLpSqsClassDefinition::DefaultDbObjectName()
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhOwnerP pOwner = pPhysical->GetOwner();
    bool hasMetaSchema = pOwner ? pOwner->GetHasMetaSchema() : false;

    // Qualify default db object name by user.

    if ( hasMetaSchema ) 
        // When datastore has MetaSchema, default user is dbo.
        return FdoStringP(L"dbo.") + GetName();
    else
        // Otherwise, default user is the feature schema name.
        return FdoStringP(GetLogicalPhysicalSchema()->GetName()) + L"." + GetName();
}

bool FdoSmLpSqsClassDefinition::AddSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const
{
    bool bHasMappings = false;
    FdoSqlServerOvPhysicalSchemaMapping* sqsSchemaMapping = (FdoSqlServerOvPhysicalSchemaMapping*)(FdoPhysicalSchemaMapping*) schemaMapping;

    // Get the physical mappings for this class
    FdoPhysicalClassMappingP classMapping = GetSchemaMappings( bIncludeDefaults );
    FdoSqlServerOvClassDefinition* sqsClassMapping = 
        dynamic_cast<FdoSqlServerOvClassDefinition*>(classMapping.p);

    // Add them to the schema override set.
    if ( sqsClassMapping ) {
        FdoSqlServerOvClassesP(sqsSchemaMapping->GetClasses())->Add( sqsClassMapping );
        bHasMappings = true;
    }

    return bHasMappings;
}

FdoPhysicalClassMappingP FdoSmLpSqsClassDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    FdoSqlServerOvClassDefinition*     classMapping = FdoSqlServerOvClassDefinition::Create( GetName() );
    FdoPhysicalClassMappingP        retClassMapping = (FdoSqlServerOvClassDefinition*) classMapping;

    // Set class type specific mappings.
    bool bHasMappings = SetSchemaMappings( retClassMapping, bIncludeDefaults );

    // return NULL if there are no mappings. This prevents generating empty objects.
    if ( !bHasMappings ) 
        retClassMapping = NULL;

    return retClassMapping;
    
}
bool FdoSmLpSqsClassDefinition::SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const
{
    ((FdoSmLpSqsClassDefinition*)this)->Finalize();

    const FdoSmLpSqsSchema*         pSchema = (const FdoSmLpSqsSchema*) RefLogicalPhysicalSchema();
    FdoSqlServerOvClassDefinition*      sqsClassMapping = dynamic_cast<FdoSqlServerOvClassDefinition*>( (FdoPhysicalClassMapping*) classMapping );

    // Default the table mapping to the one for the Feature Schema.
    FdoSmOvTableMappingType      defaultTableMappingType = pSchema->GetTableMapping();
    FdoSmOvTableMappingType      tableMappingType = GetTableMapping();
    FdoStringP                      database = GetDatabase();
    FdoStringP                      owner = GetOwner();
    FdoSmLpDbObjectP                dbObject = ((FdoSmLpSqsClassDefinition*)this)->GetDbObject();
    bool                            bHasMappings = FdoSmLpGrdClassDefinition::SetSchemaMappings( FDO_SAFE_ADDREF(sqsClassMapping), bIncludeDefaults );

    // Set the table physical mappings of including default mappings or there are any
    // non-default schema mappings.
    if ( bIncludeDefaults || 
         ((tableMappingType != FdoSmOvTableMappingType_BaseTable) && GetIsFixedDbObject()) ||
         (database.GetLength() > 0 && database != pSchema->GetDatabase()) ||
         (owner.GetLength() > 0 && owner != pSchema->GetOwner()) ||
         (mTableIndexFilegroup.GetLength() > 0 && mTableIndexFilegroup != pSchema->GetIndexFilegroup()) ||
         (mTableFilegroup.GetLength() > 0 && mTableFilegroup != pSchema->GetTableFilegroup()) ||
         (mTableTextFilegroup.GetLength() > 0 && mTableTextFilegroup != pSchema->GetTextFilegroup()) ||
         (mTextInRow != SqlServerOvTextInRowOption_Default) ||
         (mIdentityPropertyName.GetLength() > 0) ||
         (mIdentityIncrement != 1) ||
         (mIdentitySeed != 0) ||
         (mIdentityIsGloballyUnique != false)
    ) {
        // For foreign schemas override table name is the root (foreign) table.
        FdoSqlServerOvTableP tableMapping = FdoSqlServerOvTable::Create( 
            wcslen(GetRootDbObjectName()) > 0 ?
                GetRootDbObjectName() :
                GetDbObjectName()    
        );
        
        if ( dbObject ) {
            FdoSmPhDbObjectP phDbObject = dbObject->GetDbObject();
            FdoSmPhTableP phTable = phDbObject->SmartCast<FdoSmPhTable>();

            // Get generic mappings:

            if ( phTable ) 
                tableMapping->SetPKeyName( phTable->GetPkeyName() );

            if ( database.GetLength() > 0 && database != pSchema->GetDatabase())
                tableMapping->SetDatabase( database );

            if ( owner.GetLength() > 0 && owner != pSchema->GetOwner())
                tableMapping->SetOwner( owner );


            // Get SQL Server-specific mappings:

            if (mTableIndexFilegroup.GetLength() > 0 && mTableIndexFilegroup != pSchema->GetIndexFilegroup())
                tableMapping->SetIndexFilegroup(mTableIndexFilegroup);

            if (mTableFilegroup.GetLength() > 0 && mTableFilegroup != pSchema->GetTableFilegroup())
                tableMapping->SetTableFilegroup(mTableFilegroup);

            if (mTableTextFilegroup.GetLength() > 0 && mTableTextFilegroup != pSchema->GetTextFilegroup())
                tableMapping->SetTextFilegroup(mTableTextFilegroup);

            if (mTextInRow != SqlServerOvTextInRowOption_Default)
                tableMapping->SetTextInRow(mTextInRow);

            if (mIdentityPropertyName.GetLength() > 0)
                sqsClassMapping->SetIdentityPropertyName(mIdentityPropertyName);

            if (mIdentityIncrement != 1)
                sqsClassMapping->SetIdentityIncrement(mIdentityIncrement);

            if (mIdentitySeed != 0)
                sqsClassMapping->SetIdentitySeed(mIdentitySeed);

            if (mIdentityIsGloballyUnique != false)
                sqsClassMapping->SetIdentityIsGloballyUnique(mIdentityIsGloballyUnique);
        }

        sqsClassMapping->SetTable(tableMapping);
        bHasMappings = true;
    }

    return bHasMappings;
}


void FdoSmLpSqsClassDefinition::Update(
    FdoClassDefinition* pFdoClass,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    FdoStringP ovTableName;

    // Do updates from generic physical mappings
    FdoSmLpGrdClassDefinition::Update( pFdoClass, elementState, pClassOverrides, bIgnoreStates );

    // Extract the class and class table Physical Mapping overrides.

    FdoSmLpSchemaP pSchema = GetLogicalPhysicalSchema();

    // Extract any class overrides
    FdoSqlServerOvClassDefinition* pSqsOverrides = dynamic_cast<FdoSqlServerOvClassDefinition*>(pClassOverrides);
    FdoSqlServerOvTableP tableOverrides = pSqsOverrides ? pSqsOverrides->GetTable() : NULL;

    // Handle generic table overrides
    UpdateTable( 
        tableOverrides ? tableOverrides->GetDatabase() : L"",
        tableOverrides ? tableOverrides->GetOwner() : L"",
        tableOverrides 
    );

    if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
        // Store SQL Server-specific overrides when this object is new or originated
        // from FDO config doc.
         if ( tableOverrides ) {
            mTableIndexFilegroup = tableOverrides->GetIndexFilegroup();
            mTableFilegroup = tableOverrides->GetTableFilegroup();
            mTableTextFilegroup = tableOverrides->GetTextFilegroup();
            mTextInRow = tableOverrides->GetTextInRow();
        }

         if ( pSqsOverrides ) {
            mIdentityPropertyName = pSqsOverrides->GetIdentityPropertyName();
            mIdentityIncrement = pSqsOverrides->GetIdentityIncrement();
            mIdentityIsGloballyUnique = pSqsOverrides->GetIdentityIsGloballyUnique();
            mIdentitySeed = pSqsOverrides->GetIdentitySeed();
        }
    }
    //TODO: do we support modifying any of the SQL Server-specific overrides (filegroups, etc) ?
}

FdoStringP FdoSmLpSqsClassDefinition::GetSubstDbObjectName( FdoStringP dbObjectName ) const
{
    FdoStringP substObjName = FdoSmLpClassBase::GetSubstDbObjectName( dbObjectName );

    if ( substObjName.Contains(L".") )
        return substObjName.Right(L".");
    else
        return substObjName;
}

FdoStringP FdoSmLpSqsClassDefinition::GetSubstRootDbObjectName() const
{
    FdoStringP substObjName = FdoSmLpClassBase::GetSubstRootDbObjectName();

    if ( substObjName.Contains(L".") )
        return substObjName.Right(L".");
    else
        return substObjName;
}


void FdoSmLpSqsClassDefinition::SetPhysicalAddWriter( FdoSmPhClassWriterP writer )
{
    FdoSmLpGrdClassDefinition::SetPhysicalAddWriter( writer );

    writer->SetDatabase( GetDatabase() );
    writer->SetOwner( GetOwner() );
}

void FdoSmLpSqsClassDefinition::SetPhysicalOptionsWriter( FdoSmPhClassWriterP classWriter )
{
    FdoSmLpClassBase::SetPhysicalOptionsWriter( classWriter );

    if ( mTableIndexFilegroup.GetLength() > 0 )
        classWriter->SetOption( L"indexfilegroup", mTableIndexFilegroup );

    if ( mTableFilegroup.GetLength() > 0 )
        classWriter->SetOption( L"tablefilegroup", mTableFilegroup );

    if ( mTableTextFilegroup.GetLength() > 0 )
        classWriter->SetOption( L"textfilegroup", mTableTextFilegroup );

    if ( mTextInRow != SqlServerOvTextInRowOption_Default )
        classWriter->SetOption( L"textinrow", TextInRow_EnumToString(mTextInRow) );

    if ( mIdentityPropertyName.GetLength() > 0 )
        classWriter->SetOption( L"identitypropertyname", mIdentityPropertyName );

    if ( mIdentityIncrement != 1 )
        classWriter->SetOption( L"identityincrement", FdoStringP::Format(L"%d",mIdentityIncrement) );

    if ( mIdentitySeed != 0 )
        classWriter->SetOption( L"identityseed", FdoStringP::Format(L"%d",mIdentitySeed) );
         
    if ( mIdentityIsGloballyUnique != false )
        classWriter->SetOption( L"identityisgloballyunique", L"true" );
}

void FdoSmLpSqsClassDefinition::Init()
{
    mTextInRow = SqlServerOvTextInRowOption_Default;
    mIdentityIncrement = 1;
    mIdentitySeed = 0;
    mIdentityIsGloballyUnique = false;
}

void FdoSmLpSqsClassDefinition::SetDbObjectName( FdoStringP objectName )
{
    FdoSmLpClassBase::SetDbObjectName(
        ((objectName.GetLength() == 0) || objectName.Contains(L".")) ?
            objectName :
            FdoStringP(L"dbo.") + objectName
    );
}

FdoSmPhDbObjectP FdoSmLpSqsClassDefinition::NewTable( 
    FdoString* tableName, 
    FdoString* pkeyName
)
{
    // Let superclass do its job:
    FdoSmPhDbObjectP phDbObject = FdoSmLpClassBase::NewTable(tableName, pkeyName);

    // Pass SQL Server-specific overrides to physical object:
    FdoSmPhSqsTable* phSqsTable = dynamic_cast<FdoSmPhSqsTable*>(phDbObject.p);
	GetOverridesFromClass(phSqsTable);

    // Return it:
    return phDbObject;
}

void FdoSmLpSqsClassDefinition::PostFinalize()
{
    FdoSmLpGrdClassDefinition::PostFinalize();

    // Grab overrides from existing table:
    FdoSmPhSqsDbObjectP dbObj = FDO_SAFE_ADDREF(dynamic_cast<FdoSmPhSqsDbObject*>(FindPhDbObject().p));
    if ( (dbObj != NULL) && (dbObj->GetElementState() != FdoSchemaElementState_Added) )
    {
        mTableFilegroup      = dbObj->GetTableFilegroup();
        mTableTextFilegroup  = dbObj->GetTextFilegroup();
        mTableIndexFilegroup = dbObj->GetIndexFilegroup();

        // Convert identity column name to property name:
        mIdentityPropertyName = L"";
        FdoString* idColName = dbObj->GetIdentityColumn();
        if ((idColName != NULL) && (wcslen(idColName) > 0))
        {
            FdoSmLpPropertiesP lpProperties = GetProperties();
            const FdoSmLpDataPropertyDefinition* lpDataProperty = 
                FdoSmLpDataPropertyDefinitionCollection::ColName2Property(lpProperties.p, idColName);
            if (lpDataProperty != NULL)
                mIdentityPropertyName = lpDataProperty->GetName();
        }

        mIdentitySeed = dbObj->GetIdentitySeed();
        mIdentityIncrement = dbObj->GetIdentityIncrement();
        mIdentityIsGloballyUnique = dbObj->GetIdentityIsGloballyUnique();
        mTextInRow = dbObj->GetTextInRowOption();
    }
}



void FdoSmLpSqsClassDefinition::GetOverridesFromClass(FdoSmPhSqsTable* phTable)
{
	if (phTable != NULL)
	{
		phTable->SetTableFilegroup(GetTableFilegroup());
		phTable->SetTextFilegroup(GetTableTextFilegroup());
		phTable->SetIndexFilegroup(GetTableIndexFilegroup());
		phTable->SetTextInRowOption(GetTextInRow());

		// Convert identity property name to column name:
		FdoString* idColName = L"";
		if (GetIdentityPropertyName().GetLength() > 0)
		{
			FdoSmLpPropertiesP lpProperties = GetProperties();

			FdoPtr<FdoSmLpPropertyDefinition> lpProperty = lpProperties->GetItem(GetIdentityPropertyName());
			FdoSmLpSqsDataPropertyDefinition* lpSqsDataProperty = static_cast<FdoSmLpSqsDataPropertyDefinition*>(lpProperty.p);
			FdoSmPhColumnP col = lpSqsDataProperty->GetColumn();
			idColName = col->GetName();
		}
		phTable->SetIdentityColumn(idColName);
		phTable->SetIdentitySeed(mIdentitySeed);
		phTable->SetIdentityIncrement(mIdentityIncrement);
		phTable->SetIdentityIsGloballyUnique(mIdentityIsGloballyUnique);
	}
}
