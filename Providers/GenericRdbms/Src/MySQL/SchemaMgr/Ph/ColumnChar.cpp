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

#include "stdafx.h"
#include "ColumnChar.h"
#include "CharacterSet.h"
#include "Mgr.h"
#include "DbObject.h"

FdoStringP FdoSmPhMySqlColumnChar::GetTypeSql()
{
    if ( GetTypeName().ICompare(L"varchar") == 0 ) 
        // Representing by varchar, need to include length specification
        return FdoSmPhColumnChar::GetTypeSql();
    else
        // Too big for varchar so creating text type column. Don't 
        // include length in sql creation syntax.
        return FdoSmPhColumnChar::GetTypeName();
}

FdoInt64 FdoSmPhMySqlColumnChar::GetDbBinarySize()
{
    FdoInt64 charCount = GetLength();
    FdoInt64 charSize = 3; // Max character size in bytes.
                           // Assume largest size by default. 

    // Max character size is determine by column's character set. 
    FdoSmPhMySqlCharacterSetP characterSet = GetCharacterSet().p->SmartCast<FdoSmPhMySqlCharacterSet>();
    if ( characterSet ) 
        charSize = characterSet->GetCharLen();

    // Size is max characters X max character size. 
    return charCount * charSize;
}

FdoStringP FdoSmPhMySqlColumnChar::CalcTypeName( FdoSmPhRdColumnReader* reader, int length, FdoSmPhDbObject* parentObject, FdoPtr<FdoDataValue> defaultValue )
{
    FdoStringP typeName;

    if ( reader )
    {
        // Column already exist so just get RDB type name from reader.
        typeName = reader->GetString(L"", L"type_string").Upper();
    }
    else
    {
        // new column, determine RDB type from length.

        FdoSmPhMySqlMgrP mgr = parentObject->GetManager()->SmartCast<FdoSmPhMySqlMgr>();

        // For versions before MySQL 5.0.3 maximum varchar length is 255. 
        // For MySQL 5.0.3 and after, maximum varchar length is 65335. However, this is also the 
        // maximum for all non-LOB columns in this column's table.
        // At this point, we don't know how many other columns will be in the containing table so, 
        // to be on the safe side, varchar is only used for columns with length of 255 or less.
        // However, text columns cannot have default values. Columns of length 256 would be fairly 
        // common so, if the MySQL version is 5.0.3 or after, and the column has a default value,
        // set the maximum for varchar a bit higher (512).
        //
        // TODO: Move this calculation to just before the containing table is created or modified.
        // Once we know all of the columns in the table, and their sizes, the varchar maximum size
        // could potentially be set much higher.
        FdoInt32 maxLen = 255;

        if ( defaultValue ) 
        {
            maxLen = mgr->GetVarcharMaxLen();
            if ( maxLen > 512 )
                maxLen = 512;
        }

        typeName = 
            length <= maxLen ? L"VARCHAR" :
                ( length <= mTextMaxLen ? L"TEXT" :
                    ( length <= mMediumTextMaxLen ? L"MEDIUMTEXT" : L"LONGTEXT"
                    )
                );
    }

    return typeName;
}

