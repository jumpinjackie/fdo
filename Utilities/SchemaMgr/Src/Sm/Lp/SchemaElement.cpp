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
#include <Sm/Lp/SchemaElement.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Error.h>
#include <Sm/Ph/SADWriter.h>

FdoSmLpSchemaElement::FdoSmLpSchemaElement(
    FdoString* name, 
    FdoString* description, 
    FdoSmLpSchemaElement* parent,
    bool fromFdo
) :
	FdoSmSchemaElement( name, description, parent ),
    mFromFdo(fromFdo)
{
	// This element's schema is the same as the parent's schema
	if ( parent ) 
		mSchema = parent->GetLogicalPhysicalSchema();
	else
		mSchema = NULL;
}

FdoSmLpSchemaElement::~FdoSmLpSchemaElement(void)
{
}

const FdoSmLpSchema* FdoSmLpSchemaElement::RefLogicalPhysicalSchema() const
{
	return ((FdoSmLpSchemaElement*)this)->GetLogicalPhysicalSchema();
}

const FdoSmLpSAD* FdoSmLpSchemaElement::RefSAD() const
{
	return ((FdoSmLpSchemaElement*) this)->GetSAD();
}

void FdoSmLpSchemaElement::CommitSAD( const wchar_t* elementType)
{
	FdoSmPhMgrP         		pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
	FdoSmPhSADWriterP           pWriter = pPhysical->GetSADWriter();
	FdoSmLpSADP		            pSAD = GetSAD();
	FdoStringP			        owner = GetParent() ? (FdoString*) GetParent()->GetQName() : GetName();
	FdoSchemaElementState		elementState = GetElementState();

	// Delete the SAD elements. If we're modifying, they get re-added.
	if ( (elementState == FdoSchemaElementState_Deleted) ||
		 (elementState == FdoSchemaElementState_Modified) ) {
        pWriter->Delete( owner, GetName() );
	}

	// Add or re-add the elements.
	if ( (elementState == FdoSchemaElementState_Added) ||
		 (elementState == FdoSchemaElementState_Modified) ) {
		for ( int i = 0; i < pSAD->GetCount(); i++ ) {
			FdoSmLpSADElementP pSADElement = pSAD->GetItem(i);

			pWriter->SetOwnerName( owner );
			pWriter->SetElementName( GetName() );
			pWriter->SetElementType( elementType );
			pWriter->SetName( pSADElement->GetName() );
			pWriter->SetValue( pSADElement->GetValue() );
			pWriter->Add();
		}
	}
}

void FdoSmLpSchemaElement::XMLSerialize( FILE* xmlFp, int ref  ) const
{
    if ( ref == 0 && mSAD && (mSAD->GetCount() > 0) ) {
		fprintf( xmlFp, "<SAD>\n" );
		for ( int i = 0; i < mSAD->GetCount(); i++ ) {
			const FdoSmLpSADElement* pElement = mSAD->RefItem(i);
			fprintf( xmlFp, "<element name=\"%s\" value=\"%s\" />\n",
				(const char*) FdoStringP(pElement->GetName()), 
				(const char*) FdoStringP(pElement->GetValue())
			);
		}
		fprintf( xmlFp, "</SAD>\n" );
	}

	FdoSmSchemaElement::XMLSerialize( xmlFp, ref );
}

FdoSmLpSchemaP FdoSmLpSchemaElement::GetLogicalPhysicalSchema()
{
	return FDO_SAFE_ADDREF(mSchema);
}

void FdoSmLpSchemaElement::SetLogicalPhysicalSchema(FdoSmLpSchemaP schema)
{
	mSchema = schema;
}

FdoSmLpSADP FdoSmLpSchemaElement::GetSAD()
{
	if ( !mSAD )
		mSAD = new FdoSmLpSAD();

	return mSAD;
}

void FdoSmLpSchemaElement::LoadSAD( FdoSmPhISADReader* pSADReader )
{
	while ( pSADReader->ReadNext() ) {
    	FdoSmLpSADP pSAD = GetSAD();

		FdoSmLpSADElementP pElement = new FdoSmLpSADElement( pSADReader->GetName(), pSADReader->GetValue() );
		pSAD->Add( pElement );
	}
}

bool FdoSmLpSchemaElement::GetHasMetaSchema() const
{
    FdoSmPhOwnerP owner = ((FdoSmLpSchemaElement*) this)->GetLogicalPhysicalSchema()->GetPhysicalSchema()->FindOwner();

    return owner ? owner->GetHasMetaSchema() : false;
}

