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
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Ph/PropertyWriter.h>
#include <Sm/Ph/AssociationReader.h>
#include <Sm/Ph/AssociationWriter.h>
#include <Sm/Error.h>


#define  IDENTITY_REVIDENT_NOT_EQUAL_ERROR  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_279)) ) ) 

#define  IDENTITY_PROP_NOT_FOUND_ERR( prop )  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_280),\
    			prop ) ) ) 

#define  REV_IDENTITY_PROP_NOT_FOUND_ERR( prop )  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_281),\
				prop ) ) ) 

#define  IDENTITY_REVIDENT_TYPE_MATCH_ERROR(prop1, prop2)  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_282),\
				prop1, prop2) ) )

#define  ASSOCIATEDCLASS_CHG_ERROR  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_283)) ) ) 

#define  MULTIPLICITY_CHG_ERROR  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_284)) ) ) 

#define  REV_MULTIPLICITY_CHG_ERROR  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_285)) ) ) 

#define  ASSOCIATION_EXIST_ERROR( name1, name2 )    GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_286),\
				name1, name2) ) )

#define ASSOCIATED_CLASS_ABSTRACT( name )  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_288),\
  			    name ) ) ) 

#define ASSOCIATED_CLASS_NOT_FOUND_ERROR( propName, className )  GetErrors()->Add( FdoSmErrorType_Other, \
        FdoSchemaException::Create(\
            FdoSmError::NLSGetMessage(\
                FDO_NLSID(FDOSM_19),\
  			    className,\
                propName) ) ) 

//
// A portable string compare utility function
static int  util_str_cmp( const wchar_t * str1, const wchar_t * str2 )
{
#ifdef _WIN32
    return _wcsicmp( str1, str2 );
#else
    return wcscasecmp( str1, str2 );
#endif
}

const FdoSmLpPropertyDefinition* FdoSmLpAssociationPropertyDefinition::ColName2Property( 
	const FdoSmLpPropertyDefinitionCollection* pProperties,
	FdoStringP columnName 
)
{
	const FdoSmLpAssociationPropertyDefinition* pMatchedProp = NULL;

	// Search the collection for the property corresponding to the column.
	for ( int i = 0; i < pProperties->GetCount(); i++ ) {
		const FdoSmLpAssociationPropertyDefinition* pAssProp = 
			FdoSmLpAssociationPropertyDefinition::Cast( pProperties->RefItem(i) );

		if ( pAssProp ) {
            FdoSmPhColumnListP colNames = pAssProp->GetIdentityColumns();
            for(int j=0; j<colNames->GetCount(); j++ )
            {
			    if ( util_str_cmp(columnName, colNames->GetString(j)) == 0 ) 
                {
				    pMatchedProp = pAssProp;
				    break;
			    }
            }
            if( pMatchedProp )
                break;
		}
	}

	return(pMatchedProp);
}

FdoSmLpAssociationPropertyDefinition::FdoSmLpAssociationPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent): 
	FdoSmLpPropertyDefinition(propReader, parent),
    mAssociatedClassName( propReader->GetDataType() ),
    mPseudoColumnName( propReader->GetColumnName() ),
    mbColumnAdded( false ),
    mDeleteRule ( (FdoDeleteRule)0 ),
    mbCascadeLock (false ),
    mMultiplicity (L"m"),
    mReverseMultiplicity(L"0_1"),
    mReverseName(L"")

{
    mpIndProperties = FdoStringCollection::Create();
    mpReverseIndProperties = FdoStringCollection::Create();
    mpIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    mpReverseIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
}

FdoSmLpAssociationPropertyDefinition::FdoSmLpAssociationPropertyDefinition(
    FdoAssociationPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
): 
	FdoSmLpPropertyDefinition(pFdoProp, bIgnoreStates, parent),
    mbColumnAdded( false ),
    mDeleteRule ( (FdoDeleteRule)0 ),
    mbCascadeLock (false ),
    mMultiplicity (L"m"),
    mReverseMultiplicity(L"0_1"),
    mReverseName(L"")
{
    mpIndProperties = FdoStringCollection::Create();
    mpReverseIndProperties = FdoStringCollection::Create();
    mpIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    mpReverseIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    SetReadOnly( pFdoProp->GetIsReadOnly() );
    mReverseName = pFdoProp->GetReverseName();
}

