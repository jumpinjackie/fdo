#ifndef FDOSMPHSQSCOLUMNCHAR_H
#define FDOSMPHSQSCOLUMNCHAR_H      1
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
#include <Sm/Ph/ColumnChar.h>
#include "Column.h"

// Represents a SqlServer VARCHAR2 type column.
class FdoSmPhSqsColumnChar :
    public FdoSmPhColumnChar, public FdoSmPhSqsColumn
{
public:
    // Create the  column type.
    //
    // Parameters:
    //      length: maximum number of characters.
    FdoSmPhSqsColumnChar(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        int length,
        FdoStringP rootColumnName = L"",
		FdoPtr<FdoDataValue> defaultValue = (FdoDataValue*) NULL,
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhSqsColumn (reader),
        FdoSmPhColumn    ( 
            columnName, 
            // Unicode varchar is the preferred column type, but fall back 
            // to varchar or text if length too long.
            ((length > mVarcharMaxLen) ? 
                L"TEXT" : 
                ((length > mNVarcharMaxLen) ? L"VARCHAR" : L"NVARCHAR")), 
            elementState, 
            parentObject, 
            bNullable, 
            rootColumnName,
			defaultValue,
            reader
        ),
        FdoSmPhColumnChar(
            // When using the varchar type, make column as wide as possible
            // in case the database uses a double-byte character set.
            ((length > mNVarcharMaxLen) && (length < mVarcharMaxLen)) ? 
                mVarcharMaxLen : 
                length 
        )
    {}

    virtual ~FdoSmPhSqsColumnChar(void) {}

    virtual int GetRdbType()
    {
        return RDBI_STRING;
    }

    // Get SQL sub-clause for column type indicator
    virtual FdoStringP GetTypeSql();

    static const FdoInt64 mNVarcharMaxLen = 4000;
    static const FdoInt64 mVarcharMaxLen = 8000;
};

typedef FdoPtr<FdoSmPhSqsColumnChar> FdoSmPhSqsColumnCharP;

#endif
