#ifndef FDOSMPHMYSQLCOLUMNCHAR_H
#define FDOSMPHMYSQLCOLUMNCHAR_H      1
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

// Represents a MySql VARCHAR2 type column.
class FdoSmPhMySqlColumnChar :
    public FdoSmPhColumnChar, public FdoSmPhMySqlColumn
{
public:
    // Create the  column type.
    //
    // Parameters:
    //      length: maximum number of characters.
    FdoSmPhMySqlColumnChar(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        int length,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhMySqlColumn (reader),
        FdoSmPhColumn    ( 
            columnName, 
            length <= mVarcharMaxLen ? L"VARCHAR" :
                ( length <= mTextMaxLen ? L"TEXT" :
                    ( length < mMediumTextMaxLen ? L"MEDIUMTEXT" : L"LONGTEXT"
                    )
                ), 
            elementState, 
            parentObject, 
            bNullable, 
            rootColumnName
        ),
        FdoSmPhColumnChar( length )
    {}

    virtual ~FdoSmPhMySqlColumnChar(void) {}

    virtual int GetRdbType()
    {
        return RDBI_STRING;
    }

    // Get SQL sub-clause for column type indicator
    virtual FdoStringP GetTypeSql();

    static const FdoInt64 mVarcharMaxLen = 255;
    static const FdoInt64 mTextMaxLen = 65535;
    static const FdoInt64 mMediumTextMaxLen = 16777215;
};

typedef FdoPtr<FdoSmPhMySqlColumnChar> FdoSmPhMySqlColumnCharP;

#endif