FdoSmLpAssociationPropertyDefinition::FdoSmLpAssociationPropertyDefinition(
    FdoSmLpAssociationPropertyP pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
):
    FdoSmLpPropertyDefinition(
        FDO_SAFE_ADDREF((FdoSmLpAssociationPropertyDefinition*) pBaseProperty), 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
    mbColumnAdded( false ),
    mDeleteRule ( (FdoDeleteRule)0 ),
    mbCascadeLock (false ),
    mMultiplicity (L"m"),
    mReverseMultiplicity(L"0_1"),
    mReverseName(L"")
{

	if( GetElementState() == FdoSchemaElementState_Added )
    {
        FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
        FdoSmPhDbObjectP pPhDbObject = pPhysical->FindDbObject(pTargetClass->GetDbObjectName());
        SetContainingDbObject(pPhDbObject, pTargetClass->GetDbObjectName());
	}
 
    mAssociatedClassName = pBaseProperty->GetAssociatedClassName();
    mDeleteRule = pBaseProperty->GetDeleteRule();
    mbCascadeLock = pBaseProperty->GetCascadeLock();
    mMultiplicity = pBaseProperty->GetMultiplicity();
    mReverseMultiplicity = pBaseProperty->GetReverseMultiplicity();
    mpIndProperties = pBaseProperty->GetIdentityProperties();
    mpReverseIndProperties = pBaseProperty->GetReverseIdentityProperties();
    mpIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    mpReverseIdentColumns = FdoSmPhColumnList::Create( GetLogicalPhysicalSchema()->GetPhysicalSchema() );
    mReverseName = pBaseProperty->GetReverseName();

}

FdoSmLpAssociationPropertyDefinition::~FdoSmLpAssociationPropertyDefinition(void)
{
	
}

void FdoSmLpAssociationPropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
	bool		same = true;

    ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();

	if ( (pBaseProp->GetElementState() != FdoSchemaElementState_Deleted) &&
		 (GetElementState() != FdoSchemaElementState_Deleted) ) {

		const FdoSmLpAssociationPropertyDefinition* pBaseAssoProp = 
			FdoSmLpAssociationPropertyDefinition::Cast( pBaseProp );

		if ( pBaseAssoProp ) {

			// Inherited property can't redefine the base property. Check if it
			// redefines it by checking if any characteristics are different.
			if ( mAssociatedClassName != pBaseAssoProp->GetAssociatedClassName() ||
                mDeleteRule != pBaseAssoProp->GetDeleteRule() ||
                mbCascadeLock != pBaseAssoProp->GetCascadeLock() ||
                util_str_cmp( mMultiplicity, pBaseAssoProp->GetMultiplicity() ) != 0 ||
                util_str_cmp( mReverseName, pBaseAssoProp->GetReverseName() ) != 0 ||
                util_str_cmp( mReverseMultiplicity, pBaseAssoProp->GetReverseMultiplicity() ) != 0 )
				same = false;

			// Inherited property redefines base, log an error
			if ( !same ) 
				AddRedefinedError( pBaseAssoProp );
		}		 
	}

	if ( same ) 
		FdoSmLpPropertyDefinition::SetInherited( pBaseProp );
}

FdoPropertyType FdoSmLpAssociationPropertyDefinition::GetPropertyType() const
{
	return FdoPropertyType_AssociationProperty;
}

FdoString* FdoSmLpAssociationPropertyDefinition::GetAssociatedClassName() const
{
	return mAssociatedClassName;
}

const FdoSmLpClassDefinition* FdoSmLpAssociationPropertyDefinition::RefAssociatedClass() const
{
    return RefLogicalPhysicalSchema()->FindClass( mAssociatedClassName );
}

void FdoSmLpAssociationPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalClassMapping* pClassOverrides,
    bool bIgnoreStates
)
{
    // No schema overrides for association properties yet.
    Update( pFdoProp, elementState, (FdoPhysicalPropertyMapping*) NULL, bIgnoreStates );
}

