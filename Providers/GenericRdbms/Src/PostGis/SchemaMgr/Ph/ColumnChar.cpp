//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ColumnChar.h"

FdoSmPhPostGisColumnChar::FdoSmPhPostGisColumnChar(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    int length,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
        FdoSmPhColumn(columnName,
            (length <= mVarcharMaxLen ? L"VARCHAR" : L"TEXT"),
            state, 
            parentObject, 
            isNullable, 
            rootColumnName,
            defaultValue),
        FdoSmPhColumnChar(length)
{
    // idle
}

FdoSmPhPostGisColumnChar::~FdoSmPhPostGisColumnChar()
{
    // idle
}

int FdoSmPhPostGisColumnChar::GetRdbType()
{
    return RDBI_STRING;
}

FdoStringP FdoSmPhPostGisColumnChar::GetTypeSql()
{
    // NOTE: The ColumnChar does not support following PostgreSQL extension:
    // If character varying is used without length specifier,
    // the type accepts strings of any size.

    if (0 == GetTypeName().ICompare(L"varchar")) 
    {
        // For variable-length character type,
        // length specification is needed in SQL.
        return FdoSmPhColumnChar::GetTypeSql();
    }
    else
    {
        // Variable unlimited length text type does not
        // need lenght specifier.
        return FdoSmPhColumnChar::GetTypeName();
    }
}

