#ifndef FDOSMPHSQSCOLUMNDECIMAL_H
#define FDOSMPHSQSCOLUMNDECIMAL_H       1
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

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnDecimal.h>
#include "Column.h"

// Represents a SqlServer Number type column with fixed size and scale.
class FdoSmPhSqsColumnDecimal :
    public FdoSmPhColumnDecimal, public FdoSmPhSqsColumn
{
public:
    // Create a decimal columntype.
    //
    // Parameters:
    //      length: maximum number of digits, including digits to the
    //          left and right of the decimal point.
    //      scale: maximum number of digits to the right of the decimal point.
    FdoSmPhSqsColumnDecimal(
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
        FdoSmPhSqsColumn (reader),
        FdoSmPhColumn       ( columnName, L"DECIMAL", elementState, parentObject, bNullable, rootColumnName, defaultValue),
        FdoSmPhColumnDecimal( length, scale )
    {}

    ~FdoSmPhSqsColumnDecimal(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }

    virtual bool HasMaxLength() const;

    virtual int GetMaxLength() const;

    virtual bool HasMaxScale() const;

    virtual int GetMaxScale() const;
};

typedef FdoPtr<FdoSmPhSqsColumnDecimal> FdoSmPhSqsColumnDecimalP;

#endif