void FdoSmLpAssociationPropertyDefinition::Update(
    FdoPropertyDefinition* pFdoProp,
    FdoSchemaElementState elementState,
    FdoPhysicalPropertyMapping* pPropOverrides,
    bool bIgnoreStates
)
{

	FdoSmLpPropertyDefinition::Update(pFdoProp, elementState, pPropOverrides, bIgnoreStates);

	if ( pFdoProp->GetPropertyType() == FdoPropertyType_AssociationProperty ) 
    {
        FdoPtr<FdoDataPropertyDefinitionCollection>  pFdoPropCol;
        FdoPtr<FdoClassDefinition>   pFdoClass;
        FdoAssociationPropertyDefinition* pFdoAssociationProp = (FdoAssociationPropertyDefinition*) pFdoProp;
        mDeleteRule = pFdoAssociationProp->GetDeleteRule();
        mbCascadeLock = pFdoAssociationProp->GetLockCascade();
        SetReadOnly( pFdoAssociationProp->GetIsReadOnly() );

        pFdoClass = pFdoAssociationProp->GetAssociatedClass();
        if( pFdoClass == NULL )
			//Association property '%1$ls' is missing the associated class",
            throw FdoSchemaException::Create( 
                FdoSmError::NLSGetMessage(
                    FDO_NLSID(FDOSM_382),
				    (FdoString*) GetQName()
                )  
            );

        // Get attributes from FDO element if new object or object 
        // originated directly from FDO.
        if ( (GetElementState() == FdoSchemaElementState_Added) || GetIsFromFdo() ) {
            mAssociatedClassName = pFdoClass->GetName();
            mMultiplicity = pFdoAssociationProp->GetMultiplicity();
            mReverseMultiplicity = pFdoAssociationProp->GetReverseMultiplicity();
            mReverseName = pFdoAssociationProp->GetReverseName();
            // Initialize the identity collection
            pFdoPropCol = ( pFdoAssociationProp->GetIdentityProperties() );

            // 
            // The identity and the reverse identity are provided
            if( pFdoPropCol != NULL && pFdoPropCol->GetCount() != 0 )
            {
                for(int i=0; i<pFdoPropCol->GetCount(); i++ )
                {
                    FdoPtr<FdoDataPropertyDefinition>  fdoProp = pFdoPropCol->GetItem(i);
                    if( fdoProp )
                    {
                        mpIndProperties->Add( fdoProp->GetName() );
                    }
                }
                // Initialize the reverse identity collection
                pFdoPropCol = ( pFdoAssociationProp->GetReverseIdentityProperties() );
                for(int i=0; i<pFdoPropCol->GetCount(); i++ )
                {
                    FdoPtr<FdoDataPropertyDefinition>  fdoProp = pFdoPropCol->GetItem(i);
                    if( fdoProp )
                    {
                        mpReverseIndProperties->Add( fdoProp->GetName() );
                    }
                }
            }
            // else see finalize where the identities and reverse identities are auto-generated
        }
        else if ( GetElementState() == FdoSchemaElementState_Modified ) {
            if( mAssociatedClassName != FdoPtr<FdoClassDefinition>(pFdoClass)->GetName() )
                ASSOCIATEDCLASS_CHG_ERROR;
                
            if( util_str_cmp( mMultiplicity, pFdoAssociationProp->GetMultiplicity() ) )
                MULTIPLICITY_CHG_ERROR;
                
            if( util_str_cmp( mReverseMultiplicity, pFdoAssociationProp->GetReverseMultiplicity() ) )
                REV_MULTIPLICITY_CHG_ERROR;
        }
	}
}

void FdoSmLpAssociationPropertyDefinition::SynchPhysical(bool bRollbackOnly)
{	
	Finalize();
}
 
