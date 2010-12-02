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
#include <Sm/Lp/ClassDefinition.h>
#include <Sm/Lp/SimplePropertyDefinition.h>
//#include <Sm/Lp/DataTypeMapper.h>
//#include <Sm/Lp/Table.h>
#include <Sm/Lp/Schema.h>
#include <Sm/Error.h>
#include <Sm/Ph/DependencyCollection.h>

FdoSmLpSimplePropertyDefinition::FdoSmLpSimplePropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent) : 
	FdoSmLpPropertyDefinition(propReader, parent),
    mbNullable( propReader->GetIsNullable() ),
	mColumnName( propReader->GetColumnName() ),
    mbFixedColumn( propReader->GetIsFixedColumn() ),
    mbColumnCreator( propReader->GetIsColumnCreator() ),
    mRootColumnName( propReader->GetRootObjectName() )
{
}

FdoSmLpSimplePropertyDefinition::FdoSmLpSimplePropertyDefinition(
    FdoPropertyDefinition* pFdoProp, 
    bool bIgnoreStates,
    FdoSmLpClassDefinition* parent
) : 
	FdoSmLpPropertyDefinition(pFdoProp, bIgnoreStates, parent),
    mbNullable( true ),
    mbFixedColumn(false),
    mbColumnCreator(false)
{
}

FdoSmLpSimplePropertyDefinition::FdoSmLpSimplePropertyDefinition(
    FdoSmLpSimplePropertyP pBaseProperty, 
    FdoSmLpClassDefinition* pTargetClass, 
    FdoStringP logicalName, 
    FdoStringP physicalName, 
    bool bInherit,
    FdoPhysicalPropertyMapping* pPropOverrides
) :
	FdoSmLpPropertyDefinition(
        FDO_SAFE_ADDREF((FdoSmLpSimplePropertyDefinition*) pBaseProperty), 
        pTargetClass, 
        logicalName, 
        physicalName, 
        bInherit,
        pPropOverrides
    ),
    mbNullable( pBaseProperty->GetNullable() ),
	mColumnName( (physicalName.GetLength() > 0) ? (const wchar_t*) physicalName : pBaseProperty->GetColumnName() ),
    // At this point, we don't know if the target class maps to a foreign schema.
    // Set up a candidate root column and decide later if it should be the root.
    mCandRootColumnName( (physicalName.GetLength() > 0) ? (const wchar_t*) physicalName : pBaseProperty->GetRootColumnName() ),
    mbFixedColumn( pBaseProperty->GetIsFixedColumn() ),
    // When copying properties, not the creator when from FDO.
    mbColumnCreator( bInherit ? false : !GetIsFromFdo() )
{
    // By default, isColumnCreator is false for inherited properties. It is true
    // for copied properties since copied properties are usually object property 
    // sub-properties, which are not always in the metaschema (when not present
    // isColumnCreator is assumed to be true). 
    //
    // However, if copying from a nested property, set isColumnCreator from 
    // the nested property. When the sub-property is persisted in the metaschema,
    // it is persisted as a nested property.
    if ( (!bInherit) && FdoStringP(pBaseProperty->GetName()).Contains(L".") ) {
        mbColumnCreator = pBaseProperty->GetIsColumnCreator();
    }

}

bool FdoSmLpSimplePropertyDefinition::GetNullable() const
{
    return mbNullable;
}

const FdoSmPhColumn* FdoSmLpSimplePropertyDefinition::RefColumn() const
{
    return (FdoSmPhColumn*) ((FdoSmLpSimplePropertyDefinition*) this)->GetColumn();
}

FdoSmPhColumnP FdoSmLpSimplePropertyDefinition::GetColumn()
{
    Finalize();

    return mColumn;
}

FdoString* FdoSmLpSimplePropertyDefinition::GetColumnName() const
{
    ((FdoSmLpSimplePropertyDefinition*) this)->Finalize();

    return mColumnName;
}

FdoString* FdoSmLpSimplePropertyDefinition::Get_ColumnName()
{
    return mColumnName;
}

