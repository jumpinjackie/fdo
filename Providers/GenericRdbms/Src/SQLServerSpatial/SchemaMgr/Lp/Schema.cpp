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
#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>
#include "AssociationPropertyDefinition.h"
#include "DataPropertyDefinition.h"
#include "GeometricPropertyDefinition.h"
#include "ObjectPropertyDefinition.h"
#include "Class.h"
#include "FeatureClass.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>
#include <FdoCommonNlsUtil.h>
#include "../../../Gdbi/GdbiException.h"

static  char  *strNotSupportedExp = "Creating geometric properties from ordinate columns is not supported.";

FdoSmLpSqsSchema::FdoSmLpSqsSchema(FdoSmPhSchemaReaderP rdr,  FdoSmPhMgrP physicalSchema, FdoSmLpSchemaCollection* schemas) :
    FdoSmLpGrdSchema(rdr, physicalSchema, schemas )
{
	SetTableFilegroup(rdr->GetTableStorage());
	SetTextFilegroup(rdr->GetTextStorage());
	SetIndexFilegroup(rdr->GetIndexStorage());
	SetTextInRow(rdr->GetTextInRow());
}

FdoSmLpSqsSchema::FdoSmLpSqsSchema(
	FdoFeatureSchema* pFeatSchema,
    bool bIgnoreStates,
    FdoSmPhMgrP physicalSchema, 
	FdoSmLpSchemaCollection* schemas
) :
    FdoSmLpGrdSchema( pFeatSchema, bIgnoreStates, physicalSchema, schemas )
{
}

FdoSmLpSqsSchema::~FdoSmLpSqsSchema(void)
{
}

FdoPhysicalSchemaMappingP FdoSmLpSqsSchema::GetSchemaMappings( bool bIncludeDefaults ) const
{
    // Create the physical mappings
    FdoSqlServerOvPhysicalSchemaMapping* schemaMapping = FdoSqlServerOvPhysicalSchemaMapping::Create( GetName() );
    FdoPhysicalSchemaMappingP retSchemaMapping = (FdoPhysicalSchemaMapping*) schemaMapping;
    
    // Populate the generic parts
    bool bHasMappings = SetSchemaMappings( retSchemaMapping, bIncludeDefaults );

    if ( wcslen(GetDatabase()) > 0 ) {
        schemaMapping->SetDatabase( GetDatabase() );
        bHasMappings = true;
    }

    if ( wcslen(GetOwner()) > 0 ) {
        schemaMapping->SetOwner( GetOwner() );
        bHasMappings = true;
    }

    // Populate the SqlServer-specific parts.

    if ( wcslen(GetTableFilegroup()) > 0) {
        schemaMapping->SetTableFilegroup( GetTableFilegroup() );
        bHasMappings = true;
    }

    if ( wcslen(GetTextFilegroup()) > 0) {
        schemaMapping->SetTextFilegroup( GetTextFilegroup() );
        bHasMappings = true;
    }

    if ( wcslen(GetIndexFilegroup()) > 0) {
        schemaMapping->SetIndexFilegroup( GetIndexFilegroup() );
        bHasMappings = true;
    }

    if ( TextInRow_StringToEnum(GetTextInRow()) != SqlServerOvTextInRowOption_Default) {
        schemaMapping->SetTextInRow( TextInRow_StringToEnum(GetTextInRow()) );
        bHasMappings = true;
    }

    // return NULL if schema has no mappings.
    if ( !bHasMappings ) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

void FdoSmLpSqsSchema::Update(
    FdoFeatureSchema* pFeatSchema,
	FdoSchemaElementState elementState, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoSqlServerOvPhysicalSchemaMapping* pSqsOverrides = dynamic_cast<FdoSqlServerOvPhysicalSchemaMapping*>(pOverrides);

    // Extract any default physical overrides first since these are used 
    // in parent class's Update.


    if ( pSqsOverrides ) {

        // Store generic mappings:
        if ( wcslen(pSqsOverrides->GetDatabase()) > 0 ) 
            SetDatabase( pSqsOverrides->GetDatabase() );
        if ( wcslen(pSqsOverrides->GetOwner()) > 0 ) 
            SetOwner( pSqsOverrides->GetOwner() );

        // Store SQL Server-specific mappings:
        if ( wcslen(pSqsOverrides->GetTableFilegroup()) > 0 )
            SetTableFilegroup( pSqsOverrides->GetTableFilegroup() );
        if ( wcslen(pSqsOverrides->GetTextFilegroup()) > 0 )
            SetTextFilegroup( pSqsOverrides->GetTextFilegroup() );
        if ( wcslen(pSqsOverrides->GetIndexFilegroup()) > 0 )
            SetIndexFilegroup( pSqsOverrides->GetIndexFilegroup() );
        if ( pSqsOverrides->GetTextInRow() != SqlServerOvTextInRowOption_Default)
            SetTextInRow( TextInRow_EnumToString(pSqsOverrides->GetTextInRow()) );
	}
 
    // Call parent's Update() method next
    FdoSmLpGrdSchema::Update(pFeatSchema, elementState, pOverrides, bIgnoreStates);
}

FdoSmPhSchemaWriterP FdoSmLpSqsSchema::GetPhysicalAddWriter()
{
    FdoSmPhSchemaWriterP pWriter = FdoSmLpGrdSchema::GetPhysicalAddWriter();

    pWriter->SetTableStorage( GetTableFilegroup() );
    pWriter->SetIndexStorage( GetIndexFilegroup() );
    pWriter->SetTextStorage( GetTextFilegroup() );
    pWriter->SetTextInRow( GetTextInRow() );

    return pWriter;
}

FdoSmLpClassDefinitionP FdoSmLpSqsSchema::CreateClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpSqsClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpSqsSchema::CreateFeatureClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpSqsFeatureClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpSqsSchema::CreateFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpSqsFeatureClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpClassDefinitionP FdoSmLpSqsSchema::CreateClass(
    FdoClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpSqsClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateDataProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpSqsDataPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateGeometricProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpSqsGeometricPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateObjectProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpSqsObjectPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateAssociationProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpSqsAssociationPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateDataProperty(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpSqsDataPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpSqsGeometricPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateGeometricProperty(
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

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateObjectProperty(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpSqsObjectPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpSqsSchema::CreateAssociationProperty(
    FdoAssociationPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpSqsAssociationPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}
