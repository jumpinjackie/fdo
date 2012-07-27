#ifndef FDOSMPHODBCCOLUMNDECIMAL_H
#define FDOSMPHODBCCOLUMNDECIMAL_H       1
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

// Represents a Odbc Number type column with fixed size and scale.
class FdoSmPhOdbcColumnDecimal :
    public FdoSmPhColumnDecimal, public FdoSmPhOdbcColumn
{
public:
    // Create a decimal columntype.
    //
    // Parameters:
    //      length: maximum number of digits, including digits to the
    //          left and right of the decimal point.
    //      scale: maximum number of digits to the right of the decimal point.
    FdoSmPhOdbcColumnDecimal(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        int length,
        int scale,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn       ( columnName, L"DECIMAL", elementState, parentObject, bNullable, rootColumnName, defaultValue),
        FdoSmPhColumnDecimal( length, scale )
    {}

    ~FdoSmPhOdbcColumnDecimal(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }

protected:
    int mScale;
};

typedef FdoPtr<FdoSmPhOdbcColumnDecimal> FdoSmPhOdbcColumnDecimalP;

#endif
