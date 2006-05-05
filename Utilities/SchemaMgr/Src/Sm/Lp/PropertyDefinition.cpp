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
/*
#include <Sm/Lp/ClassDefinition.h>
*/
#include <Sm/Lp/Schema.h>
#include <Sm/Lp/PropertyTypeMapper.h>
#include <Sm/Ph/ClassPropertySADReader.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Error.h>

FdoSmLpPropertyDefinition::FdoSmLpPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) : 
	FdoSmLpSchemaElement(propReader->GetName(), propReader->GetDescription(), parent),
    mReadOnly( propReader->GetIsReadOnly() ),
    mIsFeatId( propReader->GetIsFeatId() ),
    mIsSystem( propReader->GetIsSystem() ),
	mpDefiningClass(parent),
	mpParentClass(parent),
	mContainingDbObjectName( propReader->GetTableName() ),
	mContainingDbObject( GetLogicalPhysicalSchema()->GetPhysicalSchema()->FindDbObject(mContainingDbObjectName) ),
    mTopProperty(NULL)
{
	// Load the Schema Attribute Dictionary

	FdoSmPhClassPropertySADReaderP pSADReader = propReader->GetSADReader();
	LoadSAD(pSADReader);
}

FdoSmLpPropertyDefinition::FdoSmLpPropertyDefinition(
    FdoPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpSchemaElement(pFdoProp->GetName(), pFdoProp->GetDescription(), parent, true),
    mReadOnly( false ),
    mIsFeatId( false ),
    mIsSystem( false ),
	mpDefiningClass(parent),
	mpParentClass(parent),
    mTopProperty(NULL)
{
}

FdoSmLpPropertyDefinition::FdoSmLpPropertyDefinition(
	FdoSmLpPropertyP baseProperty, 
	FdoSmLpClassDefinition* pTargetClass, 
	FdoStringP logicalName,
	FdoStringP physicalName,
	bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpSchemaElement( 
        (logicalName.GetLength() > 0) ? (FdoString*) logicalName : baseProperty->GetName(), 
        baseProperty->GetDescription(), 
        pTargetClass,
        baseProperty->GetIsFromFdo()
    ),
	mpDefiningClass(NULL),
    mpParentClass(pTargetClass),
    mReadOnly( baseProperty->GetReadOnly() ),
	mIsFeatId( bInherit ? baseProperty->GetIsFeatId() : false ),
	mIsSystem( bInherit ? baseProperty->GetIsSystem() : false ),
	mContainingDbObjectName( baseProperty->GetContainingDbObjectName() ),
	mContainingDbObject( baseProperty->GetContainingDbObject() ),
    mTopProperty(NULL)
{
	// The above ensures that inherited system or featid properties 
	// stay as system or featid. However copied properties lose their
	// system'ness or featid'ness.

    const FdoSmLpPropertyDefinition* pTopProperty = ( baseProperty->RefBaseProperty() ? baseProperty->RefBaseProperty() : baseProperty.p );

    // Set the element state of the copied property if it is a FeatID or non-system
    // property. Also set it for system properties that belong to a table that 
    // does not exist. These system properties need to be added to the class table.
    // Other system properties remain on their system tables.
    if ( baseProperty->GetIsFeatId() || 
        (!pTopProperty->RefContainingDbObject() && (wcscmp(baseProperty->GetName(), L"Bounds") != 0)) ||
        (wcscmp(baseProperty->RefDefiningClass()->RefLogicalPhysicalSchema()->GetName(), FdoSmLpSchema::mMetaClassSchemaName) != 0) 
    ) {
		// Need to add property if either the base property or target class are new
		if ( (pTargetClass->GetElementState() == FdoSchemaElementState_Added) ||
			 (baseProperty->GetElementState() == FdoSchemaElementState_Added) )
			SetElementState( FdoSchemaElementState_Added );

		// Need to delete property if either the base property or target class are new.
		// Also delete if one is Deleting and the other is being Added.
		if ( (pTargetClass->GetElementState() == FdoSchemaElementState_Deleted) ||
			 (baseProperty->GetElementState() == FdoSchemaElementState_Deleted) )
			SetElementState( FdoSchemaElementState_Deleted );
	}

    mPrevProperty = baseProperty;

	if ( bInherit ) {
		// Inheriting, so set the defining class to be the 
		// base property's defining class.
		mpDefiningClass = baseProperty->RefDefiningClass();

		// If the base property is also inherited then the base 
		// property for this property is the base property for the 
		// base property.
		mBaseProperty = FDO_SAFE_ADDREF((FdoSmLpPropertyDefinition*) pTopProperty);
	}
	else {
		// This property is just a copy of the base property, so 
		// just make its containing class the defining class.
		mpDefiningClass = pTargetClass;
		// Keep track of the original source property for this copy property.
        const FdoSmLpPropertyDefinition* pSrcProperty = ( baseProperty->RefSrcProperty() ? baseProperty->RefSrcProperty() : baseProperty.p );
		mSrcProperty = FDO_SAFE_ADDREF((FdoSmLpPropertyDefinition*) pSrcProperty);
	}

	// Log an error if the base property has errors, since the errors most
	// likely get propagated to this property.
	if ( baseProperty->GetErrors()->GetCount() > 0 )
		AddBasePropError( baseProperty );
}

bool FdoSmLpPropertyDefinition::GetReadOnly() const
{
    return mReadOnly;
}

bool FdoSmLpPropertyDefinition::GetIsFeatId() const
{
    return mIsFeatId;
}

void FdoSmLpPropertyDefinition::SetIsFeatId(bool value)
{
    mIsFeatId = value;
}

bool FdoSmLpPropertyDefinition::GetIsSystem() const
{
    return mIsSystem;
}

const FdoSmLpClassDefinition* FdoSmLpPropertyDefinition::RefDefiningClass() const
{
	return mpDefiningClass;
}

const FdoSmLpPropertyDefinition* FdoSmLpPropertyDefinition::RefBaseProperty() const
{
	return (FdoSmLpPropertyDefinition*) ((FdoSmLpPropertyDefinition*) this)->GetBaseProperty();
}

FdoSmLpPropertyP FdoSmLpPropertyDefinition::GetBaseProperty()
{
    return mBaseProperty;
}

const FdoSmLpPropertyDefinition* FdoSmLpPropertyDefinition::RefSrcProperty() const
{
	return (FdoSmLpPropertyDefinition*) ((FdoSmLpPropertyDefinition*) this)->GetSrcProperty();
}

FdoSmLpPropertyP FdoSmLpPropertyDefinition::GetSrcProperty()
{
    return mSrcProperty;
}

const FdoSmLpPropertyDefinition* FdoSmLpPropertyDefinition::RefPrevProperty() const
{
	return (FdoSmLpPropertyDefinition*) ((FdoSmLpPropertyDefinition*) this)->GetPrevProperty();
}

FdoSmLpPropertyP FdoSmLpPropertyDefinition::GetPrevProperty()
{
    return mPrevProperty;
}

const FdoSmLpPropertyDefinition* FdoSmLpPropertyDefinition::RefTopProperty() const
{
	return (FdoSmLpPropertyDefinition*) ((FdoSmLpPropertyDefinition*) this)->GetTopProperty();
}

FdoSmLpPropertyP FdoSmLpPropertyDefinition::GetTopProperty()
{
    return FdoSmLpPropertyP( mTopProperty ? FDO_SAFE_ADDREF(mTopProperty) : FDO_SAFE_ADDREF(this) );
}

FdoString* FdoSmLpPropertyDefinition::GetContainingDbObjectName() const
{
    ((FdoSmLpPropertyDefinition*) this)->Finalize();

	return mContainingDbObjectName;
}

const FdoSmPhDbObject* FdoSmLpPropertyDefinition::RefContainingDbObject() const
{
	return (FdoSmPhDbObject*)(((FdoSmLpPropertyDefinition*)this)->GetContainingDbObject());
}

FdoSmPhDbObjectP FdoSmLpPropertyDefinition::GetContainingDbObject()
{
    Finalize();

    return mContainingDbObject;
}

const FdoSmLpSAD* FdoSmLpPropertyDefinition::RefSAD() const
{
	if ( RefBaseProperty() )
		// Inherited property also inherits attributes so just return
		// the base property SAD
		return( RefBaseProperty()->RefSAD() );
	else
		// Not inherited, return this property's SAD.
		return FdoSmLpSchemaElement::RefSAD();
}

FdoStringP FdoSmLpPropertyDefinition::GetQName() const
{
	return( GetParent()->GetQName() + L"." + GetName() );
}

FdoStringP FdoSmLpPropertyDefinition::GetNestedName() const
{
    return( GetQName().Right( L"." ) );
}
/*
FdoOracleOvPropertyP FdoSmLpPropertyDefinition::GetSchemaMappings( bool bIncludeDefaults ) const
{
    return FdoOracleOvPropertyP();
}
*/    
void FdoSmLpPropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
	bool same = true;

	// Not allowed to redefine Schema Attribute Dictionary.
	if ( RefSAD()->GetCount() > 0 ) 
		AddRedefinedError( pBaseProp );

	if ( (pBaseProp->GetElementState() != FdoSchemaElementState_Deleted) &&
		(GetElementState() != FdoSchemaElementState_Deleted) ) {
	
		// Inherited and base property must have the same property type.
		if ( GetPropertyType() != pBaseProp->GetPropertyType() ) {
			same = false;
			AddRedefinedError( pBaseProp );
		}
	}

	if ( same ) {
        // When a non-system property is inherited from a system property, this is 
        // considered to be meta-inheritance. Therefore, don't set this property
        // to be inherited in this case.
        if ( mIsSystem == pBaseProp->GetIsSystem() ) {
    	    mpDefiningClass = pBaseProp->RefDefiningClass();
            const FdoSmLpPropertyDefinition* pBaseProperty = pBaseProp->RefBaseProperty() ? pBaseProp->RefBaseProperty() : pBaseProp;
            mBaseProperty = FDO_SAFE_ADDREF((FdoSmLpPropertyDefinition*) pBaseProperty);
            mPrevProperty = FDO_SAFE_ADDREF((FdoSmLpPropertyDefinition*) pBaseProp);
		}

		// Update attributes when base property is new or modified.
  		// FDO does not define the featId flag. Therefore, it needs to be 
		// initialized for new inherited properties.
  		if ( (GetElementState() == FdoSchemaElementState_Added)
  	    	|| (pBaseProp->GetElementState() != FdoSchemaElementState_Unchanged) 
  		) {
			SetDescription( pBaseProp->GetDescription() );
			mIsFeatId = pBaseProp->GetIsFeatId();
			mReadOnly = pBaseProp->GetReadOnly();
		}

		InheritElementState( pBaseProp );
	}
}

void FdoSmLpPropertyDefinition::SetTopProperty(FdoSmLpPropertyP pTopProp )
{
	mTopProperty = pTopProp;
}

void FdoSmLpPropertyDefinition::SetSrcProperty(FdoSmLpPropertyP pSrcProp )
{
    const FdoSmLpPropertyDefinition* pSrcProperty = pSrcProp->RefSrcProperty() ? pSrcProp->RefSrcProperty() : pSrcProp.p;
    mSrcProperty = FDO_SAFE_ADDREF((FdoSmLpPropertyDefinition*) pSrcProperty);
    mPrevProperty = pSrcProp;
    mBaseProperty = NULL;
}
   
FdoSmLpPropertyP FdoSmLpPropertyDefinition::CreateInherited( FdoSmLpClassDefinition* pSubClass ) const
{
	FdoSmLpPropertyP pProp = 
		NewInherited( pSubClass );

	// Data Property constructor does not call right SetElementState, so call it again
	// here to ensure the right one gets called.
	pProp->SetElementState( pProp->GetElementState() ) ;

	return( pProp);
}


FdoSmLpPropertyP FdoSmLpPropertyDefinition::CreateCopy( 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName,
    FdoPhysicalPropertyMapping* pPropOverrides
) const
{
	return
        NewCopy( 
            pTargetClass, 
            logicalName, 
            physicalName, 
            pPropOverrides
        );
}

void FdoSmLpPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{
	// Superclass Update must be called first. It updates the element state so that 
	// any logged errors get reported.
	FdoSmLpSchemaElement::Update(pFdoProp, elementState, bIgnoreStates);

	// Property type change not supported.
	if ( GetPropertyType() != pFdoProp->GetPropertyType() ) {
		AddPropTypeChangeError( pFdoProp->GetPropertyType() );
		return;
	}

}

void FdoSmLpPropertyDefinition::Commit( bool fromParent )
{
	FdoSmPhMgrP                	pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Get containing class
    FdoSmLpClassDefinition*		pClass = (FdoSmLpClassDefinition*) RefParentClass();
	
    // Get the highest level containing class. Different from containing class
	// when this property is nested within an object property.
	FdoSmLpClassDefinition*		pTopClass = (FdoSmLpClassDefinition*) (GetTopProperty()->RefParentClass());
    
    FdoSmPhPropertyWriterP       pWriter = pPhysical->GetPropertyWriter();

	// Adds are sub-class specific and are handled at sub-class level.
	switch ( GetElementState() ) {
	case FdoSchemaElementState_Deleted:
        pWriter->Delete( pTopClass->GetId(), GetNestedName() );

		break;

	case FdoSchemaElementState_Modified:
        pWriter->SetDescription( GetDescription() );
        pWriter->SetIsReadOnly( GetReadOnly() );
        pWriter->Modify( pTopClass->GetId(), GetName() );

		break;
	}

	// Inherited properties don't have their own Schema Attribute Dictionary entries.
	if ( !RefBaseProperty() ) 
		CommitSAD(FdoSmPhMgr::PropertyType);
}

FdoSchemaExceptionP FdoSmLpPropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	((FdoSmLpPropertyDefinition*) this)->Finalize();

	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	return pException;
}