FdoString* FdoSmLpSimplePropertyDefinition::GetRootColumnName() const
{
    return mRootColumnName;
}

void FdoSmLpSimplePropertyDefinition::SetColumn( FdoSmPhColumnP column )
{
    mColumn = column;
    if ( column ) {
        SetColumnName( column->GetName() );
        // When column in view on foreign table, tell it what its root
        // column name is.
        // TODO: the column should really figure this out for itself but
        // a bit of investigation is required to so if this is easy to do
        // in DBI.
        if ( mRootColumnName.GetLength() > 0 ) 
            mColumn->SetRootName( mRootColumnName );
    }
}

void FdoSmLpSimplePropertyDefinition::SetColumnName( FdoString* columnName )
{
    mColumnName = columnName;
}

void FdoSmLpSimplePropertyDefinition::SetRootColumnName( FdoString* columnName )
{
    mRootColumnName = columnName;
}

void FdoSmLpSimplePropertyDefinition::SetElementState(FdoSchemaElementState elementState)
{
	// Set state for this element.
	FdoSmSchemaElement::SetElementState( elementState );
}


FdoSchemaExceptionP FdoSmLpSimplePropertyDefinition::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    ((FdoSmLpSimplePropertyDefinition*) this)->Finalize();

	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	// Column errors are gathered when the errors on the table are gathered, so no need
	// to gather column errors here.

	return pException;
}

const FdoSmLpSimplePropertyDefinition* FdoSmLpSimplePropertyDefinition::ColName2Property( 
	const FdoSmLpPropertyDefinitionCollection* pProperties,
	FdoStringP columnName 
)
{
	const FdoSmLpSimplePropertyDefinition* pMatchedProp = NULL;

	// Search the collection for the property corresponding to the column.
	for ( int i = 0; i < pProperties->GetCount(); i++ ) {
		const FdoSmLpSimplePropertyDefinition* pSimpleProp = 
			FdoSmLpSimplePropertyDefinition::Cast( pProperties->RefItem(i) );

		// Only Simple properties have a single column
		if ( pSimpleProp ) {
			if ( columnName.ICompare(pSimpleProp->GetColumnName()) == 0 ) {
				pMatchedProp = pSimpleProp;
				break;
			}
		}
	}

	return(pMatchedProp);
}

void FdoSmLpSimplePropertyDefinition::SetInherited( const FdoSmLpPropertyDefinition* pBaseProp )
{
    const FdoSmLpSimplePropertyDefinition* pSimpleBaseProp =
        FdoSmLpSimplePropertyDefinition::Cast( pBaseProp );

    // Inherited property column is overridden if base property column is overridden.
    if ( pSimpleBaseProp && !pSimpleBaseProp->GetIsSystem() ) 
        mbFixedColumn = pSimpleBaseProp->GetIsFixedColumn();

    FdoSmLpPropertyDefinition::SetInherited( pBaseProp );
}

void FdoSmLpSimplePropertyDefinition::SetNullable( bool bNullable )
{
    mbNullable = bNullable;
}

