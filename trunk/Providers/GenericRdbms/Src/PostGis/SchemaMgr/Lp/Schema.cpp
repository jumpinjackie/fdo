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
#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>
#include "AssociationPropertyDefinition.h"
#include "DataPropertyDefinition.h"
#include "GeometricPropertyDefinition.h"
#include "ObjectPropertyDefinition.h"
#include "Class.h"
#include "FeatureClass.h"
#include <FdoCommonNlsUtil.h>
#include "../../../Gdbi/GdbiException.h"
#include "../Ph/Owner.h"

// TODO: PostGIS supports this feature, so to be implemented.
static char* msgNotSupportedExp = 
    "Creating geometric properties from ordinate columns is not supported.";

FdoSmLpPostGisSchema::FdoSmLpPostGisSchema(FdoSmPhSchemaReaderP reader,
    FdoSmPhMgrP mgr, FdoSmLpSchemaCollection* schemas)
    : FdoSmLpGrdSchema(reader, mgr, schemas)
{
}

FdoSmLpPostGisSchema::FdoSmLpPostGisSchema(FdoFeatureSchema* featSchema,
    bool ignoreStates,
    FdoSmPhMgrP mgr, 
	FdoSmLpSchemaCollection* schemas)
    : FdoSmLpGrdSchema(featSchema, ignoreStates, mgr, schemas)
{
    // idle
}

FdoSmLpPostGisSchema::~FdoSmLpPostGisSchema()
{
    // idle
}

FdoPhysicalSchemaMappingP FdoSmLpPostGisSchema::GetSchemaMappings(
    bool includeDefaults) const
{
    // Create the physical mappings
    FdoPostGISOvPhysicalSchemaMapping* schemaMapping = NULL;
    schemaMapping = FdoPostGISOvPhysicalSchemaMapping::Create(GetName());

    FdoPhysicalSchemaMappingP retSchemaMapping = 
        static_cast<FdoPhysicalSchemaMapping*>(schemaMapping);
    
    // Populate the generic parts
    bool hasMappings = SetSchemaMappings(retSchemaMapping, includeDefaults);

    // Populate the PostGIS-specific parts.

    if (wcslen(GetOwner()) > 0)
    {
        schemaMapping->SetOwner(GetOwner());
        hasMappings = true;
    }

    // return NULL if schema has no mappings.
    if (!hasMappings) 
        retSchemaMapping = NULL;

    return retSchemaMapping;
}

void FdoSmLpPostGisSchema::Update(FdoFeatureSchema* featSchema,
	FdoSchemaElementState state,
    FdoPhysicalSchemaMapping* overrides,
    bool ignoreStates)
{
    FdoPostGISOvPhysicalSchemaMapping* pgOverrides = NULL;
    pgOverrides = dynamic_cast<FdoPostGISOvPhysicalSchemaMapping*>(overrides);

    // Extract any default physical overrides first since these are used 
    // in parent class's Update.

	if (pgOverrides)
    {
        if (wcslen(pgOverrides->GetOwner()) > 0) 
            SetOwner(pgOverrides->GetOwner());
    }

    // Do parent's Update() next:
    FdoSmLpGrdSchema::Update(featSchema, state, overrides, ignoreStates);
}

FdoSmLpClassDefinitionP FdoSmLpPostGisSchema::CreateClass(FdoSmPhClassReaderP reader)
{
    return (new FdoSmLpPostGisClass(reader, this));
}

FdoSmLpClassDefinitionP FdoSmLpPostGisSchema::CreateFeatureClass(FdoSmPhClassReaderP reader)
{
    return (new FdoSmLpPostGisFeatureClass(reader, this));
}

FdoSmLpClassDefinitionP FdoSmLpPostGisSchema::CreateFeatureClass(
    FdoFeatureClass* fdoClass, 
    bool ignoreStates
)
{
    return new FdoSmLpPostGisFeatureClass(fdoClass, ignoreStates, this);
}

FdoSmLpClassDefinitionP FdoSmLpPostGisSchema::CreateClass(FdoClass* fdoClass,
    bool ignoreStates)
{
    return (new FdoSmLpPostGisClass(fdoClass, ignoreStates, this));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateDataProperty(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisDataPropertyDefinition(reader, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateGeometricProperty(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisGeometricPropertyDefinition(reader, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateObjectProperty(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisObjectPropertyDefinition(reader, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateAssociationProperty(
    FdoSmPhClassPropertyReaderP reader,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisAssociationPropertyDefinition(reader, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateDataProperty(
    FdoDataPropertyDefinition* fdoProp,
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisDataPropertyDefinition(fdoProp,
            ignoreStates, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* fdoProp, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisGeometricPropertyDefinition(fdoProp,
            ignoreStates, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateGeometricProperty(
    FdoGeometricPropertyDefinition* fdoProp, 
    FdoString* columnNameX,
    FdoString* columnNameY,
    FdoString* columnNameZ,
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
{   
    throw FdoCommandException::Create(
            NlsMsgGet(FDORDBMS_467, msgNotSupportedExp));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateObjectProperty(
    FdoObjectPropertyDefinition* fdoProp, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisObjectPropertyDefinition(fdoProp,
            ignoreStates, parent));
}

FdoSmLpPropertyP FdoSmLpPostGisSchema::CreateAssociationProperty(
    FdoAssociationPropertyDefinition* fdoProp, 
    bool ignoreStates,
    FdoSmLpClassDefinition* parent)
{
    return (new FdoSmLpPostGisAssociationPropertyDefinition(fdoProp,
            ignoreStates, parent));
}

void FdoSmLpPostGisSchema::CreatePhysicalSchema(
    FdoSmPhOwnerP owner
)
{
    if ( wcslen(GetName()) > owner->GetManager()->DbObjectNameMaxLen() ) {
        AddSchemaNameLengthError( GetName(), owner->GetManager()->DbObjectNameMaxLen() );
        return;
    }

    FdoPtr<FdoSmPhPostGisOwner> postOwner = owner->SmartCast<FdoSmPhPostGisOwner>();
    if ( postOwner->FindSchema(GetName()) )
        AddSchemaExistsError();
    else
        postOwner->CreateSchema( GetName() );
}

void FdoSmLpPostGisSchema::DeletePhysicalSchema(
    FdoSmPhOwnerP owner
)
{
    FdoPtr<FdoSmPhPostGisOwner> postOwner = owner->SmartCast<FdoSmPhPostGisOwner>();
    FdoSmPhPostGisSchemaP phSchema = postOwner->FindSchema( GetName() );

    if ( phSchema )
        phSchema->SetElementState( FdoSchemaElementState_Deleted );
}

