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
		try {
            FdoSmPhOwnerP owner = pPhysical->GetOwner();

    		// Column name is property name adjusted to be RDBMS-friendly.
	    	columnName = ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
                dbObject, 
                this, 
                columnName.GetLength() > 0 ? (FdoString*) columnName : GetName(), 
                GetIsFixedColumn() && (!ColumnIsForeign() || !owner->GetHasMetaSchema()) 
            );

            // skip column creation for  pre-existing non-foreign columns
            // with overridden names. 
            // Just attach to the column in this case.
            // For foreign columns, a column in the referencing view is 
            // always created.

            FdoSmPhColumnP foundColumn;
            if ( (!ColumnIsForeign() || !owner->GetHasMetaSchema()) && GetIsFixedColumn() ) {
                // By default, column override is match to column
                // by case-sensitive name compare
                foundColumn = dbObject->GetColumns()->FindItem(columnName);
                if ( !foundColumn ) {
                    // However, if no case-sensitive match, a match to column
                    // of default case is done if such a column exists.
                    FdoStringP CiColumnName = pPhysical->GetDcColumnName(columnName);
                    if ( CiColumnName != columnName ) {
                        columnName = CiColumnName;
                        foundColumn = dbObject->GetColumns()->FindItem(columnName);
                        // Matched to default case so change case of
                        // property's column name.
                        SetColumnName(columnName);
                    }
                }
                SetColumn( foundColumn );
            }


            if ( owner->GetHasMetaSchema() && ((ColumnIsForeign()) || (!GetIsFixedColumn()) || (!foundColumn)) ) {
    		    // Create the column 
                if ( (!ColumnIsForeign()) || (!GetIsSystem()) ) {
        		    SetColumn( NewColumn(dbObject, columnName) );
                    mbColumnCreator = true;
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
                ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
                    dbObject, 
                    this, 
                    columnName.GetLength() > 0 ? (FdoString*) columnName : GetName(), 
                    GetIsFixedColumn() && (!ColumnIsForeign()) 
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
            ((FdoSmLpClassDefinition*)RefParentClass())->UniqueColumnName( 
                dbObject, 
                this, 
                columnName.GetLength() > 0 ? (FdoString*) columnName : GetName(), 
                GetIsFixedColumn() && (!ColumnIsForeign() || !owner->GetHasMetaSchema()) 
            ) 
        );
	}

    // Mark this property as being changed if not already.
    if ( GetElementState() == FdoSchemaElementState_Unchanged ) 
        SetElementState( FdoSchemaElementState_Modified );
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