void FdoSmLpPropertyDefinition::InheritElementState( 
	const FdoSmLpPropertyDefinition* pBaseProp
)
{

	// Figure out new element state base on old element state
	// and base property element state.
	switch (pBaseProp->GetElementState() ) {
	case FdoSchemaElementState_Added:
		switch ( GetElementState() ) {
		case FdoSchemaElementState_Added:
			// both added, no change.
			break;
		default:
			if ( GetParent()->GetElementState() != FdoSchemaElementState_Deleted ) 
				// base added, this exists, so change this.
				SetElementState( FdoSchemaElementState_Modified );
			break;
		}
		break;
	case FdoSchemaElementState_Deleted:
		// Always delete when base property deleted.
		SetElementState( FdoSchemaElementState_Deleted );
		break;
	case FdoSchemaElementState_Modified:
		switch ( GetElementState() ) {
		case FdoSchemaElementState_Added:
			// This Added, must remain as Added.
			break;
		default:
			if ( GetParent()->GetElementState() != FdoSchemaElementState_Deleted ) 
				// Base modified, this exists, must modify this unless the
				// containing class is going away.
				SetElementState( FdoSchemaElementState_Modified );
			break;
		}
		break;
	case FdoSchemaElementState_Unchanged:
		switch ( GetElementState() ) {
		case FdoSchemaElementState_Deleted:
			// A property that is removed from a class and added
			// to the base class still exists. Therefore modify
			// it if the containing class is not being deleted.
			if ( GetParent()->GetElementState() != FdoSchemaElementState_Deleted ) 
				SetElementState( FdoSchemaElementState_Modified );
			break;
		}
		break;
	}
}

