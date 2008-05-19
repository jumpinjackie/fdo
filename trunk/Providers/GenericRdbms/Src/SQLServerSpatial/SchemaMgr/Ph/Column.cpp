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
#include "Column.h"
#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Rd/QueryReader.h"


FdoSmPhSqsColumn::FdoSmPhSqsColumn(FdoSmPhRdColumnReader* reader)
{
    if (NULL != reader)
    {
        if (reader->GetInteger(L"", L"has_computed_expression"))
        {
            // Need to fetch the computed expression from the database:
            FdoSmPhRowsP rows = new FdoSmPhRowCollection();
            FdoSmPhRowP row = new FdoSmPhRow( this->GetManager(), L"ColumnAdditionalAttributes", NULL );  // single row, no joins
            rows->Add(row);
            FdoSmPhFieldP field = new FdoSmPhField(
                row, 
                L"Text",
                row->CreateColumnChar(L"Text", true, 255)
            );
            const FdoSmSchemaElement* parent = this->GetParent();

            FdoStringP sql = FdoStringP::Format(L"SELECT text FROM syscomments "
                L" WHERE id = (SELECT id FROM sysobjects WHERE name = %ls AND xtype='U') "
                L" and number = (SELECT colid FROM syscolumns WHERE id = (SELECT id FROM sysobjects WHERE name = %ls AND xtype='U') AND name = %ls) "
                L" ORDER BY colid",
                (FdoString*) this->GetManager()->FormatSQLVal(parent->GetName(),FdoSmPhColType_String),
                (FdoString*) this->GetManager()->FormatSQLVal(parent->GetName(),FdoSmPhColType_String), 
                (FdoString*) this->GetManager()->FormatSQLVal(this->GetName(),FdoSmPhColType_String)
            );

            FdoSmPhReaderP computedExprReader = new FdoSmPhRdGrdQueryReader (row, sql, this->GetManager());
            while (computedExprReader->ReadNext())
            {
                m_computedExpression += computedExprReader->GetString(L"", L"Text");
            }
        }
    }
}


FdoSmPhSqsColumn::~FdoSmPhSqsColumn(void)
{
}


FdoStringP FdoSmPhSqsColumn::GetAddSql()
{
    FdoStringP addSql;

    // Get basic name/type/nullability SQL, or computed expression SQL:
    if (m_computedExpression.GetLength() == 0)
        addSql = FdoSmPhColumn::GetAddSql();
    else
        addSql = FdoStringP::Format(
            L"%ls as %ls %ls", 
            (FdoString*) GetDbName(), 
            (FdoString*)m_computedExpression,
    		(FdoString*) GetAutoincrementSql()
        );

    return addSql;
}


FdoStringP FdoSmPhSqsColumn::GetAutoincrementSql()
{
    // Add identity column information:
    const FdoSmPhSqsDbObject* parent = dynamic_cast<const FdoSmPhSqsDbObject*>(this->GetParent());
    FdoStringP idColumn = parent->GetIdentityColumn();

    if (0==wcscmp((FdoString*)idColumn, this->GetName()))
    {
        if (parent->GetIdentityIsGloballyUnique())
            return FdoStringP(L" ROWGUIDCOL ");
        else
            return FdoStringP::Format(L" IDENTITY (%d,%d) ", parent->GetIdentitySeed(), parent->GetIdentityIncrement());
    }
    else 
    {
        if ( FdoSmPhColumn::GetAutoincrement() )
            return L" IDENTITY (1,1) ";
        else
            return L"";
    }
}


