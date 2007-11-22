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
#include "Index.h"
#include "LogicalPhysicalSchema.h"
#include "ClassDefinition.h"
#include "../Physical/IndexWriter.h"
#include "../Physical/IndexPropertyWriter.h"
#include "../Error.h"
#include "../../DbiConnection.h"

FdoRdbmsLpIndex::FdoRdbmsLpIndex(
    FdoRdbmsPhClassIndexReader* pReader, 
	FdoRdbmsLpSchemaElement* parent
) : 
	FdoRdbmsLpSchemaElement(FdoStringP(pReader->GetIndexName()).Upper(), L"", parent ),
	mbIsUnique(pReader->GetIsUnique()),
    mpPrevIndex(NULL),
    mpBaseIndex(NULL),
    mpSrcIndex(NULL),
    mpIndex(NULL)
{
    mPropNames = FdoStringCollection::Create();

    SetElementState( FdoSchemaElementState_Unchanged );
}

FdoRdbmsLpIndex::FdoRdbmsLpIndex(
	FdoOracleOvUniqueKeyDefinition* pOvUniqueKey, 
	FdoRdbmsLpSchemaElement* parent
) : 
	FdoRdbmsLpSchemaElement(FdoStringP(pOvUniqueKey->GetName()).Upper(), L"", parent ),
	mbIsUnique(true),
    mpPrevIndex(NULL),
    mpBaseIndex(NULL),
    mpSrcIndex(NULL),
    mpIndex(NULL)
{
    mPropNames = FdoStringCollection::Create();

    SetElementState(FdoSchemaElementState_Added);

	FdoStringsP props = pOvUniqueKey->GetProperties();
    AddProperties( props );
}

FdoRdbmsLpIndex::FdoRdbmsLpIndex(
		FdoOracleOvIndexDefinition* pOvIndex, 
		FdoRdbmsLpSchemaElement* parent
) : 
	FdoRdbmsLpSchemaElement(pOvIndex->GetName(), L"", parent ),
	mbIsUnique(false),
    mpPrevIndex(NULL),
    mpBaseIndex(NULL),
    mpSrcIndex(NULL),
    mpIndex(NULL)
{
    mPropNames = FdoStringCollection::Create();

    SetElementState(FdoSchemaElementState_Added);

	
	FdoStringsP props = pOvIndex->GetProperties();	
    AddProperties( props );
}

FdoRdbmsLpIndex::FdoRdbmsLpIndex(
    const FdoRdbmsLpIndex* pBaseIndex, 
    FdoRdbmsLpClassDefinition* pTargetClass,
    bool bInherit
) : 
	FdoRdbmsLpSchemaElement(pBaseIndex->GetName(), L"", pTargetClass),
	mbIsUnique(pBaseIndex->GetIsUnique()),
    mpPrevIndex(pBaseIndex),
    mpBaseIndex(NULL),
    mpSrcIndex(NULL)
{
    int idx;

    mPropNames = FdoStringCollection::Create();

    // Need to add index if either the base index or target class are new
	if ( (pTargetClass->GetElementState() == FdoSchemaElementState_Added) ||
		 (pBaseIndex->GetElementState() == FdoSchemaElementState_Added) )
		SetElementState( FdoSchemaElementState_Added );

    // Need to delete index if either the base index or target class are to be deleted.
	if ( (pTargetClass->GetElementState() == FdoSchemaElementState_Deleted) ||
		 (pBaseIndex->GetElementState() == FdoSchemaElementState_Deleted) )
		SetElementState( FdoSchemaElementState_Deleted );

    if ( bInherit ) 
        mpBaseIndex = pBaseIndex->GetBaseIndex() ? pBaseIndex->GetBaseIndex() : pBaseIndex;
    else
        mpSrcIndex = pBaseIndex->GetBaseIndex() ? pBaseIndex->GetBaseIndex() : pBaseIndex;

    for ( idx = 0; idx < pBaseIndex->GetProperties()->GetCount(); idx++ ) {
        const FdoRdbmsLpDataPropertyDefinition* pBaseProp = pBaseIndex->GetProperties()->GetItem(idx);

        AddProperty( pBaseProp->GetName() );
    }
}

