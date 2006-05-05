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
#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>
#include "AssociationPropertyDefinition.h"
#include "DataPropertyDefinition.h"
#include "GeometricPropertyDefinition.h"
#include "ObjectPropertyDefinition.h"
#include "Class.h"
#include "FeatureClass.h"
#include <Rdbms/Override/MySQL/MySqlOvStorageEngineType.h>
#include <FdoCommonNlsUtil.h>
#include "../../../Gdbi/GdbiException.h"

static  char  *strNotSupportedExp = "Creating geometric properties from ordinate columns is not supported.";

FdoSmLpMySqlSchema::FdoSmLpMySqlSchema(FdoSmPhSchemaReaderP rdr,  FdoSmPhMgrP physicalSchema, FdoSmLpSchemaCollection* schemas) :
    FdoSmLpGrdSchema(rdr, physicalSchema, schemas )
{
	SetTableDataDirectory(rdr->GetTableStorage());
	SetIndexDataDirectory(rdr->GetIndexStorage());
	SetTableStorageEngine(rdr->GetTableStorageEngine());
}

FdoSmLpMySqlSchema::FdoSmLpMySqlSchema(
	FdoFeatureSchema* pFeatSchema,
    bool bIgnoreStates,
    FdoSmPhMgrP physicalSchema, 
	FdoSmLpSchemaCollection* schemas
) :
    FdoSmLpGrdSchema( pFeatSchema, bIgnoreStates, physicalSchema, schemas )
{
}

FdoSmLpMySqlSchema::~FdoSmLpMySqlSchema(void)
{
}

FdoPhysicalSchemaMappingP FdoSmLpMySqlSchema::GetSchemaMappings( bool bIncludeDefaults ) const
{
    // Create the physical mappings
    FdoMySQLOvPhysicalSchemaMapping* schemaMapping = FdoMySQLOvPhysicalSchemaMapping::Create( GetName() );
    FdoPhysicalSchemaMappingP retSchemaMapping = (FdoPhysicalSchemaMapping*) schemaMapping;
    
    // Populate the generic parts
    bool bHasMappings = SetSchemaMappings( retSchemaMapping, bIncludeDefaults );

    // Populate the MySQL-specific parts.

    if ( wcslen(GetDatabase()) > 0 ) {
        schemaMapping->SetDatabase( GetDatabase() );
        bHasMappings = true;
    }

    if ( wcslen( GetTableDataDirectory()) > 0 ) {
        schemaMapping->SetDataDirectory( GetTableDataDirectory() );
        bHasMappings = true;
    }

    if ( wcslen (GetIndexDataDirectory()) > 0 ) {
        schemaMapping->SetIndexDirectory( GetIndexDataDirectory() );
        bHasMappings = true;
    }

    if ( StorageEngine_StringToEnum(GetTableStorageEngine()) != MySQLOvStorageEngineType_Default ) {
        schemaMapping->SetStorageEngine( StorageEngine_StringToEnum(GetTableStorageEngine()) );
        bHasMappings = true;
    }


    // return NULL if schema has no mappings.
    if ( !bHasMappings ) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

void FdoSmLpMySqlSchema::Update(
    FdoFeatureSchema* pFeatSchema,
	FdoSchemaElementState elementState, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoMySQLOvPhysicalSchemaMapping* pMySqlOverrides = dynamic_cast<FdoMySQLOvPhysicalSchemaMapping*>(pOverrides);

    // Extract any default physical overrides first since these are used 
    // in parent class's Update.

	if ( pMySqlOverrides ) {

        if ( wcslen(pMySqlOverrides->GetDatabase()) > 0 ) 
            SetDatabase( pMySqlOverrides->GetDatabase() );

        if ( wcslen(pMySqlOverrides->GetDataDirectory()) > 0 ) 
            SetTableDataDirectory ( pMySqlOverrides->GetDataDirectory() );

        if ( wcslen(pMySqlOverrides->GetIndexDirectory ()) > 0 ) 
            SetIndexDataDirectory ( pMySqlOverrides->GetIndexDirectory() );

        if ( pMySqlOverrides->GetStorageEngine() != MySQLOvStorageEngineType_Default ) 
            SetTableStorageEngine ( StorageEngine_EnumToString(pMySqlOverrides->GetStorageEngine() ) );
    }

    // Do parent's Update() next:
    FdoSmLpGrdSchema::Update(pFeatSchema, elementState, pOverrides, bIgnoreStates);
}

FdoSmPhSchemaWriterP FdoSmLpMySqlSchema::GetPhysicalAddWriter()
{
    FdoSmPhSchemaWriterP pWriter = FdoSmLpGrdSchema::GetPhysicalAddWriter();

	pWriter->SetTableStorage( GetTableDataDirectory() );
    pWriter->SetIndexStorage( GetIndexDataDirectory() );
    pWriter->SetTableStorageEngine( GetTableStorageEngine() );

    return pWriter;
}

FdoSmLpClassDefinitionP FdoSmLpMySqlSchema::CreateClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpMySqlClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpMySqlSchema::CreateFeatureClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpMySqlFeatureClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpMySqlSchema::CreateFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpMySqlFeatureClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpClassDefinitionP FdoSmLpMySqlSchema::CreateClass(
    FdoClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpMySqlClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateDataProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpMySqlDataPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateGeometricProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpMySqlGeometricPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateObjectProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpMySqlObjectPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateAssociationProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpMySqlAssociationPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateDataProperty(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpMySqlDataPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpMySqlGeometricPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* pFdoProp, 
    FdoString * columnNameX,
    FdoString * columnNameY,
    FdoString * columnNameZ,
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_467, strNotSupportedExp));
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateObjectProperty(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpMySqlObjectPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpMySqlSchema::CreateAssociationProperty(
    FdoAssociationPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpMySqlAssociationPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}
