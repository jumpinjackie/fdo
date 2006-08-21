// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <FdoStd.h>
#include "XmlContext.h"
#include <fdo/Schema/Class.h>

FdoSchemaXmlContext::FdoSchemaXmlContext( FdoFeatureSchemaCollection* schemaCollection )
    : FdoXmlContext( FdoXmlFlagsP(schemaCollection->GetDeserializationFlags()), FdoXmlReaderP(schemaCollection->GetXmlReader()) )
{
    Init( schemaCollection );
}

FdoSchemaXmlContext::FdoSchemaXmlContext( FdoXmlFlags* flags, FdoXmlWriter* writer )
    : FdoXmlContext( flags, NULL )
{
    mWriter = FDO_SAFE_ADDREF(writer);

    Init( (FdoFeatureSchemaCollection*) NULL );
}

FdoSchemaXmlContext* FdoSchemaXmlContext::Create( FdoFeatureSchemaCollection* schemas )
{
    return new FdoSchemaXmlContext( schemas );
}

FdoSchemaXmlContext* FdoSchemaXmlContext::Create( FdoXmlFlags* flags, FdoXmlWriter* writer )
{
    return new FdoSchemaXmlContext( flags, writer );
}

FdoXmlWriter* FdoSchemaXmlContext::GetXmlWriter()
{
    return FDO_SAFE_ADDREF((FdoXmlWriter*) mWriter);
}

FdoPtr<FdoSchemaMergeContext> FdoSchemaXmlContext::GetMergeContext()
{
    return mMergeContext;
}

FdoPhysicalSchemaMappingCollection* FdoSchemaXmlContext::GetXmlSchemaMappings()
{
    return FDO_SAFE_ADDREF( (FdoPhysicalSchemaMappingCollection*) mXmlSchemaMappings );
}

void FdoSchemaXmlContext::SetUpdSchemas( FdoFeatureSchemasP schemas )
{
    GetMergeContext()->SetUpdSchemas( schemas );
}

FdoPtr<FdoFeatureSchema>  FdoSchemaXmlContext::AddSchema( FdoString* schemaName, FdoXmlAttributeCollection* attrs )
{
    FdoFeatureSchemasP updSchemas = mMergeContext->GetUpdSchemas();

    FdoFeatureSchemaP schema = updSchemas->FindItem( schemaName );

    if ( schema == NULL ) {
        schema = FdoFeatureSchema::Create( schemaName, L"" );
        updSchemas->Add( schema );
    }

    schema->InitFromXml( this, attrs );

    return(schema);
}

void FdoSchemaXmlContext::AddSchemaMapping (
    FdoString* schemaName,
    FdoString* targetNamespace
)
{
    FdoXmlSchemaMappingP mapping = GetSchemaMapping( schemaName );
    FdoStringP oldNamespace = mapping->GetTargetNamespace();

    // Set targetNamespace only if one was provided.
    if ( FdoStringP(targetNamespace) != L"" ) {
        if ( (oldNamespace != L"") && (oldNamespace != targetNamespace) ) {
            // Schema already has a targetNamespace, log error.
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_64_MULTINAMESPACES),
                            schemaName, 
                            (const wchar_t *) oldNamespace,
                            targetNamespace
                        )
                    )
                )
            );
        }
        else {
            mapping->SetTargetNamespace( targetNamespace );
        }
    }
}

void FdoSchemaXmlContext::AddElementMapping (
    FdoString* elementSchema,
    FdoString* elementName,
    FdoString* classSchema,
    FdoString* className
)
{
    FdoXmlSchemaMappingP mapping = GetSchemaMapping( elementSchema );

    FdoXmlElementMappingsP elemMappings = mapping->GetElementMappings();
    FdoXmlElementMappingP elemMapping = elemMappings->FindItem( elementName );

    if ( !elemMapping ) {
        elemMapping = FdoXmlElementMapping::Create( elementName );
        elemMapping->SetClassName( className );
        elemMapping->SetSchemaName( classSchema );
        elemMappings->Add( elemMapping );
    }
    else {
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_60_MULTIELEMMAPPING),
                        elementSchema, 
                        elementName
                    )
                )
            )
        );
    }
}