FdoRdbmsLpIndex::~FdoRdbmsLpIndex(void)
{
}

const FdoRdbmsPhIndex* FdoRdbmsLpIndex::GetIndex() const
{
	return ((FdoRdbmsLpIndex*)this)->Get_Index();
}

FdoRdbmsPhIndex* FdoRdbmsLpIndex::Get_Index()
{
    Finalize();

	return mpIndex;
}

const FdoRdbmsLpClassDefinition* FdoRdbmsLpIndex::GetDefiningClass() const
{
    if ( mpBaseIndex )
        return mpBaseIndex->GetDefiningClass();
    else
        return (const FdoRdbmsLpClassDefinition*) GetParent();
}

FdoStringP FdoRdbmsLpIndex::GetQName() const
{
    return GetParent()->GetQName() + L"." + GetName();
}

FdoOracleOvUniqueKeyP FdoRdbmsLpIndex::GetUniqueSchemaMappings() const
{
    FdoOracleOvUniqueKeyP           uniqueKey;
    
    if ( GetIsUnique() ) {
        uniqueKey = FdoOracleOvUniqueKeyDefinition::Create( GetName() );

        ((FdoRdbmsLpIndex*)this)->Finalize();

        GetPropertyMappings( uniqueKey->GetProperties() );
    }

    return uniqueKey;
}

FdoOracleOvIndexP FdoRdbmsLpIndex::GetNonUniqueSchemaMappings() const
{
    FdoOracleOvIndexP           index;
    
    if ( !GetIsUnique() ) {
        index = FdoOracleOvIndexDefinition::Create( GetName() );

        ((FdoRdbmsLpIndex*)this)->Finalize();

        GetPropertyMappings( index->GetProperties() );
    }

    return index;
}

void FdoRdbmsLpIndex::GetPropertyMappings( FdoStringsP propMappings ) const
{
    int i;

    for ( i = 0; i < mProperties.GetCount(); i++ )
        propMappings->Add( mProperties.GetItem(i)->GetName() );
}

void FdoRdbmsLpIndex::AddProperty( FdoString* pPropName )
{
    mPropNames->Add( pPropName );
}

void FdoRdbmsLpIndex::AddProperty( FdoRdbmsLpDataPropertyDefinition* pProp )
{
    // Properties cannot be added after index has been finalized.
    if ( GetState() == FdoRdbmsSmObjectState_Final ) 
        AddPropAfterFinalError( pProp->GetName() );
    else
    	mProperties.Add( pProp );
}

FdoSchemaException* FdoRdbmsLpIndex::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    ((FdoRdbmsLpIndex*) this)->Finalize();

	FdoSchemaException* pException = FdoRdbmsSmSchemaElement::Errors2Exception(pFirstException);

	if ( mpIndex ) 
		pException = mpIndex->Errors2Exception(pException);

	return pException;
}

void FdoRdbmsLpIndex::Update( FdoOracleOvUniqueKeyDefinition* pOvUniqueKey )
{
    // Index uniqueness change not supported.
    if ( !mbIsUnique )
        AddUniqueChangeError();

    FdoStringsP props = pOvUniqueKey->GetProperties();
    UpdateProperties( props );
}

void FdoRdbmsLpIndex::Update( FdoOracleOvIndexDefinition* pOvIndex )
{
    // Index uniqueness change not supported.
    if ( mbIsUnique )
        AddUniqueChangeError();

    FdoStringsP props = pOvIndex->GetProperties();
    UpdateProperties( props );
}

