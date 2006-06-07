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
#include <Fdo/Schema/FeatureSchemaCollection.h>
#include <Fdo/Schema/Class.h>
#include <Fdo/Schema/ObjectPropertyDefinition.h>
#include <Fdo/Schema/AssociationPropertyDefinition.h>

FdoSchemaXmlContext::FdoSchemaXmlContext( FdoFeatureSchemaCollection* schemaCollection )
    : FdoXmlContext( FdoXmlFlagsP(schemaCollection->GetDeserializationFlags()), FdoXmlReaderP(schemaCollection->GetXmlReader()) )
{
    mSchemas = schemaCollection;

    Init();
}

FdoSchemaXmlContext::FdoSchemaXmlContext( FdoXmlFlags* flags, FdoXmlWriter* writer )
    : FdoXmlContext( flags, NULL )
{
    mSchemas = NULL;
    mWriter = FDO_SAFE_ADDREF(writer);

    Init();
}

FdoSchemaXmlContext* FdoSchemaXmlContext::Create( FdoFeatureSchemaCollection* schemas )
{
    return new FdoSchemaXmlContext( schemas );
}

FdoSchemaXmlContext* FdoSchemaXmlContext::Create( FdoXmlFlags* flags, FdoXmlWriter* writer )
{
    return new FdoSchemaXmlContext( flags, writer );
}

FdoFeatureSchemaCollection* FdoSchemaXmlContext::GetSchemas() const
{
    FdoFeatureSchemaCollection* schemas = mSchemas;
    schemas->AddRef();
    return schemas;
}

FdoXmlWriter* FdoSchemaXmlContext::GetXmlWriter()
{
    return FDO_SAFE_ADDREF((FdoXmlWriter*) mWriter);
}

FdoPhysicalSchemaMappingCollection* FdoSchemaXmlContext::GetXmlSchemaMappings()
{
    return FDO_SAFE_ADDREF( (FdoPhysicalSchemaMappingCollection*) mXmlSchemaMappings );
}

FdoPtr<FdoFeatureSchema>  FdoSchemaXmlContext::AddSchema( FdoString* schemaName, FdoXmlAttributeCollection* attrs )
{
    FdoFeatureSchemaP schema = mUpdSchemas->FindItem( schemaName );

    if ( schema == NULL ) {
        schema = FdoFeatureSchema::Create( schemaName, L"" );
        mUpdSchemas->Add( schema );
    }

    schema->InitFromXml( this, attrs );

    return(schema);
}

void FdoSchemaXmlContext::AddBaseClassRef( FdoClassDefinition* pReferencer, const FdoString* schemaName, const FdoString* className )
{
    FdoPtr<ClassRef> ref = ClassRef::Create( pReferencer, schemaName, className );
    mBaseClassRefs->Add( ref );
}

