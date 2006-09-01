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
#include <Fdo/Schema/MergeContext.h>
#include <Fdo/Schema/FeatureSchemaCollection.h>
#include <Fdo/Schema/Class.h>
#include <Fdo/Schema/ObjectPropertyDefinition.h>
#include <Fdo/Schema/AssociationPropertyDefinition.h>
#include <Fdo/Commands/CommandType.h>
#include <Fdo/Commands/Feature/ISelect.h>

FdoSchemaMergeContext::FdoSchemaMergeContext( FdoFeatureSchemaCollection* schemas, bool defaultCapability ) :
    mIgnoreStates(true),
    mDefaultCapability(defaultCapability),
    mReplaceClass(false),
    mCopyElements(true),
    mErrorLevel(FdoXmlFlags::ErrorLevel_High),
    mUpdSchema(NULL)
{
    mSchemas = FDO_SAFE_ADDREF(schemas);
    mUpdSchemas = FdoFeatureSchemaCollection::Create(NULL);

    mElementMaps = ElementMaps::Create();
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

    mClassHasObjects = FdoDictionary::Create();

}

FdoSchemaMergeContext::~FdoSchemaMergeContext(void)
{
    FDO_SAFE_RELEASE( mUpdSchema );
}

FdoSchemaMergeContext* FdoSchemaMergeContext::Create( FdoFeatureSchemaCollection* schemas, bool defaultCapability )
{
    return new FdoSchemaMergeContext( schemas, defaultCapability );
}

bool FdoSchemaMergeContext::GetIgnoreStates()
{
    return mIgnoreStates;
}

void FdoSchemaMergeContext::SetIgnoreStates( bool ignoreStates )
{
    mIgnoreStates = ignoreStates;
}

bool FdoSchemaMergeContext::GetReplaceClass()
{
    return mReplaceClass;
}
    
void FdoSchemaMergeContext::SetReplaceClass( bool replaceClass )
{
    mReplaceClass = replaceClass;
}

bool FdoSchemaMergeContext::GetCopyElements()
{
    return mCopyElements;
}
    
void FdoSchemaMergeContext::SetCopyElements( bool copyElements )
{
    mCopyElements = copyElements;
}

FdoXmlFlags::ErrorLevel FdoSchemaMergeContext::GetErrorLevel()
{
    return mErrorLevel;
}

void FdoSchemaMergeContext::SetErrorLevel( FdoXmlFlags::ErrorLevel errorLevel )
{
    mErrorLevel = errorLevel;
}


FdoFeatureSchemaCollection* FdoSchemaMergeContext::GetSchemas() const
{
    FdoFeatureSchemaCollection* schemas = mSchemas;
    schemas->AddRef();
    return schemas;
}

FdoPtr<FdoFeatureSchemaCollection> FdoSchemaMergeContext::GetUpdSchemas() const
{
    return mUpdSchemas;
}

void FdoSchemaMergeContext::SetUpdSchemas( FdoFeatureSchemaCollection* schemas )
{
    mUpdSchema = NULL;
    mUpdSchemas = schemas;
}

void FdoSchemaMergeContext::SetUpdSchema( FdoFeatureSchema* schema )
{
    FDO_SAFE_RELEASE(mUpdSchema);
    mUpdSchema = FDO_SAFE_ADDREF(schema);
}

FdoIConnection* FdoSchemaMergeContext::GetConnection()
{
    return FDO_SAFE_ADDREF( (FdoIConnection*) mConnection );
}

void FdoSchemaMergeContext::SetConnection( FdoIConnection* connection )
{
    mConnection = FDO_SAFE_ADDREF( connection );
    mClassHasObjects->Clear();
}

void FdoSchemaMergeContext::CommitSchemas()
{
    // Merge the schemas that were read, into the original schemas.
    MergeSchemas();

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
    ResolveUniqueConstraints();

    CheckReferences();

    // If there are errors, throw them.
    ThrowErrors();
}

bool FdoSchemaMergeContext::CheckDeleteClass( FdoClassDefinition* classDef )
{
    bool canDelete = false;

    if ( CanDeleteClass(classDef) ) {
        if ( !ClassHasObjects( classDef ) ) {
            canDelete = true;
        }
        else {
            // Can't delete class that has data.
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_143_DELCLASSOBJECTS),
                            (FdoString*) classDef->GetQualifiedName()
                        )
                    )
                )
            );
        }
    }
    else {
        // Class delete not supported.
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                    FDO_NLSID(SCHEMA_123_DELCLASS),
                        (FdoString*) classDef->GetQualifiedName()
                    )
                )
            )
        );
    }

    return canDelete;
}