void FdoRdbmsLpIndex::UpdateProperties( FdoStringsP& propNames )
{
    int idx;

    if ( propNames->GetCount() == 0 ) {
        // When Schema Mappings specify an index without properties, this indicates
        // that the index is to be dropped.
        mProperties.Clear();
        mPropNames->Clear();

        if ( GetElementState() == FdoSchemaElementState_Added ) 
            SetElementState( FdoSchemaElementState_Detached );
        else
            SetElementState( FdoSchemaElementState_Deleted );
    }
    else {
        if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
            SetElementState( FdoSchemaElementState_Modified );

        // Otherwise, check if index property list is being change. This type
        // of change is not supported.
        if ( propNames->GetCount() != mPropNames->GetCount() ) {
            AddPropChangeError();
        }
        else {
            for ( idx = 0; idx < propNames->GetCount(); idx++ ) {
                if ( FdoStringP(propNames->GetString(idx)) != mPropNames->GetString(idx) )
                    AddPropChangeError();
            }
        }
    }
}

void FdoRdbmsLpIndex::Commit()
{
	FdoRdbmsPhysicalSchema* pPhysical = Get_LogicalPhysicalSchema()->Get_PhysicalSchema();
	DbiConnection* pConn = pPhysical->GetConnection();
    FdoRdbmsPhIndexWriter* pIndexWriter = NULL;
    FdoRdbmsPhIndexPropertyWriter* pPropertyWriter = NULL;
    const FdoRdbmsLpClassDefinition* pClass = (const FdoRdbmsLpClassDefinition*) GetParent();
    int i;

    Finalize();

    if ( !GetBaseIndex() ) {
	    switch ( GetElementState() ) {
  	    case FdoSchemaElementState_Added:
            pIndexWriter = pPhysical->GetIndexWriter();
            
            pIndexWriter->SetClassId( pClass->GetId() );
            pIndexWriter->SetName( GetName() );
            pIndexWriter->SetIsUnique( mbIsUnique );
            pIndexWriter->Add();

            for ( i = 0; i < mProperties.GetCount(); i++ ) {
                pPropertyWriter = pPhysical->GetIndexPropertyWriter();
                pPropertyWriter->SetIndexName( GetName() );
                pPropertyWriter->SetAttributeName( mProperties.GetItem(i)->GetName() );
                pPropertyWriter->SetPosition( i + 1 );
                pPropertyWriter->Add();
            }

		    break;

	    case FdoSchemaElementState_Deleted:
            pIndexWriter = pPhysical->GetIndexWriter();
            pIndexWriter->Delete( GetName() );

		    break;
	    }
    }
}

FdoRdbmsLpIndex* FdoRdbmsLpIndex::CreateInherited( FdoRdbmsLpClassDefinition* pSubClass ) const
{
    return new FdoRdbmsLpIndex( this, pSubClass, true );
}

FdoRdbmsLpIndex* FdoRdbmsLpIndex::CreateCopy( FdoRdbmsLpClassDefinition* pSubClass ) const
{
    return new FdoRdbmsLpIndex( this, pSubClass, false );
}

void FdoRdbmsLpIndex::SetInherited( const FdoRdbmsLpIndex* pBaseIndex )
{
    mpBaseIndex = pBaseIndex->GetBaseIndex() ? pBaseIndex->GetBaseIndex() : pBaseIndex;
    mpSrcIndex = NULL;
    mpPrevIndex = pBaseIndex;

	if ( (pBaseIndex->GetElementState() != FdoSchemaElementState_Deleted) &&
		 (GetElementState() != FdoSchemaElementState_Deleted) ) {
        
        // Make sure this index and base index are compatible.

        // They must have the same uniqueness
        if ( GetIsUnique() != pBaseIndex->GetIsUnique() )
            AddRedefinitionError(pBaseIndex);

        // They must have the same property list
        int baseCount = pBaseIndex->GetProperties()->GetCount();
        int idx = 0;

        if ( baseCount != mProperties.GetCount() ) {
            AddRedefinitionError(pBaseIndex);
        }
        else {
            for ( idx = 0; idx < baseCount; idx++ ) {
                const FdoRdbmsLpDataPropertyDefinition* pBaseProp = pBaseIndex->GetProperties()->GetItem(idx);
                const FdoRdbmsLpDataPropertyDefinition* pProp = GetProperties()->GetItem(idx);
            
                if ( FdoStringP(pBaseProp->GetName()) != pProp->GetName() )
                    AddRedefinitionError(pBaseIndex);
            }
        }
    }
}

