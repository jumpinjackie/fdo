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
#include <FdoRdbmsUtil.h>
#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>
#include "DataPropertyDefinition.h"
#include "GeometricPropertyDefinition.h"
#include "Class.h"
#include "FeatureClass.h"
#include <Rdbms/Override/ODBC/OdbcOvPhysicalSchemaMapping.h>

FdoSmLpOdbcSchema::FdoSmLpOdbcSchema(FdoSmPhSchemaReaderP rdr,  FdoSmPhMgrP physicalSchema, FdoSmLpSchemaCollection* schemas) :
    FdoSmLpGrdSchema(rdr, physicalSchema, schemas )
{
}

FdoSmLpOdbcSchema::FdoSmLpOdbcSchema(
	FdoFeatureSchema* pFeatSchema,
    bool bIgnoreStates,
    FdoSmPhMgrP physicalSchema, 
	FdoSmLpSchemaCollection* schemas
) :
    FdoSmLpGrdSchema( pFeatSchema, bIgnoreStates, physicalSchema, schemas )
{
    SetOwner(physicalSchema->GetOwner()->GetName());
}

FdoSmLpOdbcSchema::~FdoSmLpOdbcSchema(void)
{
}

FdoPhysicalSchemaMappingP FdoSmLpOdbcSchema::GetSchemaMappings( bool bIncludeDefaults ) const
{
    // Create the physical mappings
    FdoOdbcOvPhysicalSchemaMapping* schemaMapping = FdoOdbcOvPhysicalSchemaMapping::Create( GetName() );
    FdoPhysicalSchemaMappingP retSchemaMapping = (FdoPhysicalSchemaMapping*) schemaMapping;
    
    // Populate the generic parts
    bool bHasMappings = SetSchemaMappings( retSchemaMapping, bIncludeDefaults );

    // return NULL if schema has no mappings.
    if ( !bHasMappings ) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

void FdoSmLpOdbcSchema::Update(
    FdoFeatureSchema* pFeatSchema,
	FdoSchemaElementState elementState, 
    FdoPhysicalSchemaMapping* pOverrides,
    bool bIgnoreStates
)
{
    FdoOdbcOvPhysicalSchemaMapping* pOdbcOverrides = dynamic_cast<FdoOdbcOvPhysicalSchemaMapping*>(pOverrides);

    // Call parent's Update() method next
    FdoSmLpGrdSchema::Update(pFeatSchema, elementState, pOverrides, bIgnoreStates);
}

FdoSmPhSchemaWriterP FdoSmLpOdbcSchema::GetPhysicalAddWriter()
{
    FdoSmPhSchemaWriterP pWriter = FdoSmLpGrdSchema::GetPhysicalAddWriter();

    return pWriter;
}

FdoSmLpClassDefinitionP FdoSmLpOdbcSchema::CreateClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpOdbcClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpOdbcSchema::CreateFeatureClass(FdoSmPhClassReaderP classReader)
{
    return new FdoSmLpOdbcFeatureClass( classReader, this );
}

FdoSmLpClassDefinitionP FdoSmLpOdbcSchema::CreateFeatureClass(
    FdoFeatureClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpOdbcFeatureClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpClassDefinitionP FdoSmLpOdbcSchema::CreateClass(
    FdoClass* pFdoClass, 
    bool bIgnoreStates
)
{
    return new FdoSmLpOdbcClass( pFdoClass, bIgnoreStates, this );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateDataProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpOdbcDataPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateGeometricProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    return new FdoSmLpOdbcGeometricPropertyDefinition( propReader, parent );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateObjectProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_454, "Object properties are not supported."));
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateAssociationProperty(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent )
{
    throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_455, "Association properties are not supported."));
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateDataProperty(
    FdoDataPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpOdbcDataPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpOdbcGeometricPropertyDefinition( pFdoProp, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* pFdoProp, 
    FdoString * columnNameX,
    FdoString * columnNameY,
    FdoString * columnNameZ,
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    return new FdoSmLpOdbcGeometricPropertyDefinition( pFdoProp, columnNameX, columnNameY, columnNameZ, bIgnoreStates, parent );
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateObjectProperty(
    FdoObjectPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_454, "Object properties are not supported."));
}

FdoSmLpPropertyP FdoSmLpOdbcSchema::CreateAssociationProperty(
    FdoAssociationPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
)
{
    throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_455, "Association properties are not supported."));
}