void FdoSmLpAssociationPropertyDefinition::Commit( bool fromParent)
{
	FdoSmPhMgrP                     pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSmPhPropertyWriterP          pWriter = pPhysical->GetPropertyWriter();
    FdoSmPhAssociationWriterP       pAssocWriter = pPhysical->GetAssociationWriter();
	FdoSmLpClassDefinition*		    pClass = (FdoSmLpClassDefinition*) RefParentClass();
    const FdoSmLpClassDefinition*   pAssociatedClass = RefAssociatedClass();
    FdoStringP				        fullName = GetName();
	char*							pUser = "fdo_user"; //TODO: pConn->GetUser();
	FdoStringP				        dbUser(pUser);
    int								rowsProcessed =0;
    FdoStringsP                     srcColNames = FdoStringCollection::Create();
    FdoStringsP                     associatedColNames = FdoStringCollection::Create();
    FdoStringP                      pkTableName = pClass->GetDbObjectName();
    FdoStringP                      fkTableName = pAssociatedClass->GetDbObjectName();
    bool                            failed = false;
	FdoStringP				        sqlCmd;

//TODO:    delete [] pUser;

    FdoSmPhDbObjectP pPhTable = pPhysical->FindDbObject(pClass->GetDbObjectName());
    mPseudoColumnName = pClass->UniqueColumnName( pPhTable, this, fullName );

	switch ( GetElementState() ) 
    {
	    case FdoSchemaElementState_Added:
			pWriter->SetTableName( GetContainingDbObjectName() );
			pWriter->SetClassId( pClass->GetId() );  
			pWriter->SetColumnName( mPseudoColumnName ); // this is required to create a unique key in the f_attributedefinition table
			pWriter->SetName( fullName );
			pWriter->SetColumnType( L"Association"  );
			pWriter->SetDataType( pAssociatedClass ? pAssociatedClass->GetQName() : L"" );
			pWriter->SetIsNullable( true );
			pWriter->SetIsFeatId( false );
			pWriter->SetIsSystem( GetIsSystem() );
			pWriter->SetIsReadOnly(	GetReadOnly() );
			pWriter->SetUser( dbUser );
			pWriter->SetDescription( GetDescription() );
			pWriter->Add();

            if ( pkTableName.GetLength() != 0 || fkTableName.GetLength() != 0 ) 
            {       
                pAssocWriter->SetPseudoColumnName( mPseudoColumnName );
                pAssocWriter->SetPkTableName( pkTableName );
			    pAssocWriter->SetPkColumnNames( mpReverseIdentColumns );
                pAssocWriter->SetFkTableName( fkTableName );
			    pAssocWriter->SetFkColumnNames( mpIdentColumns );
			    pAssocWriter->SetMultiplicity( mMultiplicity );
			    pAssocWriter->SetReverseMultiplicity( mReverseMultiplicity );
			    pAssocWriter->SetCascadeLock( mbCascadeLock );
                pAssocWriter->SetDeleteRule( DeleteRuleSqlValue() );
                pAssocWriter->SetReverseName( mReverseName );
                pAssocWriter->Add();
            }

		    break;

	    case FdoSchemaElementState_Deleted:
            pWriter->Delete( pClass->GetId(), fullName );

            // Delete the association definition row
		    if ( pkTableName.GetLength() != 0 && fkTableName.GetLength() != 0 ) 
                pAssocWriter->Delete( pkTableName, fkTableName );
		    
		    break;

	    case FdoSchemaElementState_Modified:
		    pWriter->SetDescription( GetDescription() );
            pWriter->Modify( pClass->GetId(), fullName );

            if ( pkTableName.GetLength() != 0 || fkTableName.GetLength() != 0 ) 
            {      
			    pAssocWriter->SetCascadeLock( mbCascadeLock );
                pAssocWriter->SetDeleteRule( DeleteRuleSqlValue() );
                pAssocWriter->Modify( pkTableName, fkTableName );
            }

            break;
	}

}

int  FdoSmLpAssociationPropertyDefinition::DeleteRuleSqlValue()
{
    switch( mDeleteRule )
    {
        case FdoDeleteRule_Cascade: return 1;
        case FdoDeleteRule_Prevent: return 2;
        case FdoDeleteRule_Break:   return 3;
    }

    return 0;
}

FdoDeleteRule  FdoSmLpAssociationPropertyDefinition::DeleteRuleValue( int sqlVal )
{
    switch( sqlVal )
    {
        case 1: return FdoDeleteRule_Cascade;
        case 2: return FdoDeleteRule_Prevent;
        case 3: return FdoDeleteRule_Break;
    }
    return FdoDeleteRule_Break;
}

FdoSchemaExceptionP FdoSmLpAssociationPropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();

	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	return pException;
}


void FdoSmLpAssociationPropertyDefinition::LoadAssociationDefinition( const FdoSmLpClassDefinition* pParent )
{
	// Get the f_associationdefinition row corresponding to this
	// association property.
    const FdoSmLpClassDefinition*   pAssociatedClass = RefAssociatedClass();
	
    if ( pAssociatedClass ) {
        FdoSmPhMgrP                     pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
        FdoSmPhAssociationReaderP rdr = pPhysical->CreateAssociationReader(
            pParent->GetDbObjectName(), 
            pAssociatedClass->GetDbObjectName(), 
            true );

	    while ( rdr->ReadNext() ) 
        {
            FdoStringP pseudoColName = rdr->GetPseudoColumnName();
            if( wcscmp(pseudoColName, mPseudoColumnName ) == 0 )
            {
                mDeleteRule = DeleteRuleValue(rdr->GetDeleteRule());
                mbCascadeLock = rdr->GetCascadelock();
                mMultiplicity = rdr->GetMultiplicity();
                mReverseMultiplicity = rdr->GetReverseMultiplicity();
                mpReverseIdentColumns = rdr->GetPkColumnNames();
                mpIdentColumns = rdr->GetFkColumnNames();
                mReverseName = rdr->GetReverseName();
                mbColumnAdded = true;
                break;
            }
	    }
    }
}