void FdoRdbmsLpIndex::XMLSerialize( FILE* xmlFp, int ref ) const
{
    ((FdoRdbmsLpIndex*) this)->Finalize();

    fprintf( xmlFp, "<index name=\"%s\" unique=\"%ls\">\n",
			(const char*) FdoStringP(GetName()),
            mbIsUnique ? L"true" : L"false"
	);

	if ( ref == 0 ) {
		for ( int i = 0; i < GetProperties()->GetCount(); i++ ) 
			GetProperties()->GetItem(i)->XMLSerialize(xmlFp, 1);
	
        if ( GetIndex() ) 
            GetIndex()->XMLSerialize(xmlFp, 0);
    }

	fprintf( xmlFp, "</index>\n" );

}

void FdoRdbmsLpIndex::AddProperties( FdoStringsP& propNames)
{
    int idx;

    for ( idx = 0; idx < propNames->GetCount(); idx++ )
        AddProperty( propNames->GetString(idx) );
}


void FdoRdbmsLpIndex::Finalize()
{
    int i;
	FdoRdbmsPhysicalSchema* pPhysical = Get_LogicalPhysicalSchema()->Get_PhysicalSchema();

    if ( GetState() == FdoRdbmsSmObjectState_Finalizing ) {
		// Don't worry about errors for elements that are going away.
		if ( GetElementState() != FdoSchemaElementState_Deleted ) 
			AddFinalizeLoopError();
        return;
    }

    if ( GetState() != FdoRdbmsSmObjectState_Final ) {
        SetState( FdoRdbmsSmObjectState_Finalizing );

        const FdoRdbmsLpClassDefinition* pClass = (const FdoRdbmsLpClassDefinition*) GetParent();

        if ( pClass ) {
            for ( i = 0; i < mPropNames->GetCount(); i++ ) {
                const FdoRdbmsLpDataPropertyDefinition* pProp = 
                    FdoRdbmsLpDataPropertyDefinition::Cast( pClass->GetProperties()->GetItem(mPropNames->GetString(i)) );
    
                if ( pProp ) {
                    // Cannot delete a property if it is referenced by an index
                    if ( pProp->GetElementState() == FdoSchemaElementState_Deleted  &&
                         GetElementState() != FdoSchemaElementState_Deleted )
                        AddPropDeleteError( pProp );
                    else
                        AddProperty( (FdoRdbmsLpDataPropertyDefinition*) pProp );
                }
                else {
                    // Error if any index property does not exist.
                    AddPropNotFoundError( mPropNames->GetString(i) );
                }
            }

            const FdoRdbmsLpTable* pLpTable = pClass->GetTable();

            // Find the physical index on the containing class's physical table.

            if ( pLpTable ) {
                FdoRdbmsPhTable* pPhTable = (FdoRdbmsPhTable*)(pLpTable->GetTable());

                if ( pPhTable ) {
                    mpIndex = (FdoRdbmsPhIndex*)(pPhTable->GetIndexes()->GetItem(GetName()));
                
                    if ( mProperties.GetCount() > 0 ) {
                        if ( mpIndex ) {
                            // Physical Index found, so make sure that the uniquenesses and
                            // property lists, for the LogicalPhysical and physical indexes,
                            // agree.

                            if ( GetIsUnique() != mpIndex->GetIsUnique() )
                                AddLpUniqueError();

                            if ( mProperties.GetCount() != mpIndex->GetColumns()->GetCount() ) {
                                AddLpPropColError();
                            }
                            else {
                                for ( i = 0; i < mProperties.GetCount(); i++ ) {
                                    if ( FdoStringP(mProperties.GetItem(i)->GetColumnName()).ICompare(mpIndex->GetColumns()->GetItem(i)->GetName()) != 0 ) 
                                        AddLpPropColError();
                                }
                            }
                        }
                        else {
                            if ( (GetElementState() != FdoSchemaElementState_Unchanged) &&
                                 (GetElementState() != FdoSchemaElementState_Deleted ) ) {
                                // Physical index does not exist, so create it.
                                mpIndex = new FdoRdbmsPhIndex( GetName(), pPhTable, pPhysical, mbIsUnique, false );

                                for ( i = 0; i < mProperties.GetCount(); i++ ) {
                                    mpIndex->AddColumn( (FdoRdbmsPhColumn*)(mProperties.Get_Item(i)->GetColumn()) );
                                }
                            }
                        }
                    }
                    else {
                        if ( mpIndex ) {
                            // Physical Index exists but deleting LogicalPhysical index.
                            // Delete Physical index if it is not shared by base index.
                            if ( (!GetBaseIndex()) || 
                                 ( FdoStringP(GetPrevIndex()->GetIndex()->GetParent()->GetName()).ICompare(mpIndex->GetParent()->GetName()) != 0 )
                            ) {
                                mpIndex->SetElementState( FdoSchemaElementState_Deleted );
                            }
                        }
                    }
                }
            }
        }

        SetState( FdoRdbmsSmObjectState_Final );
    }
}

