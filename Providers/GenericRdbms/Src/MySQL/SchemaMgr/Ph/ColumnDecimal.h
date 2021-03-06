#ifndef FDOSMPHMYSQLCOLUMNDECIMAL_H
#define FDOSMPHMYSQLCOLUMNDECIMAL_H       1
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
#include "Column.h"

// Represents a MySql Number type column with fixed size and scale.
class FdoSmPhMySqlColumnDecimal :
    public FdoSmPhColumnDecimal, public FdoSmPhMySqlColumn
{
public:
    // Create a decimal columntype.
    //
    // Parameters:
    //      length: maximum number of digits, including digits to the
    //          left and right of the decimal point.
    //      scale: maximum number of digits to the right of the decimal point.
    FdoSmPhMySqlColumnDecimal(
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
        FdoSmPhMySqlColumn (reader),
        FdoSmPhColumn       ( columnName, L"DECIMAL", elementState, parentObject, bNullable, rootColumnName, defaultValue),
        FdoSmPhColumnDecimal( length, scale )
    {}

    ~FdoSmPhMySqlColumnDecimal(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }

    virtual bool HasMaxLength() const;

    virtual int GetMaxLength() const;

    virtual bool HasMaxScale() const;

    virtual int GetMaxScale() const;

    // Calculates size in Datastore based on total number of digits
    virtual FdoInt64 GetDbBinarySize();

protected:
    // Calculates size in Datastore based on given number of digits
    FdoInt64 Digits2DbBinarySize( FdoInt32 digits );

    int mScale;
};

typedef FdoPtr<FdoSmPhMySqlColumnDecimal> FdoSmPhMySqlColumnDecimalP;

#endif
