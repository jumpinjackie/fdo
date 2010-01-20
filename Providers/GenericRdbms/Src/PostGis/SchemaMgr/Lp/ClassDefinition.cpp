//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ClassDefinition.h"
#include "Utilities/SchemaMgr/Overrides/TableMappingTypeMapper.h"
#include "Schema.h"
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>
#include "../Ph/DbObject.h"
#include "../Ph/Table.h"
#include "DataPropertyDefinition.h"

FdoSmLpPostGisClassDefinition::FdoSmLpPostGisClassDefinition(
    FdoSmPhClassReaderP classReader,
    FdoSmLpSchemaElement* parent)
    :
    FdoSmLpGrdClassDefinition(classReader, parent),
    FdoSmLpClassBase(classReader, parent)
{
}

FdoSmLpPostGisClassDefinition::FdoSmLpPostGisClassDefinition(
    FdoClassDefinition* fdoClass,
    bool ignoreStates,
    FdoSmLpSchemaElement* parent)
    :
    FdoSmLpGrdClassDefinition(fdoClass, ignoreStates, parent),
	FdoSmLpClassBase(fdoClass, ignoreStates, parent)
{
}

FdoSmLpPostGisClassDefinition::FdoSmLpPostGisClassDefinition(FdoString* name,
	FdoString* description,
	FdoSmLpSchemaElement* parent,
	FdoSmLpClassDefinitionP srcClass,
	FdoSmOvTableMappingType tableMapping,
	FdoString* tableName,
	FdoSmPhDbObjectP table,
	FdoSchemaElementState state)
    :
    FdoSmLpGrdClassDefinition(name, description, parent, srcClass,
        tableMapping, tableName, table, state),
	FdoSmLpClassBase(name, description, parent, srcClass, tableMapping,
        tableName, table, state, parent->GetIsFromFdo())
{
}

FdoSmLpPostGisClassDefinition::~FdoSmLpPostGisClassDefinition()
{
    // idle
}

FdoStringP FdoSmLpPostGisClassDefinition::DefaultDbObjectName()
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhOwnerP pOwner = pPhysical->GetOwner();
    bool hasMetaSchema = pOwner ? pOwner->GetHasMetaSchema() : false;

    // Qualify default db object name by user.

    if ( hasMetaSchema ) 
        // When datastore has MetaSchema, default user is dbo.
        return FdoStringP(L"public.") + GetName();
    else
        // Otherwise, default user is the feature schema name.
        return FdoStringP(GetLogicalPhysicalSchema()->GetName()) + L"." + GetName();
}

FdoStringP FdoSmLpPostGisClassDefinition::GetSubstDbObjectName( FdoStringP dbObjectName ) const
{
    FdoStringP substObjName = FdoSmLpClassBase::GetSubstDbObjectName( dbObjectName );

    if ( substObjName.Contains(L".") )
        return substObjName.Right(L".");
    else
        return substObjName;
}

FdoStringP FdoSmLpPostGisClassDefinition::GetSubstRootDbObjectName() const
{
    FdoStringP substObjName = FdoSmLpClassBase::GetSubstRootDbObjectName();

    if ( substObjName.Contains(L".") )
        return substObjName.Right(L".");
    else
        return substObjName;
}

bool FdoSmLpPostGisClassDefinition::AddSchemaMappings(
    FdoPhysicalSchemaMappingP schemaMapping, bool includeDefaults) const
{
    bool hasMappings = false;

    FdoPostGISOvPhysicalSchemaMapping* pgSchemaMapping = NULL;
    pgSchemaMapping = 
        static_cast<FdoPostGISOvPhysicalSchemaMapping*>(schemaMapping.p);

    // Get the physical mappings for this class
    FdoPhysicalClassMappingP classMapping(GetSchemaMappings(includeDefaults));
    
    FdoPostGISOvClassDefinition* pgClassMapping = NULL;
    pgClassMapping = 
        dynamic_cast<FdoPostGISOvClassDefinition*>(classMapping.p);

    // Add them to the schema override set.
    if (pgClassMapping)
    {
        FdoPostGISOvClassesP classes(pgSchemaMapping->GetClasses());
        classes->Add(pgClassMapping);
        hasMappings = true;
    }

    return hasMappings;
}

FdoPhysicalClassMappingP FdoSmLpPostGisClassDefinition::GetSchemaMappings(
    bool includeDefaults) const
{
    FdoPostGISOvClassDefinition* classMapping = NULL;
    classMapping = FdoPostGISOvClassDefinition::Create(GetName());

    FdoPhysicalClassMappingP retClassMapping(classMapping);

    // Set class type specific mappings.
    bool hasMappings = SetSchemaMappings(retClassMapping, includeDefaults);

    // NULL indicates there are no mappings. This prevents generating
    // empty objects.
    if (!hasMappings) 
    {
        retClassMapping = NULL;
    }

    return retClassMapping;
}

