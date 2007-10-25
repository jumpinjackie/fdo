#ifndef FDOSMPHODBCCOLUMNCHAR_H
#define FDOSMPHODBCCOLUMNCHAR_H      1
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
#include <Sm/Ph/ColumnChar.h>
#include "Column.h"

// Represents a Odbc VARCHAR2 type column.
class FdoSmPhOdbcColumnChar :
    public FdoSmPhColumnChar, public FdoSmPhOdbcColumn
{
public:
    // Create the  column type.
    //
    // Parameters:
    //      length: maximum number of characters.
    FdoSmPhOdbcColumnChar(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        int length,
        FdoStringP rootColumnName = L"",
		FdoStringP defaultValue = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn    ( columnName, L"VARCHAR", elementState, parentObject, bNullable, rootColumnName, defaultValue),
        FdoSmPhColumnChar( length )
    {}

    virtual ~FdoSmPhOdbcColumnChar(void) {}

    virtual int GetRdbType()
    {
        return RDBI_STRING;
    }

};

typedef FdoPtr<FdoSmPhOdbcColumnChar> FdoSmPhOdbcColumnCharP;

#endif