void FdoSmLpPropertyDefinition::Finalize()
{

	if ( GetState() == FdoSmObjectState_Finalizing ) {
        // Error if already finalizing
		if ( GetElementState() != FdoSchemaElementState_Deleted )
			AddReferenceLoopError();
		return;
	}

	if ( GetState() == FdoSmObjectState_Initial ) {
		SetState( FdoSmObjectState_Finalizing);

        // Figure out the containing table. Default it to the table for the 
        // containing class.
        FdoSmLpClassDefinition* pClass = (FdoSmLpClassDefinition*) RefParentClass();
            
        if ( mContainingDbObjectName.GetLength() == 0 ) {
            if ( pClass ) 
                mContainingDbObjectName = pClass->GetDbObjectName();

            mContainingDbObject = NULL;
        }

        if ( (mContainingDbObjectName.GetLength() > 0) && (!mContainingDbObject) ) {
        	// Set the containing table to the containing class's table.
   	        mContainingDbObject = GetLogicalPhysicalSchema()->GetPhysicalSchema()->FindDbObject( mContainingDbObjectName );
        }

        SetState( FdoSmObjectState_Final );
    }
}

void FdoSmLpPropertyDefinition::XmlSerializeProperty( wchar_t* propertyType, FILE* xmlFp, int ref )
{
	fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\" />\n",
			propertyType, GetName(), GetDescription() );
}