void FdoSmLpAssociationPropertyDefinition::InitIdentyProperties()
{
    // Let's see if this association is the reverse of an existing association. i.e It was added as a result of initializing the 
    // reverse name of an association. The reverse association should be marked read-only.
    if( this->GetReadOnly() )
    {
        // In this case the identities and reverse identities are reversed.
        // We need to find the original association property.
        const FdoSmLpClassDefinition*      pAssociatedClass = RefAssociatedClass();
        for(int i=0; i<pAssociatedClass->RefProperties()->GetCount(); i++ )
        {
            const FdoSmLpPropertyDefinition  *prop = pAssociatedClass->RefProperties()->RefItem( i );
            if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
            {
                const FdoSmLpAssociationPropertyDefinition* assocProp = (const FdoSmLpAssociationPropertyDefinition*)prop;
                const FdoSmLpClassDefinition*      pClass = RefParentClass();
                const FdoSmLpClassDefinition*      pAssocClass = assocProp->RefAssociatedClass();
                // pClass and pAssocClass should be the same
                if( FdoStringP(pClass->GetName()) == pAssocClass->GetName() )
                {
                    mpReverseIndProperties = assocProp->GetIdentityProperties();
                    mpIndProperties = assocProp->GetReverseIdentityProperties();
                    return;
                }
            }
        }
    }
    if( mpReverseIndProperties->GetCount() == 0 )
    {
        const FdoSmLpClassDefinition*      pClass = RefParentClass();
        for(int i=0; i<mpReverseIdentColumns->GetCount(); i++ )
        {
            const FdoSmLpDataPropertyDefinition  *prop = FdoSmLpDataPropertyDefinitionCollection::ColName2Property( pClass->RefProperties(),
                mpReverseIdentColumns->GetString(i) );
            
            if( prop ) 
            {
                mpReverseIndProperties->Add( prop->GetName() );
            }
            else
            {
                // It's OK to have all the columns without corresponding properties. This is due to the fact that the reverse
                // identity is optional. If not all the columns are missing properties, then it's an meta data integrity error.
                if( mpReverseIndProperties->GetCount() != 0 )
    		        //Association property %1$ls load error
                    throw FdoSchemaException::Create(
                        FdoSmError::NLSGetMessage(
                            FDO_NLSID(FDOSM_287),
    			            (FdoString*) GetQName()
                        )
			        );
            }
        }
    }

    // We only build the identity properties if the reverse identity properties collection is not empty.
    if( mpIndProperties->GetCount() == 0 && mpReverseIndProperties->GetCount() != 0 )
    {
        const FdoSmLpClassDefinition*      pAssociatedClass = RefAssociatedClass();
        for(int i=0; i<mpIdentColumns->GetCount(); i++ )
        {
            const FdoSmLpDataPropertyDefinition  *prop = FdoSmLpDataPropertyDefinitionCollection::ColName2Property( pAssociatedClass->RefProperties(),
                mpIdentColumns->GetString(i) );
            if( prop == NULL )
            {  // This indicates a meta data integrety error.
               //Association property %1$ls load error
                throw FdoSchemaException::Create(
                    FdoSmError::NLSGetMessage(
                        FDO_NLSID(FDOSM_287),
				        (FdoString*) GetQName()
			        )
                );
            }
            else
            {
                mpIndProperties->Add( prop->GetName() );
            }
        }
    } 
}

const FdoStringsP FdoSmLpAssociationPropertyDefinition::GetIdentityProperties() const
{ 
    ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
    ((FdoSmLpAssociationPropertyDefinition*)this)->InitIdentyProperties();
    return mpIndProperties; 
}

const FdoStringsP FdoSmLpAssociationPropertyDefinition::GetReverseIdentityProperties() const
{ 
    ((FdoSmLpAssociationPropertyDefinition*) this)->Finalize();
    ((FdoSmLpAssociationPropertyDefinition*)this)->InitIdentyProperties();
    return mpReverseIndProperties; 
}