bool FdoSmLpPostGisClassDefinition::SetSchemaMappings(
    FdoPhysicalClassMappingP classMapping, bool includeDefaults) const
{
    FdoSmLpPostGisClassDefinition* thisClassDef = NULL;
    thisClassDef = const_cast<FdoSmLpPostGisClassDefinition*>(this);
    thisClassDef->Finalize();

    const FdoSmLpPostGisSchema* schema = 
        static_cast<const FdoSmLpPostGisSchema*>(RefLogicalPhysicalSchema());

    FdoPostGISOvClassDefinition* pgClassMapping = NULL;
    pgClassMapping = dynamic_cast<FdoPostGISOvClassDefinition*>(classMapping.p);
 
    // Default the table mapping to the one for the Feature Schema.
    FdoSmOvTableMappingType defaultTableMapping(schema->GetTableMapping());
    FdoSmOvTableMappingType tableMapping(GetTableMapping());
    FdoStringP owner(GetOwner());

    thisClassDef = const_cast<FdoSmLpPostGisClassDefinition*>(this);
    FdoSmLpDbObjectP dbObject(thisClassDef->GetDbObject());

    bool hasMappings = FdoSmLpGrdClassDefinition::SetSchemaMappings(
        classMapping, includeDefaults);

    // Set the table physical mappings of including default mappings or there are any
    // non-default schema mappings.
    
    if (includeDefaults
        || ((FdoSmOvTableMappingType_BaseTable != tableMapping)
            && GetIsFixedDbObject())
        || (owner.GetLength() > 0 && schema->GetOwner() != owner)
    ) {
        // For foreign schemas override table name is the root (foreign) table.
     
        FdoString* tableName = NULL;
        
        if (wcslen(GetRootDbObjectName()) > 0)
            tableName = GetRootDbObjectName();
        else
            tableName = GetDbObjectName(); 
            
        FdoPostGISOvTableP tableMapping(FdoPostGISOvTable::Create(tableName));
        
        if (NULL != dbObject)
        {
            FdoSmPhDbObjectP phDbObject(dbObject->GetDbObject());
            tableMapping->SetPKeyName(dbObject->GetPkeyName());

            if (owner.GetLength() > 0 && schema->GetOwner() != owner)
                tableMapping->SetOwner(owner);
        }

        pgClassMapping->SetTable(tableMapping);

        hasMappings = true;
    }

    return hasMappings;
}

void FdoSmLpPostGisClassDefinition::Update(FdoClassDefinition* fdoClass,
    FdoSchemaElementState state,
    FdoPhysicalClassMapping* classOverrides,
    bool ignoreStates
)
{
    FdoPostGISOvTableP tableOverrides;
    FdoPostGISOvClassDefinition* pgOverrides = NULL;

    // Do updates from generic physical mappings
    FdoSmLpGrdClassDefinition::Update(fdoClass, state, classOverrides, ignoreStates);

    // Extract the class and class table Physical Mapping overrides.

    FdoSmLpSchemaP schema(GetLogicalPhysicalSchema());

    // Extract any class overrides
    if (classOverrides)
    {
        pgOverrides = 
            static_cast<FdoPostGISOvClassDefinition*>(classOverrides);
        tableOverrides = pgOverrides->GetTable();
    }

    // Handle generic table overrides
    UpdateTable(L"",
        (tableOverrides ? tableOverrides->GetOwner() : L""),
        tableOverrides);
}

void FdoSmLpPostGisClassDefinition::SetPhysicalAddWriter(
    FdoSmPhClassWriterP writer)
{
    FdoSmLpGrdClassDefinition::SetPhysicalAddWriter(writer);

    writer->SetDatabase(GetDatabase());
    writer->SetOwner(GetOwner());
}

FdoSmPhDbObjectP FdoSmLpPostGisClassDefinition::NewTable( 
    FdoString* tableName, 
    FdoString* keyName)
{
    // Let superclass do its job
    FdoSmPhDbObjectP phDbObject(FdoSmLpClassBase::NewTable(tableName, keyName));

    if ( this->Get_TableMapping() == FdoSmOvTableMappingType_ConcreteTable ) {
        FdoSmLpClassDefinitionP baseClass = this->GetBaseClass();

        if ( baseClass ) {
            const FdoSmLpDbObject* lpBaseDbObject = baseClass->RefDbObject();
            if ( lpBaseDbObject ) {
                FdoSmPhDbObjectP phBaseDbObject = FDO_SAFE_ADDREF( (FdoSmPhDbObject*)(lpBaseDbObject->RefDbObject()));
                if ( phBaseDbObject ) {
                    FdoSmPhTableP baseTable = phBaseDbObject->SmartCast<FdoSmPhTable>();

                    if ( baseTable ) {
                        FdoSmPhBaseObjectsP baseObjects = phDbObject->GetBaseObjects();

                        if ( baseObjects->GetCount() == 0 ) {
                            FdoSmPhBaseObjectP baseObject = new FdoSmPhBaseObject(phBaseDbObject, phDbObject);
                            baseObjects->Add( baseObject );
                        }
                    }
                }
            }
        }
    }

    // Return it:
    return phDbObject;
}