void FdoSmLpSimplePropertyDefinition::CreateColumn( FdoSmPhDbObjectP dbObject )
{
	FdoStringP columnName = GetColumnName();
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

	if ( dbObject ) {
        FdoSmPhOwnerP owner = pPhysical->GetOwner();

        FdoSmPhTableP table = dbObject->SmartCast<FdoSmPhTable>();
        bool isFixedColumn = GetIsFixedColumn();
        if ( isFixedColumn ) {
            if ( owner->GetHasMetaSchema() && ColumnIsForeign() && !GetIsSystem() ) 
                // Although column has been designated as fixed, it is a non-system 
                // foreign column so it is really not fixed (It's a column in a foreign
                // table view.
                isFixedColumn = false;
        }
        else {
            if ( (!RefLogicalPhysicalSchema()->RefSchemas()->CanCreatePhysicalObjects()) || ((!ColumnIsForeign()) && (!table)) ) 
                // The column name has been designated as non fixed but, the datastore is not FDO-enabled
                // or the column will be in a non-foreign non-table. In these cases, it is really fixed.
                // This ensures we do not adjust non-foreign view column names.
                isFixedColumn = true;
        }

		try {
    		// Column name is property name adjusted to be RDBMS-friendly.
	    	FdoStringP columnNameGen = GenColumnName( 
                dbObject, 
                columnName, 
                owner->GetHasMetaSchema(),
                isFixedColumn 
            );

            // skip column creation for  pre-existing non-foreign columns
            // with overridden names. 
            // Also skips creation for non-foreign columns when the containing database object is not a table. 
            // Just attach to the column in this case.
            // For foreign columns, a column in the referencing view is 
            // always created.

            FdoSmPhColumnP foundColumn;
            if ( (!ColumnIsForeign() || !owner->GetHasMetaSchema()) && isFixedColumn ) {
                // By default, column override is match to column
                // by case-sensitive name compare
				FdoSmPhColumnsP columns = dbObject->GetColumns();
                foundColumn = columns->FindItem(columnNameGen);
                // search with real name
                if ( !foundColumn )
                    foundColumn = columns->FindItem(columnName);
                if ( !foundColumn ) {
                    // However, if no case-sensitive match, a match to column
                    // of default case is done if such a column exists.
                    FdoStringP CiColumnName = pPhysical->GetDcColumnName(columnNameGen);
                    if ( CiColumnName != columnNameGen ) {
                        foundColumn = columns->FindItem(CiColumnName);
                        if ( foundColumn || !pPhysical->SupportsMixedCase() ) 
                            // Matched to default case so change case of
                            // property's column name.
                            columnNameGen = CiColumnName;
                    }
                }
            }

            if ( foundColumn ) 
                SetColumn( foundColumn );
            else
                SetColumnName(columnNameGen);
 
            // Create the column in the datastore if all of the following are true ...

            // datastore is fdo-enabled and column does not yet exist
            if ( RefLogicalPhysicalSchema()->RefSchemas()->CanCreatePhysicalObjects() && (!foundColumn) ) {
                // column is in a table or a foreign schema wrapping view
                if ( ColumnIsForeign() || table ) {
                    // column is not a system column in a foreign schema wrapping view.
                    // System properties not applicable to foreign schema.
                    if ( (!ColumnIsForeign()) || (!GetIsSystem()) ) {
                        // Do not add system columns to dbobject that already exists. 
                        if ( (!GetIsSystem()) || (dbObject->GetElementState() == FdoSchemaElementState_Added) ) {
        		            // Create the column 
     		                SetColumn( NewColumn(dbObject, columnNameGen) );
                            mbColumnCreator = true;
                        }
                    }
                }
            }
 		}
		catch ( FdoSchemaException* se )
		{
			// Log column type errors.
			AddGenericError( se->GetExceptionMessage() );
			FDO_SAFE_RELEASE(se);
			SetColumn( (FdoSmPhColumn*) NULL );

            SetColumnName( 
                GenColumnName( 
                    dbObject, 
                    columnName,
                    owner->GetHasMetaSchema(),
                    isFixedColumn 
                ) 
            );

			return;
		}
	}
	else {
		// This property has no table but properties inherited or copied from it
		// might have a table. Set the column name for these inherited or copied
		// properties to use.
        FdoSmPhOwnerP owner = pPhysical->GetOwner();
		SetColumn( (FdoSmPhColumn*) NULL );
        SetColumnName( 
            GenColumnName( 
                dbObject, 
                columnName, 
                owner->GetHasMetaSchema(),
                GetIsFixedColumn() && (!ColumnIsForeign() || !owner->GetHasMetaSchema()) 
            ) 
        );
	}

    // Mark this property as being changed if not already.
    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
}