bool FdoSchemaMergeContext::CheckAddProperty( FdoPropertyDefinition* prop )
{
    bool canAdd = false;

    if ( CanAddProperty(prop) ) {
        canAdd = true;

        if ( prop->GetPropertyType() == FdoPropertyType_DataProperty ) {
            FdoDataPropertyDefinition* dataProp = (FdoDataPropertyDefinition*) prop;
            FdoClassDefinition* classDef = (FdoClassDefinition*)(prop->GetParent());
            
            if ( (!dataProp->GetNullable()) && ClassHasObjects(classDef) ) {
                // Can't add not-null property if class has objects.
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_144_ADDPROPOBJECTS),
                                (FdoString*) prop->GetQualifiedName()
                            )
                        )
                    )
                );

                canAdd = false;
            }
        }
    }
    else {
        // Adding properties not supported
        AddError( 
            FdoSchemaExceptionP(
                FdoSchemaException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(SCHEMA_124_ADDPROP),
                        (FdoString*) prop->GetQualifiedName()
                    )
                )
            )
        );
    }

    return canAdd;
}

bool FdoSchemaMergeContext::CanModElementDescription( FdoSchemaElement* element )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanAddSchema( FdoFeatureSchema* schema )
{
    bool canAddSchema = mDefaultCapability;

    FdoPtr<FdoIConnection> connection = GetConnection();
    if ( connection ) {
	    FdoPtr<FdoISchemaCapabilities> schemaCapabilities = connection->GetSchemaCapabilities();
        canAddSchema = schemaCapabilities->SupportsMultipleSchemas();
    }

    return canAddSchema;
}

