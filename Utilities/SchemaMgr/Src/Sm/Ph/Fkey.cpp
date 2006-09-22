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
#include <Sm/Ph/Fkey.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Error.h>


FdoSmPhFkey::FdoSmPhFkey(
    FdoStringP name, 
    const FdoSmPhTable* pFkeyTable,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
) : 
    FdoSmPhDbElement( name, (FdoSmPhMgr*) NULL, pFkeyTable, elementState ),
    mPkeyTableName(pkeyTableName),
    mPkeyTableOwner(pkeyTableOwner)
{
    mPkeyColumnNames = FdoStringCollection::Create();
}

FdoSmPhFkey::~FdoSmPhFkey(void)
{
}

const FdoSmPhTable* FdoSmPhFkey::RefPkeyTable() const
{
    return (FdoSmPhTable*) ((FdoSmPhFkey*) this)->GetPkeyTable();
}

FdoSmPhTableP FdoSmPhFkey::GetPkeyTable()
{
	LoadPkeyTable();

    return mPkeyTable;
}

const FdoSmPhColumnCollection* FdoSmPhFkey::RefPkeyColumns() const
{
    return (FdoSmPhColumnCollection*) ((FdoSmPhFkey*) this)->GetPkeyColumns();
}

FdoSmPhColumnsP FdoSmPhFkey::GetPkeyColumns()
{
    LoadPkeyTable();

	return mPkeyColumns;
}

const FdoSmPhColumnCollection* FdoSmPhFkey::RefFkeyColumns() const
{
    return (FdoSmPhColumnCollection*) ((FdoSmPhFkey*) this)->GetFkeyColumns();
}

FdoSmPhColumnsP FdoSmPhFkey::GetFkeyColumns()
{
    if ( !mFkeyColumns )
        mFkeyColumns = new FdoSmPhColumnCollection();

	return mFkeyColumns;
}

void FdoSmPhFkey::AddFkeyColumn( FdoSmPhColumnP column, FdoStringP pkeyColumnName )
{
    GetFkeyColumns()->Add( column );
    mPkeyColumnNames->Add( pkeyColumnName );
}

FdoStringP FdoSmPhFkey::GetAddSql()
{
    FdoInt32            i;
    FdoSmPhColumnsP     fkeyColumns = GetFkeyColumns();
    FdoSmPhColumnsP     pkeyColumns = GetPkeyColumns();
    FdoSmPhTableP       pkeyTable   = GetPkeyTable();
    FdoStringsP         fkColNames  = FdoStringCollection::Create();
    FdoStringsP         pkColNames  = FdoStringCollection::Create();
    FdoStringP          fkeySql;

    if ( fkeyColumns->GetCount() > 0 ) {
	    for ( i = 0; i < fkeyColumns->GetCount(); i++ )
	        fkColNames->Add( fkeyColumns->RefItem(i)->GetDbName() );
    
        for ( i = 0; i < pkeyColumns->GetCount(); i++ )
	        pkColNames->Add( pkeyColumns->RefItem(i)->GetDbName() );

        fkeySql = FdoStringP::Format( 
            L"constraint %ls foreign key ( %ls ) references %ls ( %ls ) ",
            (FdoString*) GetDbName(),
            (FdoString*) fkColNames->ToString(),
            (FdoString*) pkeyTable->GetDDLQName(),
            (FdoString*) pkColNames->ToString()
        );
    }

    return fkeySql;
}

void FdoSmPhFkey::LoadPkeyTable()
{
    FdoInt32 i;

	if ( !mPkeyTable ) {
        // Load primary columns as well
        mPkeyColumns = new FdoSmPhColumnCollection();

        const FdoSmPhTable*  pFkTable = dynamic_cast<const FdoSmPhTable*>(GetParent());

        // Find the referenced primary table,
        // it is always in the same database as the foreign key
        FdoStringP database = pFkTable->GetParent()->GetParent()->GetName();
        FdoSmPhDbObjectP dbObject = GetManager()->FindDbObject( mPkeyTableName, mPkeyTableOwner, database );
        mPkeyTable = dbObject.p->SmartCast<FdoSmPhTable>();

        if ( mPkeyTable ) {
            // Find each primary key column.
            for ( i = 0; i < mPkeyColumnNames->GetCount(); i++ ) {
                FdoStringP columnName = mPkeyColumnNames->GetString(i);
                FdoSmPhColumnP column = FdoSmPhColumnsP(mPkeyTable->GetColumns())->FindItem(columnName);

                if ( column ) {
                    mPkeyColumns->Add( column );
                }
                else {
                    // Column must exist in primary table.
                    AddPkeyColumnError( columnName );
                    mPkeyColumns->Clear();
                    break;
                }
            }
        }
    }
}

void FdoSmPhFkey::AddPkeyColumnError(FdoStringP columnName)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
                FDO_NLSID(FDOSM_5), 
				(FdoString*) columnName, 
				(FdoString*) GetQName(),
                (FdoString*) mPkeyTable->GetQName()
            )
        )
	);
}