FdoStringP FdoSmLpSimplePropertyDefinition::GenColumnName( 
    FdoSmPhDbObjectP dbObject, 
    FdoStringP columnName,
    bool hasMetaSchema,
    bool bFixed
)
{
    FdoStringP outColumnName = columnName.GetLength() > 0 ? (FdoString*) columnName : GetName();

    if ( hasMetaSchema ) {
        // Has MetaSchema so generate unique column name.
        outColumnName = ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
            dbObject, 
            this, 
            outColumnName, 
            bFixed 
        );
    }
    else {
        // No MetaSchema, can't adjust column name since this leads to property name changing.
        // Just report any problems with the column name.
        VldColumnName( outColumnName );
    }

    return outColumnName;
}

bool FdoSmLpSimplePropertyDefinition::ColumnIsForeign()
{
    bool isForeign = false;

    if ( mRootColumnName.GetLength() > 0 ) {
        // Root column assigned, so column must be foreign.
        isForeign = true;
    }
    else {
        // Root column not yet assigned, column is foreign if class
        // table is foreign.
        const FdoSmLpClassDefinition* pClass = 
            this->RefParentClass();

        if ( pClass ) {
            if ( wcslen(pClass->GetOwner()) > 0 ) 
                isForeign = true;
        }
    }

    return(isForeign);
}

void FdoSmLpSimplePropertyDefinition::Finalize()
{
	FdoSmLpPropertyDefinition::Finalize();
	FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();

    // If column in view on foreign table, set root column name
    // if not already. If this column was copied or inherited from another 
    // column then use the candidate root name. Otherwise default to the 
    // property name.

    if ( (mRootColumnName.GetLength() == 0) && ColumnIsForeign() ) {
        FdoStringP rootColumnName = (mCandRootColumnName.GetLength() > 0) ? mCandRootColumnName : pPhysical->GetDcColumnName(GetName());
        SetRootColumnName( rootColumnName );
/* TODO
        ValidateForeignObjectName( rootColumnName );
*/
        if ( mColumn ) 
            ((FdoSmPhColumn*) mColumn)->SetRootName( rootColumnName );
    }
}

bool FdoSmLpSimplePropertyDefinition::VldColumnName( FdoStringP columnName )
{
    bool bValid = true;
    FdoSmPhMgrP pPhysical = GetLogicalPhysicalSchema()->GetPhysicalSchema();
    FdoSize iNameMaxLen = pPhysical->ColNameMaxLen();;


    // Censor out characters not acceptable to RDBMS
	FdoStringP workName = pPhysical->CensorDbObjectName(columnName);

    // When workName is different, column name has an invalid character.
    if ( workName != columnName && !GetIsFromConfig() ) {
       AddColCharError( columnName );
       bValid = false;
    }

    if ( (int)strlen(workName) > iNameMaxLen ) {
        AddColLengthError( columnName );
        bValid = false;
    }

    if ( pPhysical->IsDbObjectNameReserved( workName) && !GetIsFromConfig() ) {
        AddColReservedError( columnName );
        bValid = false;
    }

    // If the datastore has no metaschema and the column name is overridden, the override
    // must come from a config doc. If it came from ApplySchema then the override will 
    // cause the property name to change on round-trip Apply/Describe.
    if ( !GetIsFromConfig() && !GetHasMetaSchema() && (columnName != GetName()) ) {
        AddPropNameChangeError( columnName );
        bValid = false;
    }

    return( bValid );
}

void FdoSmLpSimplePropertyDefinition::AddColCharError( 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_302),
                (FdoString*)GetQName(),
				columnName
			)
		)
	);
}

void FdoSmLpSimplePropertyDefinition::AddColLengthError( 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_303),
                (FdoString*)GetQName(),
                GetLogicalPhysicalSchema()->GetPhysicalSchema()->ColNameMaxLen(),
				columnName
			)
		)
	);
}

void FdoSmLpSimplePropertyDefinition::AddColReservedError( 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_304),
                (FdoString*)GetQName(),
				columnName
			)
		)
	);
}

void FdoSmLpSimplePropertyDefinition::AddPropNameChangeError( 
    FdoString* columnName
)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_38),
				columnName,
                (FdoString*)GetQName()
			)
		)
	);
}