void FdoSmLpAssociationPropertyDefinition::Finalize()
{
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // Finalize is not re-entrant. If already finalizing then
	// the class for this association property must be including this
	// association property. At some level it contains an association property
	// whose class is this association property's defining class. 

	if ( GetState() == FdoSmObjectState_Finalizing ) 
    {
		return;
	}

	if ( GetState() == FdoSmObjectState_Initial ) 
    {
        FdoSmLpPropertyDefinition::Finalize();


        SetState( FdoSmObjectState_Finalizing);

        // For performance, load this property from RDBMS only if not new and not 
        // from a config doc. 
        // When property is new, it is not in the RDBMS.
        // When property is from config doc, it is fully defined by the config doc.
        if( (GetElementState() != FdoSchemaElementState_Added) && !GetIsFromConfig() )
            LoadAssociationDefinition( RefParentClass() );

        FdoSmLpAssociationPropertyP pPrevProp = GetPrevProperty().p->SmartCast<FdoSmLpAssociationPropertyDefinition>();

        // New property in same table as the base property. New property
        // just latches onto base property column.
        if ( pPrevProp && (util_str_cmp( this->GetContainingDbObjectName(), GetPrevProperty()->GetContainingDbObjectName() ) == 0) )
        {
            mpIdentColumns = pPrevProp->GetIdentityColumns();
            mpReverseIdentColumns  = pPrevProp->GetReverseIdentityColumns();
            mbColumnAdded = true;
        }

        InitIdentyProperties();

        if( mpIndProperties->GetCount() != mpReverseIndProperties->GetCount() )
        {
            IDENTITY_REVIDENT_NOT_EQUAL_ERROR;
    		SetState( FdoSmObjectState_Final);
            return; // No point in getting any further
        }

        //
        // Make sure the identity properties are part of the asociated class properties and are of data
        // property type.
        const FdoSmLpClassDefinition*		pAssociatedClass = RefAssociatedClass();

        if ( !pAssociatedClass ) {
            ASSOCIATED_CLASS_NOT_FOUND_ERROR( ((FdoString*)GetQName()), GetAssociatedClassName() ) ;
    		SetState( FdoSmObjectState_Final);
            return; // No point in getting any further
        }

        const FdoSmLpPropertyDefinitionCollection* lpPropCol = pAssociatedClass->RefProperties();
        FdoSmLpClassDefinition*		pClass = (FdoSmLpClassDefinition*) RefParentClass();
        const FdoSmLpPropertyDefinitionCollection* lpRevPropCol = pClass->RefProperties();

        if ( pAssociatedClass->GetIsAbstract() ) 
            ASSOCIATED_CLASS_ABSTRACT( (FdoString*)pAssociatedClass->GetQName() );

        for(int i=0; i<mpIndProperties->GetCount(); i++ )
        {
            //// Make sure the identity property exist
            const FdoSmLpPropertyDefinition* lpProp = lpPropCol->RefItem( (FdoString*)(mpIndProperties->GetString(i)) );
            if( lpProp == NULL || lpProp->GetPropertyType() != FdoPropertyType_DataProperty )
            {
                IDENTITY_PROP_NOT_FOUND_ERR( (FdoString*)(mpIndProperties->GetString(i)) );
                break;
            }

            //// Make sure the reverse identity property exist
            const FdoSmLpPropertyDefinition* lpRevProp = lpRevPropCol->RefItem( (const wchar_t*)(mpReverseIndProperties->GetString(i)) );
            if( lpRevProp == NULL || lpRevProp->GetPropertyType() != FdoPropertyType_DataProperty )
            {
                REV_IDENTITY_PROP_NOT_FOUND_ERR( (FdoString*)(mpReverseIndProperties->GetString(i)) );
                break;
            }

            if( ((FdoSmLpDataPropertyDefinition*)lpProp)->GetDataType() != ((FdoSmLpDataPropertyDefinition*)lpRevProp)->GetDataType() )
            {
                IDENTITY_REVIDENT_TYPE_MATCH_ERROR((FdoString*)(mpIndProperties->GetString(i)), (const wchar_t*)(mpReverseIndProperties->GetString(i)) );
                break;
            }

            mpIdentColumns->Add( ((FdoSmLpDataPropertyDefinition*)lpProp)->GetColumnName() );
            mpReverseIdentColumns->Add( ((FdoSmLpDataPropertyDefinition*)lpRevProp)->GetColumnName() );

        }

        if ( GetElementState() == FdoSchemaElementState_Added ||
             GetElementState() == FdoSchemaElementState_Unchanged )
        {
            FdoSmLpClassDefinition*		      pParent = (FdoSmLpClassDefinition*) RefParentClass();
 
            // Skip association existence check if from config doc. This helps
            // performance by avoiding unnecessary foreign key loading.
            if ( GetElementState() == FdoSchemaElementState_Added && !GetIsFromConfig() ) 
            {
                // Make sure that an association between these two classes does not already exist
                FdoSmPhAssociationReaderP rdr = pPhysical->CreateAssociationReader( pParent->GetDbObjectName(), pAssociatedClass->GetDbObjectName(), true );
	            if ( rdr->ReadNext() ) 
                {
                  ASSOCIATION_EXIST_ERROR(pParent->GetName(), pAssociatedClass->GetName() );
                }
            }

            if( mpIndProperties->GetCount() == 0 && ! mbColumnAdded )
            {
                // Let's see if this association is the reverse of an existing association. i.e It was added as a result of initializing the 
                // reverse name of an association. The reverse association should be marked read-only.
                if( this->GetReadOnly() )
                {
                    // In this case the identities and reverse identities are reversed.
                    // We need to find the original association property.
                    for(int i=0; i<pAssociatedClass->RefProperties()->GetCount(); i++ )
                    {
                        const FdoSmLpPropertyDefinition  *prop = pAssociatedClass->RefProperties()->RefItem( i );
                        if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty )
                        {
                            const FdoSmLpAssociationPropertyDefinition* assocProp = (const FdoSmLpAssociationPropertyDefinition*)prop;
                            const FdoSmLpClassDefinition*      pClass = RefParentClass();
                            const FdoSmLpClassDefinition*      pAssocClass = assocProp->RefAssociatedClass();
                            // pClass and pAssocClass should be the same
                            if( util_str_cmp( pClass->GetName(), pAssocClass->GetName() ) == 0 )
                            {
                                mpReverseIdentColumns = assocProp->GetIdentityColumns();
                                mpIdentColumns = assocProp->GetReverseIdentityColumns();
                                break;
                            }
                        }
                    }
                }
                else
                {
                    // No identity properties are provided; we need to create the fk/pk linkage internally:
                    // We need to add fk column to the parent class table. Those column should correspond to 
                    // the associated class pk columns.
                    const FdoSmLpDataPropertyDefinitionCollection* pPropCol = pAssociatedClass->RefIdentityProperties();
                    FdoSmPhDbObjectP pPhTable = pPhysical->FindDbObject(RefParentClass()->GetDbObjectName());
                    FdoSmPhDbObjectP pPhAssocTable = pPhysical->FindDbObject(pAssociatedClass->GetDbObjectName());
                    if ( pPhTable ) {
                        for(int i=0; i<pPropCol->GetCount(); i++ )
                        {
                            const FdoSmLpDataPropertyDefinition* pProp = pPropCol->RefItem(i);
                            // Column name is property name adjusted to be RDBMS-friendly.
		                    FdoStringP mColumnName = ((FdoSmLpClassDefinition*) RefParentClass())->UniqueColumnName( pPhTable, this, pProp->GetName() );
                            
                            // Create the column 
    	                    FdoSmPhColumnP pColumn = ((FdoSmLpDataPropertyDefinition*) pProp)->NewColumn( pPhTable, mColumnName, true, L"" );
							pColumn->SetAutoincrement( false ); // Always sets it to false as it's not part of the object key.
                            mpIdentColumns->Add( pProp->GetColumnName() );
                            mpReverseIdentColumns->Add( mColumnName );
                        }
                    }
                }
                mbColumnAdded = true;
            }
        }

		SetState( FdoSmObjectState_Final);
	}
}

void FdoSmLpAssociationPropertyDefinition::XMLSerialize( FILE* xmlFp, int ref ) const
{
	fprintf( xmlFp, "<property xsi:type=\"%ls\" name=\"%ls\" description=\"%ls\" associatedClassName=\"%ls\" >\n",
			(FdoString*) FdoSmLpPropertyTypeMapper::Type2String(GetPropertyType()),
			GetName(), GetDescription(), GetAssociatedClassName()
	);
    //
    // Add the rest of the attributes.

	fprintf( xmlFp, "</property>\n" );
}

