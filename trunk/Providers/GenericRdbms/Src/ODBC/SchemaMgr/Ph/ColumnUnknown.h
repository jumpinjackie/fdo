#ifndef FDOSMPHODBCCOLUMNUNKNOWN_H
#define FDOSMPHODBCCOLUMNUNKNOWN_H       1
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
#include <Sm/Ph/ColumnUnknown.h>

// This class represents a type of column not known to the ODBC
// provider. Therefore it is handled in a generic manner, meaning
// that the caller must provide the native RDBMS type for this column.
//
// The caller may also provide the length and scale if relevant. These are
// used to format the SQL clause for adding the column if new. Therefore, this
// column type allows callers to created columns of types not explicitly
// handled by the ODBC provider.
class FdoSmPhOdbcColumnUnknown :
    public FdoSmPhColumnUnknown, public FdoSmPhOdbcColumn
{
public:
    // Create an unknown columntype.
    //
    // Parameters:
    //      length: maximum number of digits, including digits to the
    //          left and right of the decimal point.
    //      scale: maximum number of digits to the right of the decimal point.
    FdoSmPhOdbcColumnUnknown(
        FdoStringP columnName,
        FdoStringP typeName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn       ( columnName, typeName, elementState, parentObject, bNullable, rootColumnName),
        FdoSmPhColumnUnknown( length, scale )
    {}

    ~FdoSmPhOdbcColumnUnknown(void) {}

    virtual int GetRdbType()
    {
        // Best guess at appropriate type. Most types can be represented as
        // strings.
        return RDBI_STRING;
    }

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhOdbcColumnUnknown() {}
};

typedef FdoPtr<FdoSmPhOdbcColumnUnknown> FdoSmPhOdbcColumnUnknownP;

#endif