void FdoSchemaXmlContext::AddSubElementMapping (
    FdoString* elementSchema,
    FdoString* elementContainer,
    FdoString* elementName,
    FdoString* classSchema,
    FdoString* className,
    FdoString* gmlUri,
    FdoString* gmlLocalName
)
{
    FdoXmlSchemaMappingP mapping = GetSchemaMapping( elementSchema );

    FdoXmlClassMappingsP classMappings = mapping->GetClassMappings();
    FdoXmlClassMappingP classMapping = classMappings->FindItem( elementContainer );

    if ( !classMapping ) {
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_62_NOCLASSMAPPING),
                        elementName,
                        elementSchema, 
                        elementContainer
                    )
                )
            )
        );
    }

    FdoXmlElementMappingsP elemMappings = classMapping->GetElementMappings();
    FdoXmlElementMappingP elemMapping = elemMappings->FindItem( elementName );

    if ( !elemMapping ) {
        elemMapping = FdoXmlElementMapping::Create( elementName );
        elemMapping->SetClassName( className );
        elemMapping->SetSchemaName( classSchema );
        elemMapping->SetGmlUri( gmlUri );
        elemMapping->SetGmlLocalName( gmlLocalName );
        elemMappings->Add( elemMapping );
    }
    else {
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_61_MULTIELEMMAPPING),
                        elementSchema, 
                        elementContainer,
                        elementName
                    )
                )
            )
        );
    }
}

void FdoSchemaXmlContext::AddClassMapping (
    FdoString* classSchema,
    FdoString* className,
    FdoString* gmlName,
    FdoString* wkSchema,
    FdoString* wkClass
)
{
    FdoXmlSchemaMappingP mapping = GetSchemaMapping( classSchema );

    FdoXmlClassMappingsP classMappings = mapping->GetClassMappings();
    FdoXmlClassMappingP classMapping = classMappings->FindItem( className );

    if ( !classMapping ) {
        classMapping = FdoXmlClassMapping::Create( className, gmlName, wkClass, wkSchema );
        classMappings->Add( classMapping );
    }
    else {
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_63_MULTICLASSMAPPING),
                        classSchema, 
                        className
                    )
                )
            )
        );
    }
}

FdoClassDefinition* FdoSchemaXmlContext::CreateClass( 
    FdoString* schemaName,
    FdoString* className, 
    FdoXmlAttributeCollection* atts )
{
    // Get the root class (the most specific ancestor class not in the XML
    // file ).
    FdoXmlAttributeP rootSchemaAtt = atts->FindItem( L"rootSchema" );
    FdoXmlAttributeP rootClassAtt = atts->FindItem( L"rootClass" );

    // The following error should never happen. Any GML class 
    // without base class will have its type resolved by now

    if ( (rootSchemaAtt == NULL) || (rootClassAtt == NULL) ) {
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_35_NOCLASSTYPE),
                        (FdoString*) FdoStringP::Format( L"%ls:%ls", schemaName, className )
                    )
                )
            )
        );

        return(NULL);
    }

    FdoStringP rootSchemaName = DecodeName(rootSchemaAtt->GetValue());
    FdoStringP rootClassName = DecodeName(rootClassAtt->GetValue());

    FdoFeatureSchemasP schemas = GetMergeContext()->GetSchemas();

    // The root class must be in the original schema collection
    FdoClassDefinitionP rootClass = GetMergeContext()->FindClass( schemas, rootSchemaName, rootClassName );

    if ( rootClass == NULL ) {
        // Not there so log an error and give up
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_26_BASECLASSREF),
                        (FdoString*) rootSchemaName, 
                        (FdoString*) rootClassName, 
                        (FdoString*) FdoStringP::Format( L"%ls:%ls", schemaName, className )
                    )
                )
            )
        );

        return(NULL);
    }

    // Create the class with same class type as root class.
    switch ( rootClass->GetClassType() ) {
    case FdoClassType_FeatureClass:
        return FdoFeatureClass::Create( className, L"" );
    default:
        return FdoClass::Create( className, L"" );
    }
}

FdoStringP FdoSchemaXmlContext::EncodeName( FdoStringP name )
{
    // Encode name only if requested
    if ( FdoXmlFlagsP(GetFlags())->GetNameAdjust() ) 
        return mWriter->EncodeName( name );
    else
        return name;
}

void FdoSchemaXmlContext::CommitSchemas()
{
    try {
        GetMergeContext()->CommitSchemas();
    }
    catch ( FdoException* e ) {
        // On exception, copy thrown errors up to the this Xml Context's list.
        FdoPtr<FdoException> currE = e;
        while ( currE ) {
            AddError( currE );
            currE = currE->GetCause();
        }
    }

    ResolveSchemaMappings();

    // If there are errors, throw them.
    ThrowErrors();

}

FdoStringP FdoSchemaXmlContext::RefClass2SchemaName( FdoSchemaElement* pRefElement, FdoClassDefinition* pClass )
{
    FdoStringP schemaName;

    FdoFeatureSchemaP pSchema = pClass->GetFeatureSchema();

    if ( !pSchema ) 
        // Log error where a schema element references an orphan class
        AddError( 
            FdoSchemaException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(SCHEMA_51_CLASSNOSCHEMA),
                    (FdoString*) pRefElement->GetQualifiedName(),
                    (FdoString*) pClass->GetName()
                )
            )
        );
    else
        schemaName = pSchema->GetName();

    // Reference ok, return schema name from referenced class
    return schemaName;
}

