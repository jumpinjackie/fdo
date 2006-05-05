#ifndef FDOSMPHODBCCOLUMNSINGLE_H
#define FDOSMPHODBCCOLUMNSINGLE_H        1
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
#include <Sm/Ph/ColumnSingle.h>

// Represents a Odbc single precision floating point type column.
class FdoSmPhOdbcColumnSingle :
    public FdoSmPhColumnSingle, public FdoSmPhOdbcColumn
{
public:
    FdoSmPhOdbcColumnSingle(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn      ( columnName, L"REAL", elementState, parentObject, bNullable, rootColumnName)
    {}

    virtual ~FdoSmPhOdbcColumnSingle(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }
};

typedef FdoPtr<FdoSmPhOdbcColumnSingle> FdoSmPhOdbcColumnSingleP;

#endif