bool FdoSchemaMergeContext::CanDeleteSchema( FdoFeatureSchema* schema )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModSchemaName( FdoFeatureSchema* schema )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanAddClass( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanDeleteClass( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModClassName( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModBaseClass( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModClassAbstract( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModIdProps( FdoClassDefinition* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModFeatGeometry( FdoFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNetLayer( FdoNetworkClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNetCost( FdoNetworkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNetProp( FdoNetworkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNetFeat( FdoNetworkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNetParent( FdoNetworkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModLinkStartNode( FdoNetworkLinkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModLinkEndNode( FdoNetworkLinkFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModNodeLayer( FdoNetworkNodeFeatureClass* classDef )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanAddProperty( FdoPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanDeleteProperty( FdoPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModPropertyName( FdoPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataType( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDefaultValue( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataNullable( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataLength( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataPrecision( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataScale( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataAutoGenerated( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataReadOnly( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModDataConstraint( FdoDataPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModGeomTypes( FdoGeometricPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModGeomElevation( FdoGeometricPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModGeomMeasure( FdoGeometricPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModGeomSC( FdoGeometricPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModGeomReadOnly( FdoGeometricPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocClass( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocReverseName( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocDeleteRule( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocLockCascade( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocMultiplicity( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocReverseMultiplicity( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocIdentity( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocReverseIdentity( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModAssocReadOnly( FdoAssociationPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModObjClass( FdoObjectPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModObjId( FdoObjectPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModObjType( FdoObjectPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModObjOrder( FdoObjectPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterReadOnly( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterNullable( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterModel( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterXSize( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterYSize( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::CanModRasterSC( FdoRasterPropertyDefinition* prop )
{
    return mDefaultCapability;
}

bool FdoSchemaMergeContext::ClassHasObjects( FdoClassDefinition* classDef )
{
    bool hasObjects = false;

    FdoPtr<FdoIConnection> connection = GetConnection();

    // If no connection then class has no reachable objects
    if ( connection ) {
        // Check our cache to avoid doing repeated selects on the same class.
        FdoDictionaryElementP elem = mClassHasObjects->FindItem( classDef->GetQualifiedName() );

        if ( elem ) {
            // Class is in the cache.
            hasObjects = ( FdoStringP(elem->GetValue()) == L"y" );
        }
        else {
            // Not in the cache, check the connection for objects.
            FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
		    selCmd->SetFeatureClassName( classDef->GetQualifiedName() );
            FdoPtr<FdoIFeatureReader> rdr = selCmd->Execute();
            hasObjects = rdr->ReadNext();

            // Add the class, and whether it has objects, to the cache
            elem = FdoDictionaryElement::Create( classDef->GetQualifiedName(), hasObjects ? L"y" : L"n" );
        }
    }

    return hasObjects;
}



void FdoSchemaMergeContext::AddElementMap( FdoSchemaElement* pElement )
{
    FdoPtr<ElementMap> ref = mElementMaps->FindItem( pElement->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetElement( pElement );
    }
    else {
        ref = ElementMap::Create( pElement );
        mElementMaps->Add( ref );
    }
}

void FdoSchemaMergeContext::AddBaseClassRef( FdoClassDefinition* pReferencer, const FdoString* schemaName, const FdoString* className )
{
    FdoPtr<ClassRef> ref = mBaseClassRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        // Reference already exists. Just update the referencer. The referenced class
        // must not change.
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = ClassRef::Create( pReferencer, schemaName, className );
        mBaseClassRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddObjPropRef( FdoObjectPropertyDefinition* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> ref = mObjPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = ClassRef::Create( pReferencer, schemaName, className );
        mObjPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddAssocPropRef( FdoAssociationPropertyDefinition* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> ref = mAssocPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = ClassRef::Create( pReferencer, schemaName, className );
        mAssocPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddIdPropRef( FdoClassDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = mIdPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = StringsRef::Create( pReferencer, idProps );
        mIdPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddUniqueConstraintRef( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP props )
{
    FdoPtr<UniqueConstraintRef> ref = mUniConsRefs->FindItem( pClsRef->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetRefClass( pClsRef );
    }
    else {
        ref = UniqueConstraintRef::Create( pClsRef, pUniConsRef, props );
        mUniConsRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddObjIdPropRef( FdoObjectPropertyDefinition* pReferencer, FdoString* idProp )
{
    FdoPtr<StringsRef> ref = mObjIdPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(idProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mObjIdPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkClassRef( FdoNetworkClass* pReferencer, FdoString* schemaName, FdoString* className )
{
    FdoPtr<ClassRef> ref = mNetworkRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = ClassRef::Create( pReferencer, schemaName, className );
        mNetworkRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkLinkStartAssocPropRef( FdoNetworkLinkFeatureClass* pReferencer, FdoString* assocProp )
{
    FdoPtr<StringsRef> ref = mNetworkLinkStartAssocPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(assocProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkLinkStartAssocPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkNodeAssocPropRef( FdoNetworkNodeFeatureClass* pReferencer, FdoString* assocProp )
{
    FdoPtr<StringsRef> ref = mNetworkNodeAssocPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(assocProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkNodeAssocPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkLinkEndAssocPropRef( FdoNetworkLinkFeatureClass* pReferencer, FdoString* assocProp )
{
    FdoPtr<StringsRef> ref = mNetworkLinkEndAssocPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(assocProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkLinkEndAssocPropRefs->Add( ref );
    }
}


void FdoSchemaMergeContext::AddNetworkFeatureCostPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* costProp )
{
    FdoPtr<StringsRef> ref = mNetworkFeatureCostPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(costProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkFeatureCostPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkFeatureNetworkPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* networkProp )
{
    FdoPtr<StringsRef> ref = mNetworkFeatureNetworkPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(networkProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkFeatureNetworkPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkFeatureRefFeatPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* refFeatProp )
{
    FdoPtr<StringsRef> ref = mNetworkFeatureRefFeatPropRef->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(refFeatProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkFeatureRefFeatPropRef->Add( ref );
    }
}

void FdoSchemaMergeContext::AddNetworkFeatureParentNetworkFeatPropRef( FdoNetworkFeatureClass* pReferencer, FdoString* parentNetworkFeatProp )
{
    FdoPtr<StringsRef> ref = mNetworkFeatureParentNetworkFeatPropRef->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        FdoSchemaElementP oldReferencer = ref->GetReferencer();
        ref->SetReferencer( pReferencer );
        if ( pReferencer != (FdoSchemaElement*) oldReferencer ) 
            CopyNetworkAssocProp( ref );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(parentNetworkFeatProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mNetworkFeatureParentNetworkFeatPropRef->Add( ref );
    }
}

void FdoSchemaMergeContext::AddAssocIdPropRef( FdoAssociationPropertyDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = mAssocIdPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = StringsRef::Create( pReferencer, idProps );
        mAssocIdPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddAssocIdReversePropRef( FdoAssociationPropertyDefinition* pReferencer, FdoStringsP idProps )
{
    FdoPtr<StringsRef> ref = mAssocIdReversePropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        ref = StringsRef::Create( pReferencer, idProps );
        mAssocIdReversePropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::AddGeomPropRef( FdoFeatureClass* pReferencer, FdoString* geomProp )
{
    FdoPtr<StringsRef> ref = mGeomPropRefs->FindItem( pReferencer->GetQualifiedName() );
    
    if ( ref ) {
        ref->SetReferencer( pReferencer );
    }
    else {
        // Put the identity property name in a single element collection
        FdoStringsP strings = FdoStringCollection::Create();
        strings->Add( FdoStringP(geomProp) );

        ref = StringsRef::Create( pReferencer, strings );
        mGeomPropRefs->Add( ref );
    }
}

void FdoSchemaMergeContext::MergeSchemas()
{
    if ( mUpdSchema ) {
        // Updating from single schema
        MergeSchema( mUpdSchema );
    }
    else {
        // Updating from multiple schemas
        FdoInt32 updIdx;
        FdoFeatureSchemasP updSchemas = GetUpdSchemas();

        // For each schema that was read.
        for ( updIdx = 0; updIdx < updSchemas->GetCount(); updIdx++ ) {
            FdoFeatureSchemaP newSchema = updSchemas->GetItem(updIdx);
            MergeSchema( newSchema );
        }
    }
}

void FdoSchemaMergeContext::MergeSchema( FdoFeatureSchema* newSchema )
{
    FdoFeatureSchemasP schemas = GetSchemas();

    FdoFeatureSchemaP oldSchema = schemas->FindItem( newSchema->GetName() );

    // Determine modification state. 

    // If ignoring states then it is a modification if the schema already exists,
    // and an add if it doesn't
    FdoSchemaElementState schemaState = oldSchema ? FdoSchemaElementState_Modified : FdoSchemaElementState_Added;
    if ( !GetIgnoreStates() ) 
        // Not ignoring element states, use state from schema to update from.
        schemaState = newSchema->GetElementState();

    switch  ( schemaState ) {
    case FdoSchemaElementState_Added:
        if ( oldSchema ) {
            // Can't add schema if it already exists
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_118_SCHEMAEXISTS),
                            (FdoString*) newSchema->GetQualifiedName()
                        )
                    )
                )
            );
        }
        else {
            if ( CanAddSchema( newSchema ) ) {
                if ( GetCopyElements() ) {
                    // Make a copy of the schema and add it to schemas to update.
                    oldSchema = FdoFeatureSchema::Create();
                    oldSchema->Set(newSchema, this);
                    schemas->Add(oldSchema);
                }
                else {
                    // Allowed to re-use elements from second set of schemas so 
                    // just move schema to schemas to update.
                    schemas->Add(newSchema);
                }
            }
            else {
                // Adding schema not supported. 
                AddError( 
                    FdoSchemaExceptionP(
                        FdoSchemaException::Create(
                            FdoException::NLSGetMessage(
                                FDO_NLSID(SCHEMA_120_ADDSCHEMA),
                                (FdoString*) newSchema->GetQualifiedName()
                            )
                        )
                    )
               );
            }
        }

        break;

    case FdoSchemaElementState_Deleted:
        if ( CanDeleteSchema(oldSchema) ) {
            // Delete current schema.
            if ( oldSchema ) 
                oldSchema->Delete();
        }
        else {
            // Schema delete not supported
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_121_DELSCHEMA),
                            (FdoString*) oldSchema->GetQualifiedName()
                        )
                    )
                )
            );
        }
        break;

    case FdoSchemaElementState_Modified:
        if ( oldSchema ) {
            // Update schema
            oldSchema->Set(newSchema, this);
        }
        else {
            // Can't update non-existing schema
            AddError( 
                FdoSchemaExceptionP(
                    FdoSchemaException::Create(
                        FdoException::NLSGetMessage(
                            FDO_NLSID(SCHEMA_119_SCHEMANOTEXISTS),
                            (FdoString*) newSchema->GetQualifiedName()
                        )
                    )
                )
            );
        }
        break;
    }
}

void FdoSchemaMergeContext::CheckReferences()
{
    if ( !GetIgnoreStates() ) {
        FdoInt32 idx;
        FdoFeatureSchemasP schemas = GetSchemas();

        // Check references for each schema class.
        for ( idx = 0; idx < schemas->GetCount(); idx++ ) {
            FdoFeatureSchemaP schema = schemas->GetItem( idx );
            schema->CheckReferences( this );
        }
    }
}

void FdoSchemaMergeContext::ResolveBaseClasses()
{
    FdoInt32 i;

    for ( i = 0; i < mBaseClassRefs->GetCount(); i++ ) {

        FdoPtr<ClassRef> ref = mBaseClassRefs->GetItem(i);
        FdoStringP baseSchemaName = ref->GetSchemaName();
        FdoStringP baseClassName = ref->GetClassName();

        FdoClassDefinitionP referencer = (FdoClassDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));

        // Find the referenced class
        FdoPtr<FdoClassDefinition> baseClass = FindClass( mSchemas, baseSchemaName, baseClassName );

        if ( (baseClass == NULL) && ( baseClassName != L"") ) {
            // Dangling reference error
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
            if ( baseClass ) 
                // Class with base class has no identity properties of its own.
                FdoDataPropertiesP(referencer->GetIdentityProperties())->Clear();

            // Set the base class.
            try {
                referencer->SetBaseClass( baseClass );
            }
            catch ( FdoSchemaException* ex ) {
                // A class-baseclass type mismatch is possible.
                AddError( ex );
                FDO_SAFE_RELEASE(ex);
            }
        }
    }
}


void FdoSchemaMergeContext::ResolveObjPropClasses()
{
    FdoInt32 i;
    FdoXmlFlags::ErrorLevel errorLevel = GetErrorLevel();

    for ( i = 0; i < mObjPropRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mObjPropRefs->GetItem(i);

        // Find the original object for the object property class.
        FdoPtr<FdoClassDefinition> opClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoObjectPropertyDefinition> refProp = (FdoObjectPropertyDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoPtr<FdoClassDefinition> parent = (FdoClassDefinition*) refProp->GetParent();
        FdoPtr<FdoClassDefinition> mergeParent = FindClass( mSchemas, parent );

        if ( (opClass == NULL) && (ref->GetClassName() != L"") ) {
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
                // TODO: don't throw away for non-XML merges
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

void FdoSchemaMergeContext::ResolveAssociatedPropClasses()
{
    FdoInt32 i;

    for ( i = 0; i < mAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mAssocPropRefs->GetItem(i);

        FdoPtr<FdoClassDefinition> apClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));

        FdoPtr<FdoClassDefinition> parent = (FdoClassDefinition*) refProp->GetParent();
        FdoPtr<FdoClassDefinition> mergeParent = FindClass( mSchemas, parent );

        if ( (apClass == NULL) && (ref->GetClassName() != L"") ) {
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

void FdoSchemaMergeContext::ResolveIdProps()
{
    FdoInt32 i;
    FdoInt32 j;
    FdoXmlFlags::ErrorLevel errorLevel = GetErrorLevel();

    for ( i = 0; i < mIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mIdPropRefs->GetItem(i);
        FdoPtr<FdoClassDefinition> refClass = (FdoClassDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoPtr<FdoClassDefinition> baseClass = refClass->GetBaseClass();

        // Get all the relevant classes
        FdoPtr<FdoClassDefinition> topClass = GetTopClass(refClass);

        FdoStringsP idProps = ref->GetStrings();

        FdoDataPropertiesP(refClass->GetIdentityProperties())->Clear();

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

void FdoSchemaMergeContext::ResolveObjIdProps()
{
    FdoInt32 i;

    for ( i = 0; i < mObjIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mObjIdPropRefs->GetItem(i);
        FdoObjectPropertyP refProp = (FdoObjectPropertyDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoClassDefinitionP opClass = refProp->GetClass();
        FdoStringsP idProps = ref->GetStrings();

        FdoPtr<FdoClassDefinition> parent = (FdoClassDefinition*) refProp->GetParent();
 
        if ( idProps->GetCount() > 0 ) {
            if ( (opClass == NULL) ) {
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
        else {
            // Set the object property identity property
            refProp->SetIdentityProperty( (FdoDataPropertyDefinition*) NULL );
        }
    }
}

void FdoSchemaMergeContext::ResolveNetworkLinkProps()
{

    FdoInt32 i;

    for ( i = 0; i < mNetworkLinkStartAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkLinkStartAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkLinkFeatureClass> refClass = (FdoNetworkLinkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetStartNodeProperty(assoc);
        }
        else {
            refClass->SetStartNodeProperty( (FdoAssociationPropertyDefinition*) NULL );
        }
    }

    for ( i = 0; i < mNetworkLinkEndAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkLinkEndAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkLinkFeatureClass> refClass = (FdoNetworkLinkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetEndNodeProperty(assoc);
        }
        else {
            refClass->SetEndNodeProperty( (FdoAssociationPropertyDefinition*) NULL );
        }
    }
}


void FdoSchemaMergeContext::ResolveNetworkProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkRefs->GetCount(); i++ ) {
        FdoPtr<ClassRef> ref = mNetworkRefs->GetItem(i);

        FdoClassDefinition* networkLayerClass = FindClass( mSchemas, ref->GetSchemaName(), ref->GetClassName() );
        FdoPtr<FdoNetworkClass> refClass = (FdoNetworkClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));

        if ( (networkLayerClass == NULL) && (ref->GetClassName() != L"") ) {
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
            FDO_SAFE_RELEASE(networkLayerClass);
        }
    }
}


void FdoSchemaMergeContext::ResolveNetworkNodeProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkNodeAssocPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkNodeAssocPropRefs->GetItem(i);
        FdoPtr<FdoNetworkNodeFeatureClass> refClass = (FdoNetworkNodeFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetLayerProperty(assoc);
        }
        else {
            refClass->SetLayerProperty((FdoAssociationPropertyDefinition*) NULL);
        }
    }
}

void FdoSchemaMergeContext::ResolveNetworkFeatureProps()
{
    FdoInt32 i;

    for ( i = 0; i < mNetworkFeatureCostPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureCostPropRefs->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoDataPropertyP prop = (FdoDataPropertyDefinition*) FindProperty( refClass, props->GetString(0) );
            if (prop)
                refClass->SetCostProperty(prop);
        }
        else {
            refClass->SetCostProperty((FdoDataPropertyDefinition*) NULL);
        }
    }

    for ( i = 0; i < mNetworkFeatureNetworkPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureNetworkPropRefs->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetNetworkProperty(assoc);
        }
        else {
            refClass->SetNetworkProperty((FdoAssociationPropertyDefinition*) NULL);
        }
    }

    for ( i = 0; i < mNetworkFeatureRefFeatPropRef->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureRefFeatPropRef->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetReferencedFeatureProperty(assoc);
        }
        else {
            refClass->SetReferencedFeatureProperty((FdoAssociationPropertyDefinition*) NULL);
        }
    }

    for ( i = 0; i < mNetworkFeatureParentNetworkFeatPropRef->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mNetworkFeatureParentNetworkFeatPropRef->GetItem(i);
        FdoPtr<FdoNetworkFeatureClass> refClass = (FdoNetworkFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP props = ref->GetStrings();
        if ( props->GetCount() > 0 ) {
            FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
            FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) (MapElement(FdoSchemaElementP(assocItem->GetReferencer())));
            if ( assoc && FdoClassDefinitionP(assoc->GetAssociatedClass()) )
                refClass->SetParentNetworkFeatureProperty(assoc);
        }
        else {
            refClass->SetParentNetworkFeatureProperty((FdoAssociationPropertyDefinition*) NULL);
        }
    }
}

void FdoSchemaMergeContext::ResolveAssocIdProps()
{
    FdoInt32 i;
    
    for ( i = 0; i < mAssocIdPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mAssocIdPropRefs->GetItem(i);
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoClassDefinitionP apClass = refProp->GetAssociatedClass();
        FdoStringsP idProps = ref->GetStrings();

        FdoDataPropertiesP(refProp->GetIdentityProperties())->Clear();

        if ( idProps->GetCount() > 0 ) {
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
}

void FdoSchemaMergeContext::ResolveAssocIdReverseProps()
{
    FdoInt32 i;
    
    for ( i = 0; i < mAssocIdReversePropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mAssocIdReversePropRefs->GetItem(i);
        FdoPtr<FdoAssociationPropertyDefinition> refProp = (FdoAssociationPropertyDefinition*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoClassDefinitionP apClass = (FdoClassDefinition *) refProp->GetParent();
        FdoStringsP idProps = ref->GetStrings();

        FdoDataPropertiesP(refProp->GetReverseIdentityProperties())->Clear();

        if ( idProps->GetCount() > 0 ) {
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
}

void FdoSchemaMergeContext::ResolveGeomProps()
{
    FdoInt32 i;

    for ( i = 0; i < mGeomPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mGeomPropRefs->GetItem(i);
        FdoFeatureClassP refClass = (FdoFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
        FdoStringsP geomProps = ref->GetStrings();
        FdoStringP geomPropName = (geomProps->GetCount() > 0) ? geomProps->GetString(0) : L"";

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
        else {
            refClass->SetGeometryProperty((FdoGeometricPropertyDefinition*) NULL);
        }
    }
}

void FdoSchemaMergeContext::ResolveUniqueConstraints()
{
    for (FdoInt32 i = 0; i < mUniConsRefs->GetCount(); i++ ) {
        FdoPtr<UniqueConstraintRef> ref = mUniConsRefs->GetItem(i);
        FdoClassDefinitionP pClass = (FdoClassDefinition*)(MapElement(FdoSchemaElementP(ref->GetRefClass())));
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

void FdoSchemaMergeContext::ResolveDefaultGeomProps()
{
    FdoInt32 i,j;

    for ( i = 0; i < mGeomPropRefs->GetCount(); i++ ) {
        FdoPtr<StringsRef> ref = mGeomPropRefs->GetItem(i);
        FdoFeatureClassP refClass = (FdoFeatureClass*)(MapElement(FdoSchemaElementP(ref->GetReferencer())));
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

FdoSchemaElement* FdoSchemaMergeContext::MapElement( FdoSchemaElement* pElement )
{
    if ( !pElement ) 
        return (FdoSchemaElement*) NULL;

    FdoSchemaElement* retElement = NULL;

    FdoPtr<ElementMap> ref = mElementMaps->FindItem( pElement->GetQualifiedName() );

    if ( ref ) 
        retElement = ref->GetElement();
    else
        retElement = FDO_SAFE_ADDREF(pElement);

    return retElement;
}

FdoClassDefinition* FdoSchemaMergeContext::FindClass( FdoFeatureSchemaCollection* pSchemas, FdoClassDefinition* pClass )
{
    if ( !pClass )
        return (FdoClassDefinition*) NULL;

    FdoSchemaElementP parent = pClass->GetParent();
    if ( !parent ) 
        return (FdoClassDefinition*) NULL;

    return FindClass( pSchemas, parent->GetName(), pClass->GetName() );
}

FdoClassDefinition* FdoSchemaMergeContext::FindClass( FdoFeatureSchemaCollection* pSchemas, const FdoString* schemaName, const FdoString* className )
{
    FdoClassDefinition* classDef = NULL;
    FdoFeatureSchemaP schema = pSchemas->FindItem(schemaName);

    if ( schema != NULL )
        classDef = FdoClassesP(schema->GetClasses())->FindItem(className);

    return(classDef);
}

FdoPropertyDefinition* FdoSchemaMergeContext::FindProperty( FdoClassDefinition* pClass, const FdoString* propName, FdoBoolean checkUpdSchemas )
{
    FDO_SAFE_ADDREF(pClass);
    FdoClassDefinitionP currClass = pClass;
    FdoPropertyDefinition* pProp = NULL;

    // Walk up the base classes until the property is found.
    while ( (pProp == NULL) && (currClass != NULL) ) {
        pProp = FdoPropertiesP(currClass->GetProperties())->FindItem( propName );

        if ( pProp == NULL )
            currClass = currClass->GetBaseClass();
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

FdoDataPropertyDefinition* FdoSchemaMergeContext::GetIdProperty( FdoClassDefinition* pClass, const FdoInt32 idx, FdoBoolean checkUpdSchemas )
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

FdoClassDefinition* FdoSchemaMergeContext::GetTopClass( FdoClassDefinition* pClass )
{
    FDO_SAFE_ADDREF(pClass);
    FdoClassDefinitionP currClass = pClass;
    FdoClassDefinitionP baseClass = pClass->GetBaseClass();
    FdoClassDefinition* topClass = NULL;

    // Find the topmost base class
    while ( baseClass != NULL ) {
        currClass = baseClass;
        baseClass = baseClass->GetBaseClass();
    }

    topClass = currClass;
    FDO_SAFE_ADDREF( topClass );

    return( topClass );
}

FdoClassDefinition* FdoSchemaMergeContext::GetBaseClass( FdoClassDefinition* pClass )
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

void FdoSchemaMergeContext::CopyNetworkAssocProp( FdoPtr<StringsRef> ref )
{
    FdoStringsP props = ref->GetStrings();
    
    if ( props->GetCount() > 0 ) {
        FdoClassDefinitionP referencer = (FdoClassDefinition*) ref->GetReferencer();
        FdoPtr<ClassRef> assocItem = mAssocPropRefs->FindItem(props->GetString(0));
        FdoAssociationPropertyP assoc = (FdoAssociationPropertyDefinition *) assocItem->GetReferencer();
        if ( assoc ) {
            FdoAssociationPropertyP newProp = FdoAssociationPropertyDefinition::Create();
            newProp->SetParent(referencer);
            newProp->Set( assoc, this );
        }
    }
}

FdoSchemaMergeContext::ElementMap* FdoSchemaMergeContext::ElementMap::Create( FdoSchemaElement* pElement )
{
    return new ElementMap( pElement );
}

FdoSchemaElement* FdoSchemaMergeContext::ElementMap::GetElement()
{
    FdoSchemaElement* pElement = mElement;
    FDO_SAFE_ADDREF(pElement);

    return(pElement);
}

void FdoSchemaMergeContext::ElementMap::SetElement( FdoSchemaElement* pElement )
{
    mElement = FDO_SAFE_ADDREF(pElement);
    mElementName = pElement->GetQualifiedName();
}

FdoStringP FdoSchemaMergeContext::ElementMap::GetName()
{
    return( mElementName );
}

FdoSchemaMergeContext::ElementMap::ElementMap( FdoSchemaElement* pElement )
{
    SetElement( pElement );
}

void FdoSchemaMergeContext::ElementMap::Dispose()
{
    delete this;
}

void FdoSchemaMergeContext::ElementMaps::Dispose()
{
    delete this;
}

FdoSchemaMergeContext::ClassRef* FdoSchemaMergeContext::ClassRef::Create( FdoSchemaElement* pReferencer, FdoStringP schemaName, FdoStringP className )
{
    return new ClassRef( pReferencer, schemaName, className );
}

FdoSchemaElement* FdoSchemaMergeContext::ClassRef::GetReferencer()
{
    FdoSchemaElement* pReferencer = mReferencer;
    FDO_SAFE_ADDREF(pReferencer);

    return(pReferencer);
}

void FdoSchemaMergeContext::ClassRef::SetReferencer( FdoSchemaElement* pReferencer )
{
    mReferencer = FDO_SAFE_ADDREF(pReferencer);
    mReferencerName = pReferencer->GetQualifiedName();
}

FdoSchemaElement* FdoSchemaMergeContext::ClassRef::GetNewReferenced()
{
    FdoSchemaElement* pReferenced = mNewReferenced;
    FDO_SAFE_ADDREF(pReferenced);

    return(pReferenced);
}

void FdoSchemaMergeContext::ClassRef::SetNewReferenced( FdoSchemaElement* pNewReferenced )
{
    FDO_SAFE_ADDREF(pNewReferenced);
    mNewReferenced = pNewReferenced;
}

FdoStringP FdoSchemaMergeContext::ClassRef::GetName()
{
    return( mReferencerName );
}

FdoStringP FdoSchemaMergeContext::ClassRef::GetSchemaName()
{
    return( mSchemaName );
}

FdoStringP FdoSchemaMergeContext::ClassRef::GetClassName()
{
    return( mClassName );
}

FdoSchemaMergeContext::ClassRef::ClassRef( FdoSchemaElement* pReferencer, FdoStringP schemaName, FdoStringP className )
{
    SetReferencer( pReferencer );
    mSchemaName = schemaName;
    mClassName = className;
}

void FdoSchemaMergeContext::ClassRef::Dispose()
{
    delete this;
}

void FdoSchemaMergeContext::ClassRefs::Dispose()
{
    delete this;
}

FdoSchemaMergeContext::StringsRef* FdoSchemaMergeContext::StringsRef::Create( FdoSchemaElement* pReferencer, FdoStringsP strings )
{
    return new StringsRef( pReferencer, strings );
}

FdoSchemaElement* FdoSchemaMergeContext::StringsRef::GetReferencer()
{
    FdoSchemaElement* pReferencer = mReferencer;
    FDO_SAFE_ADDREF(pReferencer);

    return(pReferencer);
}

void FdoSchemaMergeContext::StringsRef::SetReferencer( FdoSchemaElement* pReferencer )
{
    mReferencer = FDO_SAFE_ADDREF(pReferencer);
    mQName = pReferencer->GetQualifiedName();
}

FdoStringP FdoSchemaMergeContext::StringsRef::GetName()
{
    return( mQName );
}


FdoStringsP FdoSchemaMergeContext::StringsRef::GetStrings()
{
    return( mStrings );
}

void FdoSchemaMergeContext::StringsRef::Dispose()
{
    delete this;
}

FdoSchemaMergeContext::StringsRef::StringsRef( FdoSchemaElement* pReferencer, FdoStringsP strings )
{
    SetReferencer( pReferencer );
    mStrings = strings;
}

void FdoSchemaMergeContext::StringsRefs::Dispose()
{
    delete this;
}

FdoSchemaMergeContext::UniqueConstraintRef* FdoSchemaMergeContext::UniqueConstraintRef::Create( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP strings )
{
    return new UniqueConstraintRef( pClsRef, pUniConsRef, strings );
}

FdoSchemaMergeContext::UniqueConstraintRef::UniqueConstraintRef( FdoClassDefinition* pClsRef, FdoUniqueConstraint* pUniConsRef, FdoStringsP strings )
{
	FDO_SAFE_ADDREF(pUniConsRef);
    SetRefClass( pClsRef );
	mUniConsRef = pUniConsRef;
    mStrings = strings;
}

FdoClassDefinition* FdoSchemaMergeContext::UniqueConstraintRef::GetRefClass()
{
    FdoClassDefinition* pClsRef = mClsRef;
    FDO_SAFE_ADDREF(pClsRef);

    return(pClsRef);
}

void FdoSchemaMergeContext::UniqueConstraintRef::SetRefClass( FdoClassDefinition* pClsRef)
{
    mClsRef = FDO_SAFE_ADDREF(pClsRef);
    mQName = mClsRef->GetQualifiedName();
}

FdoUniqueConstraint* FdoSchemaMergeContext::UniqueConstraintRef::GetRefUniqueConstraint()
{
    FdoUniqueConstraint* pUniConsRef = mUniConsRef;
    FDO_SAFE_ADDREF(pUniConsRef);

    return(pUniConsRef);
}

FdoStringP FdoSchemaMergeContext::UniqueConstraintRef::GetName()
{
    return( mQName );
}


FdoStringsP FdoSchemaMergeContext::UniqueConstraintRef::GetStrings()
{
    return( mStrings );
}

void FdoSchemaMergeContext::UniqueConstraintRefs::Dispose()
{
    delete this;
}