void FdoSchemaXmlContext::AddObjPropRef( FdoObjectPropertyDefinition* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> ref = ClassRef::Create( pReferencer, schemaName, className );
    mObjPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddAssocPropRef( FdoAssociationPropertyDefinition* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> tmp = mAssocPropRefs->FindItem(pReferencer->GetQualifiedName());
    if (tmp) {
        return;
    }
    FdoPtr<ClassRef> ref = ClassRef::Create( pReferencer, schemaName, className );
    mAssocPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddIdPropRef( FdoClassDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, idProps );

    // Only consider the first set of identity properties.
    if ( !mIdPropRefs->Contains(ref->GetName()) )
        mIdPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddUniqueConstraintRef( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP props )
{
	FdoPtr<UniqueConstraintRef> ref = UniqueConstraintRef::Create( pClsRef, pUniConsRef, props );
    mUniConsRefs->Add( ref );
}

void FdoSchemaXmlContext::AddObjIdPropRef( FdoObjectPropertyDefinition* pReferencer, FdoString* idProp )
{
    // Put the identity property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(idProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mObjIdPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkClassRef( FdoNetworkClass* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> ref = ClassRef::Create( pReferencer, schemaName, className );
    mNetworkRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkLinkStartAssocPropRef( FdoNetworkLinkFeatureClass* pReferencer, FdoString* assocProp )
{
    // Put the identity property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(assocProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkLinkStartAssocPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkNodeAssocPropRef( FdoNetworkNodeFeatureClass* pReferencer, FdoString* assocProp )
{
    // Put the identity property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(assocProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkNodeAssocPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkLinkEndAssocPropRef( FdoNetworkLinkFeatureClass* pReferencer, FdoString* assocProp )
{
    // Put the identity property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(assocProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkLinkEndAssocPropRefs->Add( ref );
}


void FdoSchemaXmlContext::AddNetworkFeatureCostPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* costProp )
{
    // Put the identity property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(costProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkFeatureCostPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkFeatureNetworkPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* networkProp )
{
    // Put the property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(networkProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkFeatureNetworkPropRefs->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkFeatureRefFeatPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* refFeatProp )
{
    // Put the property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(refFeatProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkFeatureRefFeatPropRef->Add( ref );
}

void FdoSchemaXmlContext::AddNetworkFeatureParentNetworkFeatPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* parentNetworkFeatProp )
{
    // Put the property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(parentNetworkFeatProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mNetworkFeatureParentNetworkFeatPropRef->Add( ref );
}

void FdoSchemaXmlContext::AddAssocIdPropRef( FdoAssociationPropertyDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, idProps );

    mAssocIdPropRefs->Add( ref );    
}

void FdoSchemaXmlContext::AddAssocIdReversePropRef( FdoAssociationPropertyDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, idProps );
    mAssocIdReversePropRefs->Add( ref );    
}

void FdoSchemaXmlContext::AddGeomPropRef( FdoFeatureClass* pReferencer, FdoString* geomProp )
{
    // Put the geometry property name in a single element collection
    FdoStringsP strings = FdoStringCollection::Create();
    strings->Add( FdoStringP(geomProp) );

    // Create and add the reference.
    FdoPtr<StringsRef> ref = StringsRef::Create( pReferencer, strings );
    mGeomPropRefs->Add( ref );
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

    // The root class must be in the original schema collection
    FdoClassDefinitionP rootClass = FindClass( mSchemas, rootSchemaName, rootClassName );

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

void FdoSchemaXmlContext::CommitSchemas()
{
    // Resolve all references.
    ResolveBaseClasses();
    ResolveObjPropClasses();
    ResolveAssociatedPropClasses();
    ResolveIdProps();
    ResolveObjIdProps();
    ResolveNetworkLinkProps();
    ResolveNetworkProps();
    ResolveNetworkNodeProps();
    ResolveNetworkFeatureProps();
    ResolveAssocIdProps();
    ResolveAssocIdReverseProps();
    ResolveGeomProps();
    ResolveDefaultGeomProps();
    ResolveSchemaMappings();
    ResolveUniqueConstraints();

    // If there are errors, throw them.
    ThrowErrors();

    // Otherwise, merge the schemas that were read, into the original schemas.
    MergeSchemas();

    // Shouldn't get any errors at this point, but throw them anyway.
    ThrowErrors();

}

FdoStringP FdoSchemaXmlContext::EncodeName( FdoStringP name )
{
    // Encode name only if requested
    if ( FdoXmlFlagsP(GetFlags())->GetNameAdjust() ) 
        return mWriter->EncodeName( name );
    else
        return name;
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

void FdoSchemaXmlContext::Init()
{
    mUpdSchemas = FdoFeatureSchemaCollection::Create(NULL);

    mBaseClassRefs = ClassRefs::Create();
    mObjPropRefs = ClassRefs::Create();
    mAssocPropRefs = ClassRefs::Create();
    mIdPropRefs = StringsRefs::Create();
    mUniConsRefs = UniqueConstraintRefs::Create();
    mObjIdPropRefs = StringsRefs::Create();
    mNetworkLinkStartAssocPropRefs = StringsRefs::Create();
    mNetworkNodeAssocPropRefs = StringsRefs::Create();
    mNetworkLinkEndAssocPropRefs = StringsRefs::Create();
    mNetworkFeatureCostPropRefs = StringsRefs::Create();
    mNetworkFeatureNetworkPropRefs = StringsRefs::Create();
    mNetworkFeatureParentNetworkFeatPropRef = StringsRefs::Create();
    mNetworkFeatureParentNetworkFeatPropRef = StringsRefs::Create();
    mNetworkFeatureRefFeatPropRef = StringsRefs::Create();
    mNetworkRefs = ClassRefs::Create();
    mAssocIdPropRefs = StringsRefs::Create();
    mAssocIdReversePropRefs = StringsRefs::Create();
    mGeomPropRefs = StringsRefs::Create();
    mXmlSchemaMappings = FdoPhysicalSchemaMappingCollection::Create();
}


void FdoSchemaXmlContext::ResolveBaseClasses()
{
    FdoInt32 i;

    for ( i = 0; i < mBaseClassRefs->GetCount(); i++ ) {

        // Find the referenced class
        FdoPtr<ClassRef> ref = mBaseClassRefs->GetItem(i);
        FdoStringP baseSchemaName = ref->GetSchemaName();
        FdoStringP baseClassName = ref->GetClassName();

        FdoClassDefinitionP referencer = (FdoClassDefinition*)(ref->GetReferencer());
        // Check original schemas first. 
        FdoPtr<FdoClassDefinition> baseClass = FindClass( mSchemas, baseSchemaName, baseClassName );
        // Check the read schemas.
        FdoPtr<FdoClassDefinition> updBaseClass = FindClass( mUpdSchemas, baseSchemaName, baseClassName );

        // Use the original class, unless it is a new class.
        if ( baseClass == NULL ) 
            baseClass = updBaseClass;

        if ( baseClass == NULL ) {
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_26_BASECLASSREF),
                            (FdoString*) ref->GetSchemaName(), 
                            (FdoString*) ref->GetClassName(), 
                            (FdoString*) referencer->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {     
            if ( updBaseClass == NULL ) 
                updBaseClass = baseClass;

            // Set the base class.
            referencer->SetBaseClass( baseClass );

            // Also keep track of the new object for the base class, if 
            // base class was also read from XML.
            ref->SetNewReferenced( updBaseClass );
        }
    }
}


void FdoSchemaXmlContext::ResolveObjPropClasses()
{
    FdoInt32 i;
    FdoPtr<FdoXmlFlags> flags = GetFlags();
    FdoXmlFlags::ErrorLevel errorLevel = flags->GetErrorLevel();

    for ( i = 0; i < mObjPropRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mObjPropRefs->GetItem(i);

        // Find the original object for the object property class.
        FdoPtr<FdoClassDefinition> opClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoObjectPropertyDefinition> refProp = (FdoObjectPropertyDefinition*)(ref->GetReferencer());


        if ( opClass == NULL )
            // New class so get the object read from XML.
            opClass = FindClass( mUpdSchemas, ref->GetSchemaName(), ref->GetClassName() );

        if (opClass == NULL ) {
            // Unable to resolve this reference
            if ( (errorLevel == FdoXmlFlags::ErrorLevel_High) || 
                 (errorLevel == FdoXmlFlags::ErrorLevel_Normal)
            ) {
                // This is an error for higher error levels
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create (
                            FdoSchemaException::NLSGetMessage( 
                                FDO_NLSID(SCHEMA_27_OBJPROPCLASSREF), 
                                (FdoString*) ref->GetSchemaName(),
                                (FdoString*) ref->GetClassName(),
                                (FdoString*) refProp->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            else {
                // For lower error levels, just skip the Object Property.
                FdoPtr<FdoClassDefinition> parent = (FdoClassDefinition*) refProp->GetParent();
                if ( parent ) {
                    FdoPtr<FdoPropertyDefinitionCollection> props = parent->GetProperties();
                    props->Remove( refProp );
                }
            }
        }
        else {
            // Reference was resolved, set the object property class.
            refProp->SetClass( opClass );
        }
    }
}

void FdoSchemaXmlContext::ResolveAssociatedPropClasses()
{
    FdoInt32 i;

    for ( i = 0; i < mAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mAssocPropRefs->GetItem(i);

        FdoPtr<FdoClassDefinition> apClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(ref->GetReferencer());


        if ( apClass == NULL )
            apClass = FindClass( mUpdSchemas, ref->GetSchemaName(), ref->GetClassName() );

        if (apClass == NULL ) {
            // Unable to resolve this reference
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create (
                        FdoSchemaException::NLSGetMessage( 
                            FDO_NLSID(SCHEMA_45_ASSOCPROPCLASSREF),
                            (FdoString*) ref->GetSchemaName(),
                            (FdoString*) ref->GetClassName(),
                            (FdoString*) refProp->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {
            refProp->SetAssociatedClass( apClass );
        }
    }
}



void FdoSchemaXmlContext::ResolveIdProps()
{
    FdoInt32 i;
    FdoInt32 j;
    FdoPtr<FdoXmlFlags> flags = mSchemas->GetDeserializationFlags();
    FdoXmlFlags::ErrorLevel errorLevel = flags->GetErrorLevel();

    for ( i = 0; i < mIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mIdPropRefs->GetItem(i);
        FdoPtr<FdoClassDefinition> refClass = (FdoClassDefinition*)(ref->GetReferencer());
        FdoPtr<FdoClassDefinition> baseClass = GetBaseClass(refClass);

        // Get all the relevant classes
        FdoPtr<FdoClassDefinition> topClass = GetTopClass(refClass);

        FdoStringsP idProps = ref->GetStrings();

        if ( baseClass != NULL ) {
            // Class with base class actually inherits id props from base class.
            // Just check that id properties agree with top base class id properties

            // This checking is only done for the most strict error level.
            if ( errorLevel == FdoXmlFlags::ErrorLevel_High ) {
                // Get the top class identity properties. Check the id property references
                // first, these id properties might not have been resolved yet.
                FdoPtr<StringsRef> topRef = mIdPropRefs->FindItem( topClass->GetQualifiedName() );
                FdoStringsP topIdProps;

                if ( topRef != NULL ) {
                    // There's a reference so get top id properties from it
                    topIdProps = topRef->GetStrings();
                }
                else {
                    // Otherwise, just get them from the top base class.
                    topIdProps = FdoStringCollection::Create();
                
                    FdoDataPropertiesP pTopIdProps = topClass->GetIdentityProperties();
                    for ( j = 0; j < pTopIdProps->GetCount(); j++ ) 
                        topIdProps->Add( FdoDataPropertyP(pTopIdProps->GetItem(j))->GetName() );

                }
                
                // Make sure the counts agree before checking each property.
                if ( idProps->GetCount() != topIdProps->GetCount() ) {
                    AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create (
                                FdoSchemaException::NLSGetMessage( 
                                    FDO_NLSID(SCHEMA_28_BASEIDPROPS), 
                                    (FdoString*) refClass->GetQualifiedName(),
                                    (FdoString*) baseClass->GetQualifiedName()
                                )
                            )
                        )
                    );
                }
                else {
                    for ( j = 0; j < idProps->GetCount(); j++ ) {
                        // Verify that each id property from XML
                        // matches each base class id property.

                        if ( wcscmp( idProps->GetString(j), topIdProps->GetString(j) ) != 0 ) {
                            AddError( 
                                FdoSchemaExceptionP(
                                    FdoSchemaException::Create (
                                        FdoSchemaException::NLSGetMessage( 
                                            FDO_NLSID(SCHEMA_28_BASEIDPROPS), 
                                            (FdoString*) refClass->GetQualifiedName(),
                                            (FdoString*) baseClass->GetQualifiedName()
                                        )
                                    )
                                )
                            );
                        }
                    }
                }
            }
        }
        else {
            // No base class, resolve each id property reference
            for ( j = 0; j < idProps->GetCount(); j++ ) {
                FdoDataPropertyP idProp = (FdoDataPropertyDefinition*) FindProperty( refClass, idProps->GetString(j) );

                if ( idProp == NULL ) {
                    AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create (
                                FdoSchemaException::NLSGetMessage( 
                                    FDO_NLSID(SCHEMA_29_IDPROPREF), 
                                    (FdoString*) idProps->GetString(j),
                                    (FdoString*) refClass->GetQualifiedName()
                                )
                            )
                        )
                    );
                }
                else {
                    FdoDataPropertiesP(refClass->GetIdentityProperties())->Add(idProp);
                }
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveObjIdProps()
{
    FdoInt32 i;

    for ( i = 0; i < mObjIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mObjIdPropRefs->GetItem(i);
        FdoObjectPropertyP refProp = (FdoObjectPropertyDefinition*)(ref->GetReferencer());
        FdoClassDefinitionP opClass = refProp->GetClass();
        FdoStringsP idProps = ref->GetStrings();

        if ( opClass == NULL ) {
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create (
                        FdoSchemaException::NLSGetMessage( 
                            FDO_NLSID(SCHEMA_30_BASEIDPROPS), 
                            (FdoString*) refProp->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {
            FdoDataPropertyP idProp = (FdoDataPropertyDefinition*) FindProperty( opClass, idProps->GetString(0) );

            if ( idProp == NULL ) {
                // identity property was not found.
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create (
                            FdoSchemaException::NLSGetMessage( 
                                FDO_NLSID(SCHEMA_31_OBJPROPIDPROPREF), 
                                (FdoString *) idProps->GetString(0),
                                (FdoString *) refProp->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            else {
                // Set the object property identity property
                refProp->SetIdentityProperty( idProp );
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveNetworkLinkProps()
{

   FdoInt32 i;

    for ( i = 0; i < mNetworkLinkStartAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkLinkStartAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkLinkFeatureClass> refClass = (FdoNetworkLinkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetStartNodeProperty(assoc);
    }

    for ( i = 0; i < mNetworkLinkEndAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkLinkEndAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkLinkFeatureClass> refClass = (FdoNetworkLinkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetEndNodeProperty(assoc);
    }
}


void FdoSchemaXmlContext::ResolveNetworkProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mNetworkRefs->GetItem(i);

        FdoClassDefinition* networkLayerClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoNetworkClass> refClass = (FdoNetworkClass*)(ref->GetReferencer());

        if ( networkLayerClass == NULL )
            networkLayerClass = FindClass( mUpdSchemas, ref->GetSchemaName(), ref->GetClassName() );

        if (networkLayerClass == NULL ) {
            // Unable to resolve this reference
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create (
                        FdoSchemaException::NLSGetMessage( 
                            FDO_NLSID(SCHEMA_50_NETWORKLAYERPROPREF),
                            (FdoString*) ref->GetSchemaName(),
                            (FdoString*) ref->GetClassName(),
                            (FdoString*) refClass->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {
            refClass->SetLayerClass((FdoNetworkLayerClass *) networkLayerClass);
            networkLayerClass->Release();
        }
    }
}


void FdoSchemaXmlContext::ResolveNetworkNodeProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkNodeAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkNodeAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkNodeFeatureClass> refClass = (FdoNetworkNodeFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetLayerProperty(assoc);
    }
}

void FdoSchemaXmlContext::ResolveNetworkFeatureProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkFeatureCostPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureCostPropRefs->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoDataPropertyP prop = (FdoDataPropertyDefinition*) FindProperty( refClass, props->GetString(0) );\
        if (prop)
            refClass->SetCostProperty(prop);
    }

    for ( i = 0; i < mNetworkFeatureNetworkPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureNetworkPropRefs->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetNetworkProperty(assoc);
    }

    for ( i = 0; i < mNetworkFeatureRefFeatPropRef->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureRefFeatPropRef->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetReferencedFeatureProperty(assoc);
    }

    for ( i = 0; i < mNetworkFeatureParentNetworkFeatPropRef->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureParentNetworkFeatPropRef->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(ref->GetReferencer());
        FdoStringsP props = ref->GetStrings();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( FdoClassDefinitionP(assoc->GetAssociatedClass()) )
            refClass->SetParentNetworkFeatureProperty(assoc);
    }
}

void FdoSchemaXmlContext::ResolveAssocIdProps()
{
    FdoInt32 i;
    
    for ( i = 0; i < mAssocIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mAssocIdPropRefs->GetItem(i);
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(ref->GetReferencer());
        FdoClassDefinitionP apClass = refProp->GetAssociatedClass();
        FdoStringsP idProps = ref->GetStrings();

        if ( apClass == NULL ) {
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create (
                        FdoSchemaException::NLSGetMessage( 
                            FDO_NLSID(SCHEMA_46_ASSOCPROPS), 
                            (FdoString*) refProp->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {
            for (int j=0; j<idProps->GetCount(); j++) {
                FdoDataPropertyP idProp = (FdoDataPropertyDefinition*) FindProperty( apClass, idProps->GetString(j) );

                if ( idProp == NULL ) {
                    // identity property was not found.
                    AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create (
                                FdoSchemaException::NLSGetMessage( 
                                    FDO_NLSID(SCHEMA_47_ASSOCPROPIDPROPREF), 
                                    (FdoString*) idProps->GetString(j),
                                    (FdoString*) refProp->GetQualifiedName()
                                )
                            )
                        )
                    );
                }
                else {
                    FdoDataPropertiesP(refProp->GetIdentityProperties())->Add(idProp);
                }
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveAssocIdReverseProps()
{
    FdoInt32 i;
    
    for ( i = 0; i < mAssocIdReversePropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mAssocIdReversePropRefs->GetItem(i);
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(ref->GetReferencer());
        FdoClassDefinitionP apClass = (FdoClassDefinition *) refProp->GetParent();
        FdoStringsP idProps = ref->GetStrings();

        if ( apClass == NULL ) {
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create (
                        FdoSchemaException::NLSGetMessage( 
                            FDO_NLSID(SCHEMA_48_ASSOCREVERSEPROPCLASSREF),
                            (FdoString*) refProp->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {

            for (int j=0; j<idProps->GetCount(); j++) {
                FdoDataPropertyP idProp = (FdoDataPropertyDefinition*) FindProperty( apClass, idProps->GetString(j) );

                if ( idProp == NULL ) {
                    // identity property was not found.
                    AddError( 
                        FdoSchemaExceptionP(
                            FdoSchemaException::Create (
                                FdoSchemaException::NLSGetMessage( 
                                    FDO_NLSID(SCHEMA_49_ASSOCREVERSEPROPIDPROPREF), 
                                    (FdoString*) idProps->GetString(j),
                                    (FdoString*) refProp->GetQualifiedName()
                                )
                            )
                        )
                    );
                }
                else {
                    FdoDataPropertiesP(refProp->GetReverseIdentityProperties())->Add(idProp);
                }
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveGeomProps()
{
    FdoInt32 i;

    for ( i = 0; i < mGeomPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mGeomPropRefs->GetItem(i);
        FdoFeatureClassP refClass = (FdoFeatureClass*)(ref->GetReferencer());
        FdoStringsP geomProps = ref->GetStrings();
        FdoStringP geomPropName = geomProps->GetString(0);

        if ( geomPropName != L"" ) {
            FdoGeometricPropertyP geomProp = (FdoGeometricPropertyDefinition*) FindProperty( refClass, geomPropName );

            if ( geomProp == NULL ) {
                // geometry property was not found.
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create (
                            FdoSchemaException::NLSGetMessage( 
                                FDO_NLSID(SCHEMA_32_GEOMPROPREF), 
                                (FdoString*) geomPropName,
                                (FdoString*) refClass->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            else {
                FdoPropertiesP refClassProps = refClass->GetProperties();

                // If a geometry property was specified for a feature class,
                // but it has no settings, then it might be an inherited property.
                // In this case the geometric property is removed from the feature
                // and the geometry property is re-pointed to the base property
                if ( geomProp->GetAllDefaults() ) {

                    // Look for the base property. Either the top base property 
                    // or the lowest one with explicit settings.
                    FdoFeatureClassP baseClass =
                        dynamic_cast<FdoFeatureClass*>(refClass->GetBaseClass());
                    FdoGeometricPropertyP topGeomProp = geomProp;

                    while ( topGeomProp->GetAllDefaults() && baseClass ) {
                        FdoGeometricPropertyP baseGeomProp = (FdoGeometricPropertyDefinition*) 
                            FindProperty( baseClass, geomProp->GetName() );
                
                        if ( !baseGeomProp )
                            // reached top base property, we're done
                            break;

                        topGeomProp = baseGeomProp;
                        baseClass = dynamic_cast<FdoFeatureClass*>(baseClass->GetBaseClass());
                    }

                    // repoint the geometry property if a base property was found
                    if ( geomProp->GetQualifiedName() != topGeomProp->GetQualifiedName() ) {
                        // If previous geometry property is property in the feature
                        // class then remove it.
                        if ( refClassProps->FindItem(geomProp->GetName()) )
                            refClassProps->Remove(geomProp);
                        geomProp = topGeomProp;
                    }
                }

                // set the geometry property
                refClass->SetGeometryProperty(geomProp);
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveUniqueConstraints()
{
    for (FdoInt32 i = 0; i < mUniConsRefs->GetCount(); i++ ) {
        FdoPtr<UniqueConstraintRef> ref = mUniConsRefs->GetItem(i);
        FdoClassDefinitionP pClass = ref->GetRefClass();
        FdoUniqueConstraintP pConstraint = ref->GetRefUniqueConstraint();
        FdoStringsP consProps = ref->GetStrings();

        for (FdoInt32 j=0; j<consProps->GetCount(); j++) {
            FdoStringP dvPropName = consProps->GetString(j);
            FdoDataPropertyP dvProp = (FdoDataPropertyDefinition*) FindProperty( pClass, dvPropName );
            
            if ( dvProp == NULL ) {
                // data property was not found.
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create (
                            FdoSchemaException::NLSGetMessage( 
                                FDO_NLSID(SCHEMA_65_UNICONSPROPREF), 
                                (FdoString*) dvPropName,
                                (FdoString*) pClass->GetQualifiedName()
                            )
                        )
                    )
                );
            }
            else
            {
                FdoDataPropertiesP(pConstraint->GetProperties())->Add(dvProp);
            }
        }
    }
}

void FdoSchemaXmlContext::ResolveDefaultGeomProps()
{
    FdoInt32 i,j;

    for ( i = 0; i < mGeomPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mGeomPropRefs->GetItem(i);
        FdoFeatureClassP refClass = (FdoFeatureClass*)(ref->GetReferencer());
        FdoStringsP geomProps = ref->GetStrings();
        FdoStringP geomPropName = geomProps->GetString(0);

        // If class has no explicit geometry property, try to pick one.
        if ( (geomPropName == L"") && !(refClass->GetGeometryProperty()) ) {
            FdoGeometricPropertyP geomProp1;
            FdoGeometricPropertyP geomProp2;

            // Start with the class and work up the ancestory.
            FdoClassDefinitionP currClass = FDO_SAFE_ADDREF((FdoFeatureClass*) refClass);

            while ( currClass && !geomProp2 ) {
                FdoPropertiesP props = currClass->GetProperties();

                for ( j = 0; j < props->GetCount(); j++ ) {
                    FdoPropertyP prop = props->GetItem(j);

                    if ( prop->GetPropertyType() == FdoPropertyType_GeometricProperty ) {
                        if ( geomProp1 ) {
                            // Found more than one geometric property, can't pick a default
                            // geometryProperty
                            geomProp2 = FDO_SAFE_ADDREF((FdoGeometricPropertyDefinition*)(FdoPropertyDefinition*)prop);
                            break;
                        }
                        else {
                            // Found a geometric property, make it the candidate
                            geomProp1 = FDO_SAFE_ADDREF((FdoGeometricPropertyDefinition*)(FdoPropertyDefinition*)prop);
                        }
                    }
                }

                currClass = currClass->GetBaseClass();
            }

            if ( geomProp1 && !geomProp2 ) 
                // Found a default geometric property, make it the geometry property.
                refClass->SetGeometryProperty( geomProp1 );
        }
    }
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

void FdoSchemaXmlContext::MergeSchemas()
{
    FdoInt32 updIdx;

    // For each schema that was read.
    for ( updIdx = 0; updIdx < mUpdSchemas->GetCount(); updIdx++ ) {
        FdoFeatureSchemaP newSchema = mUpdSchemas->GetItem(updIdx);

        FdoFeatureSchemaP oldSchema = mSchemas->FindItem( newSchema->GetName() );

        if ( oldSchema != NULL ) {
            // Schema already exists, merge in new definition
            oldSchema->Set(newSchema, this);
        }
        else {
            // Schema is new, just add it to the Feature Schema collection.
            mSchemas->Add(newSchema);
        }
    }
}

FdoClassDefinition* FdoSchemaXmlContext::FindClass( FdoFeatureSchemaCollection* pSchemas, const FdoString* schemaName, const FdoString* className )
{
    FdoClassDefinition* classDef = NULL;
    FdoFeatureSchemaP schema = pSchemas->FindItem(schemaName);

    if ( schema != NULL )
        classDef = FdoClassesP(schema->GetClasses())->FindItem(className);

    return(classDef);
}

FdoPropertyDefinition* FdoSchemaXmlContext::FindProperty( FdoClassDefinition* pClass, const FdoString* propName, FdoBoolean checkUpdSchemas )
{
    FDO_SAFE_ADDREF(pClass);
    FdoClassDefinitionP currClass = pClass;
    FdoPropertyDefinition* pProp = NULL;

    // Walk up the base classes until the property is found.
    while ( (pProp == NULL) && (currClass != NULL) ) {
        pProp = FdoPropertiesP(currClass->GetProperties())->FindItem( propName );

        if ( pProp == NULL )
            currClass = GetBaseClass(currClass);
    }

    if ( (pProp == NULL) && checkUpdSchemas ) {
        // Property not found, might be a new property so check the schemas that were
        // read from XML.
        FdoFeatureSchemaP schema = pClass->GetFeatureSchema();
        FdoClassDefinitionP updClass = FindClass( mUpdSchemas, schema->GetName(), pClass->GetName() );

        if ( updClass != NULL ) 
            pProp = FindProperty( updClass, propName, false );
    }

    return( pProp );
}

FdoDataPropertyDefinition* FdoSchemaXmlContext::GetIdProperty( FdoClassDefinition* pClass, const FdoInt32 idx, FdoBoolean checkUpdSchemas )
{
    FDO_SAFE_ADDREF(pClass);

    // Check the top base class
    FdoClassDefinitionP topClass = GetTopClass(pClass);
    FdoDataPropertyDefinition* pProp = FdoDataPropertiesP(topClass->GetIdentityProperties())->GetItem( idx );

    if ( (pProp == NULL) && checkUpdSchemas ) {
        // Identity property not found, might be a new property so check the schemas
        // that were read from XML.
        FdoFeatureSchemaP schema = pClass->GetFeatureSchema();
        FdoClassDefinitionP updClass = FindClass( mUpdSchemas, schema->GetName(), pClass->GetName() );

        if ( updClass != NULL ) 
            pProp = GetIdProperty( updClass, idx, false );
    }

    return( pProp );
}

FdoClassDefinition* FdoSchemaXmlContext::GetTopClass( FdoClassDefinition* pClass )
{
    FDO_SAFE_ADDREF(pClass);
    FdoClassDefinitionP currClass = pClass;
    FdoClassDefinitionP baseClass = GetBaseClass(pClass);
    FdoClassDefinition* topClass = NULL;

    // Find the topmost base class
    while ( baseClass != NULL ) {
        currClass = baseClass;
        baseClass = GetBaseClass(baseClass);
    }

    topClass = currClass;
    FDO_SAFE_ADDREF( topClass );

    return( topClass );
}

FdoClassDefinition* FdoSchemaXmlContext::GetBaseClass( FdoClassDefinition* pClass )
{
    // Check if this class has a base class reference
    FdoPtr<ClassRef> ref = mBaseClassRefs->FindItem( pClass->GetQualifiedName() );

    if ( ref == NULL ) 
        // It doesn't, so just return the base class
        return pClass->GetBaseClass();
    else
        // It does, so return the most up-to-date object for the base class.
        return (FdoClassDefinition*) ref->GetNewReferenced();
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

FdoSchemaXmlContext::ClassRef* FdoSchemaXmlContext::ClassRef::Create( FdoSchemaElement* pReferencer, FdoStringP schemaName, FdoStringP className )
{
    return new ClassRef( pReferencer, schemaName, className );
}

FdoSchemaElement* FdoSchemaXmlContext::ClassRef::GetReferencer()
{
    FdoSchemaElement* pReferencer = mReferencer;
    FDO_SAFE_ADDREF(pReferencer);

    return(pReferencer);
}

FdoSchemaElement* FdoSchemaXmlContext::ClassRef::GetNewReferenced()
{
    FdoSchemaElement* pReferenced = mNewReferenced;
    FDO_SAFE_ADDREF(pReferenced);

    return(pReferenced);
}

void FdoSchemaXmlContext::ClassRef::SetNewReferenced( FdoSchemaElement* pNewReferenced )
{
    FDO_SAFE_ADDREF(pNewReferenced);
    mNewReferenced = pNewReferenced;
}

FdoStringP FdoSchemaXmlContext::ClassRef::GetName()
{
    return( mReferencerName );
}

FdoStringP FdoSchemaXmlContext::ClassRef::GetSchemaName()
{
    return( mSchemaName );
}

FdoStringP FdoSchemaXmlContext::ClassRef::GetClassName()
{
    return( mClassName );
}

FdoSchemaXmlContext::ClassRef::ClassRef( FdoSchemaElement* pReferencer, FdoStringP schemaName, FdoStringP className )
{
    FDO_SAFE_ADDREF(pReferencer);
    mReferencer = pReferencer;
    mReferencerName = pReferencer->GetQualifiedName();
    mSchemaName = schemaName;
    mClassName = className;
}

void FdoSchemaXmlContext::ClassRef::Dispose()
{
    delete this;
}

void FdoSchemaXmlContext::ClassRefs::Dispose()
{
    delete this;
}

FdoSchemaXmlContext::StringsRef* FdoSchemaXmlContext::StringsRef::Create( FdoSchemaElement* pReferencer, FdoStringsP strings )
{
    return new StringsRef( pReferencer, strings );
}

FdoSchemaElement* FdoSchemaXmlContext::StringsRef::GetReferencer()
{
    FdoSchemaElement* pReferencer = mReferencer;
    FDO_SAFE_ADDREF(pReferencer);

    return(pReferencer);
}

FdoStringP FdoSchemaXmlContext::StringsRef::GetName()
{
    return( mQName );
}


FdoStringsP FdoSchemaXmlContext::StringsRef::GetStrings()
{
    return( mStrings );
}

void FdoSchemaXmlContext::StringsRef::Dispose()
{
    delete this;
}

FdoSchemaXmlContext::StringsRef::StringsRef( FdoSchemaElement* pReferencer, FdoStringsP strings )
{
    FDO_SAFE_ADDREF(pReferencer);
    mReferencer = pReferencer;
    mStrings = strings;
    mQName = mReferencer->GetQualifiedName();
}

void FdoSchemaXmlContext::StringsRefs::Dispose()
{
    delete this;
}

FdoSchemaXmlContext::UniqueConstraintRef* FdoSchemaXmlContext::UniqueConstraintRef::Create( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP strings )
{
    return new UniqueConstraintRef( pClsRef, pUniConsRef, strings );
}

FdoSchemaXmlContext::UniqueConstraintRef::UniqueConstraintRef( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP strings )
{
    FDO_SAFE_ADDREF(pClsRef);
	FDO_SAFE_ADDREF(pUniConsRef);
    mClsRef = pClsRef;
	mUniConsRef = pUniConsRef;
    mStrings = strings;
    mQName = mClsRef->GetQualifiedName();
}

FdoClassDefinition* FdoSchemaXmlContext::UniqueConstraintRef::GetRefClass()
{
    FdoClassDefinition* pClsRef = mClsRef;
    FDO_SAFE_ADDREF(pClsRef);

    return(pClsRef);
}

FdoUniqueConstraint* FdoSchemaXmlContext::UniqueConstraintRef::GetRefUniqueConstraint()
{
    FdoUniqueConstraint* pUniConsRef = mUniConsRef;
    FDO_SAFE_ADDREF(pUniConsRef);

    return(pUniConsRef);
}

FdoStringP FdoSchemaXmlContext::UniqueConstraintRef::GetName()
{
    return( mQName );
}


FdoStringsP FdoSchemaXmlContext::UniqueConstraintRef::GetStrings()
{
    return( mStrings );
}

void FdoSchemaXmlContext::UniqueConstraintRefs::Dispose()
{
    delete this;
}
