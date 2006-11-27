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
#include <Sm/SchemaElement.h>
#include <Sm/Error.h>

FdoSmSchemaElement::FdoSmSchemaElement(FdoString* name, FdoString* description, const FdoSmSchemaElement* parent ) :
	FdoSmBase(parent ? parent->GetName() : L"", name),	
	mName(name),
	mDescription(description),
	mpParent(parent),
	mElementState(FdoSchemaElementState_Unchanged),
	mObjectState(FdoSmObjectState_Initial)
{
	mErrors = new FdoSmErrorCollection();
}

FdoSmSchemaElement::FdoSmSchemaElement()
{
}

FdoSmSchemaElement::~FdoSmSchemaElement(void)
{
}

const FdoSmSchemaElement* FdoSmSchemaElement::GetParent() const
{
    return mpParent;
}

FdoString* FdoSmSchemaElement::GetName()const
{
    return mName;
}

FdoStringP FdoSmSchemaElement::GetQName() const
{
	return GetName();
}

FdoString* FdoSmSchemaElement::GetDescription() const
{
    return mDescription;
}

const FdoSmErrorCollection* FdoSmSchemaElement::RefErrors() const
{
    FdoSmErrorsP errors = ((FdoSmSchemaElement*) this)->GetErrors();

    return (FdoSmErrorCollection*) errors;
}

FdoSmErrorsP FdoSmSchemaElement::GetErrors()
{
    if ( !mErrors )
        mErrors = new FdoSmErrorCollection();

	return mErrors;
}

FdoSchemaElementState FdoSmSchemaElement::GetElementState() const
{
	return mElementState;
}

FdoSmObjectState FdoSmSchemaElement::GetState() const
{
	return mObjectState;
}

void FdoSmSchemaElement::SetElementState(FdoSchemaElementState elementState) 
{
	mElementState = elementState;
}

void FdoSmSchemaElement::SetState(FdoSmObjectState objectState) 
{
	mObjectState = objectState;
}

FdoSchemaExceptionP FdoSmSchemaElement::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);
	FdoSchemaExceptionP pPrevException = FDO_SAFE_ADDREF(pFirstException);

	// Skip errors for elements not being modified. We don't want errors
	// in existing schemas to prevent the loading of new schemas.
	if ( mErrors && 
			((mElementState == FdoSchemaElementState_Added) ||
			 (mElementState == FdoSchemaElementState_Deleted) ||
			 (mElementState == FdoSchemaElementState_Modified)) ) {
		for ( int i = 0; i < mErrors->GetCount(); i++ ) {
			const FdoSmError* pError = mErrors->RefItem(i);

			// The NoJoinPath error happens for every non-feature class,
			// since there is no way to join the class table to 
			// F_ClassDefinition ( for classname and schemaname base properties ).
			// Therefore, ignore this error.
			// Todo: look at preventing this error from being generated in the
			// first place.
			if ( pError->GetType() != FdoSmErrorType_NoJoinPath ) {

				pException = pError->CreateException( pPrevException );
				pPrevException = pException;
			}
		}
	}

	return pException;
}

void FdoSmSchemaElement::XMLSerialize( FILE* xmlFp, int ref  ) const
{
	if ( ref == 0 ) {
		for ( int i = 0; i < mErrors->GetCount(); i++ )
			mErrors->GetItem(i)->XMLSerialize( xmlFp, ref );
	}
}

void FdoSmSchemaElement::AddFinalizeLoopError( void )
{

    mErrors->Add( FdoSmErrorType_ClassLoop, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_129), 
				(FdoString*) GetQName()
            )
        )
    );    
}



