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
#include "Rd/ConstraintReader.h"
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
	FdoStringP	ckey = GetManager()->FormatSQLVal(val);

    return ckey;
}

FdoStringP FdoSmPhSqsTable::FixCkeyClause( FdoStringP ckey)
{
    // This function removes the N (unicode) specifiers from strings, so that the 
    // constraint parser can handle them.
    FdoSize len = ckey.GetLength();
    wchar_t*  buffer = new wchar_t[len + 1];
    wcscpy( buffer, ckey );
    FdoInt32 state = 0;
    
    for ( FdoSize i = 0; i < len; i++ )
    {
        switch (state) 
        {
        case 0:    // outside single quote delimiters
            if ( buffer[i] == 'N' )
                // encountered Unicode specifier
                state = 1;
            else if ( buffer[i] == '\'' ) 
                // encountered opening delimiter
                state = 2;
            break;
        
        case 1:     // N (Unicode specifier) encountered
            if ( buffer[i] == '\'' ) 
            {
                // N followed by single quote so it is a unicode specifier.
                // Blank it out.
                buffer[i-1] = ' ';
                // we're now inside delimited part.
                state = 2;
            }
            else 
            {
                // N not followed by single quote; need to keep it.
                // Still outside delimited part.
                state = 0;
            }
            break;

        case 2:         // inside quote delimited part
            // This state ensures that no N that is inside the delimited part is 
            // deleted. E.G: ensures that "N'OPEN'" becomes "'OPEN'" instead of "'OPE'"
            if ( buffer[i] == '\'' ) 
                // encountered next single quote. Now outside delimited part.
                state = 0;
            break;

        default:
            state = 0;
            break;
        }
    }

	FdoStringP	clause = buffer;
    delete[] buffer;

    return clause;
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