FdoBoolean FdoSchemaXmlContext::CheckWriteAssoc( FdoSchemaElement* pRefElement, FdoAssociationPropertyDefinition* pAssoc )
{
    FdoStringP schemaName;

    if ( !pAssoc ) 
        // No association, therefore can't be written to XML
        return false;

    FdoClassDefinitionP pClass = pAssoc->GetAssociatedClass();

    if ( !pClass )
        // Association has no class. However, association can still be partially written.
        return true;

    FdoFeatureSchemaP pSchema = pClass->GetFeatureSchema();

    if ( !pSchema ) {
        // Associated class is orphan (does not belong to a schema).
        // Log an error since assocation property can't be properly written
        AddError( 
            FdoSchemaException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(SCHEMA_51_CLASSNOSCHEMA),
                    (FdoString*) (pRefElement->GetQualifiedName() + L"." + pAssoc->GetName()),
                    (FdoString*) pClass->GetName()
                )
            )
        );

        return false;
    }

    // Associated class ok, association property can be written.
    return true;
}

void FdoSchemaXmlContext::Init( FdoFeatureSchemaCollection* schemas )
{
    // Create merge context. For reading from XML, all types of element
    // modifications are supported.
    mMergeContext = FdoSchemaMergeContext::Create( schemas, true );

    // Class Properties are not additively merged.
    mMergeContext->SetReplaceClass( true );

    // Ok to re-use elements from schemas that were read from XML, when merging
    // into current schemas. The schemas created on read from XML are discarded 
    // anyway.
    mMergeContext->SetCopyElements( false );

    mMergeContext->SetErrorLevel( FdoXmlFlagsP(GetFlags())->GetErrorLevel() );

    mXmlSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
}


void FdoSchemaXmlContext::ResolveSchemaMappings()
{
    FdoInt32 schI, classI;

    for ( schI = 0; schI < mXmlSchemaMappings->GetCount(); schI++ ) {
        FdoXmlSchemaMappingP schemaMapping = (FdoXmlSchemaMapping*) mXmlSchemaMappings->GetItem(schI);
        
        // Resolve element-class mappings for global elements
        FdoXmlElementMappingsP elementMappings = schemaMapping->GetElementMappings();
        ResolveElementMappings( schemaMapping, elementMappings );

        FdoXmlClassMappingsP classMappings = schemaMapping->GetClassMappings();

        for ( classI = 0; classI < classMappings->GetCount(); classI++ ) {
            FdoXmlClassMappingP classMapping = classMappings->GetItem(classI);
            FdoXmlElementMappingsP elementMappings = classMapping->GetElementMappings();

            // Resolve element-class mappings for class sub-elements
            ResolveElementMappings( schemaMapping, elementMappings );
        }
    }
}

void FdoSchemaXmlContext::ResolveElementMappings( 
    FdoXmlSchemaMappingP schemaMapping,
    FdoXmlElementMappingsP elementMappings
)
{
    FdoInt32 elemI;

    for ( elemI = 0; elemI < elementMappings->GetCount(); elemI++ ) {
        FdoXmlElementMappingP elementMapping = elementMappings->GetItem(elemI);
        FdoStringP refSchemaName = elementMapping->GetSchemaName();

        // Determine the schema containing the element's class.
        FdoXmlSchemaMappingP refSchema = (refSchemaName == L"" ) ?
            schemaMapping :
            FdoXmlSchemaMappingP( (FdoXmlSchemaMapping*) mXmlSchemaMappings->GetItem( schemaMapping->GetProvider(), refSchemaName ) );

        if ( refSchema ) {
            // Find the referenced class in the schema.
            FdoXmlClassMappingsP classMappings = refSchema->GetClassMappings();
            FdoXmlClassMappingP classMapping = classMappings->FindItem( elementMapping->GetClassName() );
            
            // classMapping can be NULL; dangling references are ok.
            elementMapping->SetClassMapping( classMapping );
        }
    }
}

FdoXmlSchemaMappingP FdoSchemaXmlContext::GetSchemaMapping( FdoStringP schemaName )
{
    // Create dummy schema mapping for getting the provider name for GML Schema mapping sets.
    if ( !mXmlMapping )
        mXmlMapping = FdoXmlSchemaMapping::Create( L"" );

    // Get the requested schema mapping set
    FdoXmlSchemaMappingP mapping = (FdoXmlSchemaMapping*) mXmlSchemaMappings->GetItem(
        mXmlMapping->GetProvider(),
        schemaName
    );

    if ( !mapping ) {
        // Doesn't exist, so auto-create
        mapping = FdoXmlSchemaMapping::Create( schemaName );
        mXmlSchemaMappings->Add( mapping );
    }

    return mapping;
}

