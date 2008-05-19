/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "Table.h"
#include "Owner.h"
#include "Mgr.h"
#include "SpatialIndex.h"
#include "Rd/ConstraintReader.h"
#include "Rd/IndexReader.h"
#include "Rd/DbObjectReader.h"


FdoSmPhSqsTable::FdoSmPhSqsTable(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoStringP pkeyName,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdTable(name, pOwner, elementState, pkeyName),
    FdoSmPhSqsDbObject(name, pOwner, reader),
    FdoSmPhDbObject(name, pOwner, elementState)
{
}

FdoSmPhSqsTable::~FdoSmPhSqsTable(void)
{
}

bool FdoSmPhSqsTable::SupportsAddNotNullColumn() const
{
    return false;
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhSqsTable::CreateConstraintReader( FdoString* type ) const
{
	FdoSmPhSqsOwner* pSqsOwner = static_cast<FdoSmPhSqsOwner*>((FdoSmPhDbElement*) GetParent());

    return new FdoSmPhRdSqsConstraintReader( pSqsOwner, GetName(), type );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhSqsTable::CreateIndexReader() const
{
    FdoSmPhSqsTable* pTable = (FdoSmPhSqsTable*) this;

    return new FdoSmPhRdSqsIndexReader( pTable->GetManager(), FDO_SAFE_ADDREF(pTable) );
}

bool FdoSmPhSqsTable::Add()
{
    FdoInt32 idx;
    bool hasBlob = false;

    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    // Create table:
    FdoStringP sqlStmt = FdoStringP::Format(
        L"%ls %ls",
        (FdoString*) GetAddSql(),
        (FdoString*) GetAddStorageSql()
    );
    gdbiConn->ExecuteNonQuery( (FdoString*) sqlStmt );

    // Check if table has blob column. If it does not 
    // then the subsequent textInRow setting command will 
    // generate an error. In this case, skip the textInRow
    // setting since it is not applicable anyway to this table.
    FdoSmPhColumnsP columns = GetColumns();
    for ( idx = 0; idx < columns->GetCount(); idx++ )
    {
        FdoSmPhColumnP column = columns->GetItem(idx);
        FdoStringP typeName = column->GetTypeName();
        if ( (typeName.ICompare(L"text") == 0) ||
             (typeName.ICompare(L"ntext") == 0) ||
             (typeName.ICompare(L"image") == 0)
        ) {
            hasBlob = true;
            break;
        }

    }

    // Run additional text-in-row option on table, if necessary:
    if (hasBlob && (GetTextInRowOption() == SqlServerOvTextInRowOption_InRow))
    {
        sqlStmt = FdoStringP::Format(
            L"sp_tableoption %ls, 'text in row', 'ON'", 
            (FdoString*) GetManager()->FormatSQLVal(GetName(),FdoSmPhColType_String)
        );
        gdbiConn->ExecuteNonQuery( (FdoString*) sqlStmt );
    }

    return true;
}

bool FdoSmPhSqsTable::Modify()
{
    // Nothing to modify yet.
    return true;
}

bool FdoSmPhSqsTable::Delete()
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = GetDeleteSql();

    gdbiConn->ExecuteNonQuery( (FdoString*) sqlStmt );

    return true;
}

FdoSmPhIndexP FdoSmPhSqsTable::NewIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhSqsIndex( name, this, isUnique, elementState );
}

FdoSmPhIndexP FdoSmPhSqsTable::NewSpatialIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhSqsSpatialIndex( name, this, elementState );
}

FdoSmPhFkeyP FdoSmPhSqsTable::NewFkey(
    FdoStringP name,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhSqsFkey( name, this, pkeyTableName, pkeyTableOwner, elementState );
}

FdoStringP FdoSmPhSqsTable::GetAddHdgSql()
{
    return FdoStringP::Format( 
        L"create table %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringP FdoSmPhSqsTable::GetAddStorageSql()
{
    FdoStringP storageSql;

    if (mTableFilegroup.GetLength() > 0)
        storageSql += FdoStringP::Format( 
            L" ON \"%ls\"", 
            (FdoString*)mTableFilegroup 
        );

    if (mTextFilegroup.GetLength() > 0)
        storageSql += FdoStringP::Format( 
            L" TEXTIMAGE_ON \"%ls\"", 
            (FdoString*)mTextFilegroup
        );

    return storageSql;
}


FdoStringP FdoSmPhSqsTable::GetAddSql()
{
    return FdoSmPhTable::GetAddSql();
}

FdoStringP FdoSmPhSqsTable::ConvertCkey( FdoDataValue *val )
{
	FdoStringP	ckey;
	FdoDataType	type = val->GetDataType();

	if ( type == FdoDataType_DateTime ) 
	{		
		FdoStringP	date = val->ToString();  // Returns "TIMESTAMP 'YYY-MM-DD HH24:MI:SS'"
		FdoStringP	date1 = date.Replace(L"TIMESTAMP", L"");

		ckey = FdoStringP::Format(L"%ls", (FdoString *)date1);
	}
	else
		ckey = ( val->ToString() );	

	return ckey;
}

FdoStringP FdoSmPhSqsTable::GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp )
{
	FdoStringP	ckey = L"";

	FdoPtr<FdoPropertyValueConstraint>  valConstr = fdoProp->GetValueConstraint();

	if ( valConstr && (valConstr->GetConstraintType() == FdoPropertyValueConstraintType_Range) ) 
	{
		FdoPropertyValueConstraintRange *c = static_cast<FdoPropertyValueConstraintRange*>( valConstr.p );
		
		if ( c->GetMinValue() )
		{
    		ckey += FdoStringP(L"\"") + columnName + L"\"";
			ckey += c->GetMinInclusive() ? L" >= " : L" > ";
			FdoPtr<FdoDataValue>  val = c->GetMinValue();
	
			ckey += ConvertCkey( val );
			ckey += L" ";
		}

		if ( c->GetMaxValue() )
		{
			if ( ckey != L"" )
				ckey += L" and ";
    		ckey += FdoStringP(L"\"") + columnName + L"\"";
			ckey += c->GetMaxInclusive() ? L" <= " : L" < ";
			FdoPtr<FdoDataValue>  val = c->GetMaxValue();

			ckey += ConvertCkey( val );
			ckey += L" ";
		}
	} 
	else if ( valConstr && (valConstr->GetConstraintType() == FdoPropertyValueConstraintType_List) ) 
	{	
		FdoPropertyValueConstraintList *c = static_cast<FdoPropertyValueConstraintList*>( valConstr.p );
		FdoPtr<FdoDataValueCollection>  lst = c->GetConstraintList();
		
		if ( lst->GetCount() == 0 )
			return L"";

   		ckey += FdoStringP(L"\"") + columnName + L"\"";
		ckey += L" in (";

		for ( int i = 0; i < lst->GetCount(); i++ ) 
		{
			FdoPtr<FdoDataValue>  val = lst->GetItem(i);
			FdoDataType	type = val->GetDataType();

			// This shouldn't happen - check needed when creating the list
			if ( type == FdoDataType_BLOB || type == FdoDataType_CLOB )
				continue; 
	
			ckey += ConvertCkey( val );

			if ( i != (lst->GetCount() - 1))
				ckey += L", ";
		}
		ckey += L")";
	}
	else
	{
		// Cannot happen since one cannot set the constraint type;  
	}

	return ckey;
}