void FdoRdbmsLpIndex::AddPropNotFoundError( FdoString* pPropName )
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet3(
				FDORDBMS_341,
				"Property %1$ls is in index %2$ls but not class %3$ls.",
				pPropName,
                GetName(),
                (FdoString*)GetParent()->GetQName() 
			)
		)
	);
}

void FdoRdbmsLpIndex::AddPropAfterFinalError( FdoString* pPropName )
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet2(
				FDORDBMS_342,
				"Index %1$ls has been finalized, cannot add property %2$ls.",
                (FdoString*) GetQName(),
				pPropName
			)
		)
	);

    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
}

void FdoRdbmsLpIndex::AddUniqueChangeError()
{
	if ( GetIsUnique() ) {
		mpErrors->Add( FdoRdbmsSmErrorType_Other, 
			NlsMsgGet1( 
				FDORDBMS_343,
				"Cannot change index %1$ls to be non-unique",
				(FdoString*) GetQName()
			)
		);
	}
	else {
		mpErrors->Add( FdoRdbmsSmErrorType_Other, 
			NlsMsgGet1( 
				FDORDBMS_344,
				"Cannot change index %1$ls to be unique",
				(FdoString*) GetQName()
			)
		);
	}

    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
}

void FdoRdbmsLpIndex::AddPropChangeError()
{
	mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		NlsMsgGet1( 
			FDORDBMS_345,
			"Cannot change property list for index %1$ls",
			(FdoString*) GetQName()
		)
    );

    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
}

void FdoRdbmsLpIndex::AddRedefinitionError( const FdoRdbmsLpIndex* pBaseIndex )
{
	mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		NlsMsgGet2( 
			FDORDBMS_346,
			"Index %1$ls redefines the characteristics of base index %2$ls",
			(FdoString*) GetQName(),
            (FdoString*) pBaseIndex->GetQName()
		)
    );
}

void FdoRdbmsLpIndex::AddColNotFoundError( FdoString* pColName )
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet3(
				FDORDBMS_347,
				"Column %1$ls is in index %2$ls but no property in class %3$ls references it",
				pColName,
                GetName(),
                (FdoString*) GetParent()->GetQName() 
			)
		)
	);
}

void FdoRdbmsLpIndex::AddLpUniqueError()
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet1(
				FDORDBMS_349,
				"Logical and Physical versions of index '%1$ls' have different uniqueness settings",
                (FdoString*) GetQName()
			)
		)
	);
}

void FdoRdbmsLpIndex::AddLpPropColError()
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet1(
				FDORDBMS_350,
				"Properties for index '%1$ls' do not match the columns in its physical index",
                (FdoString*) GetQName()
			)
		)
	);
}

void FdoRdbmsLpIndex::AddPropDeleteError( const FdoRdbmsLpDataPropertyDefinition* pProp)
{
    mpErrors->Add( FdoRdbmsSmErrorType_Other, 
		FdoStringP( 
			NlsMsgGet2(
				FDORDBMS_365,
				"Cannot delete property '%1$ls'; it is in index '%2$ls'",
                (FdoString*) pProp->GetQName(),
                (FdoString*) GetQName()
			)
		)
	);

    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
}

