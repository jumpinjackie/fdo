#ifndef FDOSMPHODBCCOLUMNDOUBLE_H
#define FDOSMPHODBCCOLUMNDOUBLE_H        1
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

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnDecimal.h>

// Represents a Odbc floating point type column.
class FdoSmPhOdbcColumnDouble :
    public FdoSmPhColumnDouble, public FdoSmPhOdbcColumn
{
public:
    FdoSmPhOdbcColumnDouble(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        FdoStringP rootColumnName = L"",
		FdoStringP defaultValue = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn      ( columnName, L"FLOAT", elementState, parentObject, bNullable, rootColumnName, defaultValue)
    {}
/*
FdoStringP FdoSmPhColumn::ToSql() const
{
    FdoStringP colSql = FdoStringP::Format(
        L"%ls %ls %ls",
        GetName(),
        (FdoString*) ((FdoSmPhColumn*) this)->GetColumn()->ToSql(),
        mbNullable ? L"" : L"not null"
    );

    return colSql;
}
*/
    virtual ~FdoSmPhOdbcColumnDouble(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }
};

typedef FdoPtr<FdoSmPhOdbcColumnDouble> FdoSmPhOdbcColumnDoubleP;

#endif