void FdoSmLpPropertyDefinition::AddPropTypeChangeError( FdoPropertyType newType )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_204),
			    (FdoString*) GetQName(),
                (FdoString*) FdoSmLpPropertyTypeMapper::Type2String( GetPropertyType() ),
			    (FdoString*) FdoSmLpPropertyTypeMapper::Type2String( newType )
            )
		)
	);
}

void FdoSmLpPropertyDefinition::AddRedefinedError( 
	const FdoSmLpPropertyDefinition* pBaseProp
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_242),
			    (FdoString*) GetQName(), 
			    (FdoString*) pBaseProp->RefDefiningClass()->GetQName(),
			    (FdoString*) pBaseProp->GetName()
            )
		)
	);
}

void FdoSmLpPropertyDefinition::AddBasePropError( 
	const FdoSmLpPropertyDefinition* pBaseProp
)
{
    FdoSmErrorsP baseErrors = ((FdoSmLpPropertyDefinition*)pBaseProp)->GetErrors();
    FdoSmErrorP baseError = baseErrors->GetItem(0);
    FdoSchemaExceptionP baseException = baseError->CreateException(NULL);

	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_246),
			    (FdoString*) pBaseProp->GetQName(), 
			    (FdoString*) GetQName(),
			    baseException->GetExceptionMessage()
            )
		)
	);
}

void FdoSmLpPropertyDefinition::AddWrongOverrideTypeError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_300),
			    (FdoString*) GetQName()
            )
		)
	);
}

void FdoSmLpPropertyDefinition::AddReferenceLoopError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_317),
				(FdoString*) GetQName()
			)
		)
	);
}

void FdoSmLpPropertyDefinition::AddGenericError( 
	FdoStringP specificError
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
			    FDO_NLSID(FDOSM_183),
			    (FdoString*) GetQName(), 
			    (FdoString*) specificError
            )
		)
	);
}
