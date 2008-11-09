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