void FdoSmLpSchemaElement::Update(
    FdoSchemaElement* pFdoElement,
    FdoSchemaElementState elementState,
    bool bIgnoreStates
)
{
	FdoSmPhMgrP         		pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Updating a schema element after it is finalized causes a variety of bugs
    // that are hard to track down. Therefore, log an error when this happens.
    if ( GetState() == FdoSmObjectState_Final ) {
        //message: Cannot update logicalPhysical element '%1$ls', it has already been finalized,
        throw FdoSchemaException::Create (
            FdoSmError::NLSGetMessage(
    			FDO_NLSID(FDOSM_335),
		        (FdoString*) GetQName()
            )
	    );
    }

	SetElementState( elementState );

	if ( GetElementState() == FdoSchemaElementState_Modified )
		SetDescription( pFdoElement->GetDescription() );

    // Make sure name and description aren't too long.

    ValidateStringLength( 
        GetName(), 
        pPhysical->GetDcDbObjectName(L"f_schemainfo"),
        pPhysical->GetDcColumnName(L"schemaname"),
        FDOSM_269, "Schema Element",     
        FDOSM_274, "Name"
	);

    ValidateStringLength( 
        GetDescription(), 
        pPhysical->GetDcDbObjectName(L"f_schemainfo"),
        pPhysical->GetDcColumnName(L"description"),
        FDOSM_269, "Schema Element",     
        FDOSM_271, "Description"
	);


    if ( (GetElementState() == FdoSchemaElementState_Modified) ||
 		 (GetElementState() == FdoSchemaElementState_Added) ||
         GetIsFromFdo()
     ) {
	
		// Load the Schema Attribute Dictionary, when element is being modified
        // or loaded from config doc.

		FdoSADP pFdoSAD = pFdoElement->GetAttributes();

        if ( GetLogicalPhysicalSchema()->GetSchemas()->CanCreatePhysicalObjects() ) {
            FdoSmPhOwnerP owner = GetLogicalPhysicalSchema()->GetPhysicalSchema()->GetOwner();
            if ( (!owner) || !(owner->GetHasMetaSchema()) ) {
                if ( pFdoSAD->GetCount() > 0 )
                    // Need metaschema to store Schema Attribute Dictionary
                    AddSADNoMetaError( owner );
            }
        }

        if ( bIgnoreStates ) {
            // Ignoring element states so do an additive merge
            MergeSAD(pFdoSAD);
        }
        else {
            // When not ignoring element states, complete replace the 
            // SAD from the fdo element.
    		DeleteSAD();	
	    	LoadSAD(pFdoSAD);
        }
	}
}

void FdoSmLpSchemaElement::LoadSAD( FdoSchemaAttributeDictionary* pFdoSAD )
{
	int             iCount = 0;
	FdoSmLpSADP     pSAD = GetSAD();
	FdoString**     pAttNames = pFdoSAD->GetAttributeNames( iCount );
	FdoSmPhMgrP     pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

	for ( int i = 0; i < iCount; i++ ) {
		FdoSmLpSADElementP pElement = new FdoSmLpSADElement( pAttNames[i], pFdoSAD->GetAttributeValue(pAttNames[i]) );

        // Log error if SAD name or value are too long.

        ValidateStringLength( 
            pElement->GetName(), 
            pPhysical->GetDcDbObjectName(L"f_sad"),
            pPhysical->GetDcColumnName(L"name"),
            FDOSM_270, "Schema Attribute Dictionary",     
            FDOSM_274, "Name"
    	);

        ValidateStringLength( 
            pElement->GetValue(), 
            pPhysical->GetDcDbObjectName(L"f_sad"),
            pPhysical->GetDcColumnName(L"value"),
            FDOSM_270, "Schema Attribute Dictionary",     
            FDOSM_275, "Value"  
    	);

        pSAD->Add( pElement );
	}
}

void FdoSmLpSchemaElement::MergeSAD( FdoSchemaAttributeDictionary* pFdoSAD )
{
	int iCount = 0;
	FdoSmLpSADP pSAD = GetSAD();
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

	FdoString** pAttNames = pFdoSAD->GetAttributeNames( iCount );

    // Loop through the SAD entries from FDO and do an additive merge
	for ( int i = 0; i < iCount; i++ ) {
        FdoString* SADName  = pAttNames[i];
        FdoString* SADValue = pFdoSAD->GetAttributeValue( SADName );

        // Check if the entry already exists.
        FdoSmLpSADElementP pSADElement = pSAD->FindItem( SADName );

        if ( pSADElement ) {
            // It does, just update its value
            pSADElement->SetValue( SADValue );
        }
        else {
            // It doesn't, so add it.
    		FdoSmLpSADElementP pElement = new FdoSmLpSADElement( SADName, SADValue );
            pSAD->Add( pElement );
        }

        // Log error if SAD name or value are too long.

        ValidateStringLength( 
            SADName, 
            pPhysical->GetDcDbObjectName(L"f_sad"),
            pPhysical->GetDcColumnName(L"name"),
            FDOSM_270, "Schema Attribute Dictionary",     
            FDOSM_274, "Name"
    	);

        ValidateStringLength( 
            SADValue, 
            pPhysical->GetDcDbObjectName(L"f_sad"),
            pPhysical->GetDcColumnName(L"value"),
            FDOSM_270, "Schema Attribute Dictionary",     
            FDOSM_275, "Value"  
    	);
	}
}

void FdoSmLpSchemaElement::DeleteSAD()
{
    mSAD = NULL;
}

void FdoSmLpSchemaElement::ValidateStringLength( 
    FdoString* pString, 
    FdoString* pTable,
    FdoString* pColumn,
    FdoInt32   elementNlsNum,
    const char* dfltElementName,
    FdoInt32   itemNlsNum,
    const char* dfltItemName
)
{
   try {
        FdoSmPhMgrP physicalSchema = GetLogicalPhysicalSchema()->GetPhysicalSchema();
        FdoSmPhOwnerP owner = physicalSchema->GetOwner();

        if ( owner && owner->GetHasMetaSchema() ) {
            FdoSmPhDbObjectP dbObject = physicalSchema->FindDbObject( pTable );

            if ( dbObject ) {
				FdoSmPhColumnsP columns = dbObject->GetColumns();
                FdoSmPhColumnP column = columns->FindItem( pColumn );

                if ( column ) {

                    physicalSchema->ValidateStringLength( 
                        pString, 
                        column->GetLength(),
                        elementNlsNum,
                        dfltElementName,
                        itemNlsNum,
                        dfltItemName
	                );
                }
            }
        }
    }
    catch ( FdoSchemaException* e ) {
        // Exception is thrown when string too long.
    	GetErrors()->Add( FdoSmErrorType_Other, e );
    }
}

void FdoSmLpSchemaElement::AddSADNoMetaError( FdoSmPhOwnerP owner )
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_36),
				(FdoString*) GetQName(),
                owner ? owner->GetName() : L""
			)
		)
	);
}


