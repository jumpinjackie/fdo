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
#include "ColumnDecimal.h"

bool FdoSmPhMySqlColumnDecimal::HasMaxLength() const
{
    return true;
}

int FdoSmPhMySqlColumnDecimal::GetMaxLength() const
{
    return 65;
}

bool FdoSmPhMySqlColumnDecimal::HasMaxScale() const
{
    return true;
}

int FdoSmPhMySqlColumnDecimal::GetMaxScale() const
{
    return 30;
}

FdoInt64 FdoSmPhMySqlColumnDecimal::GetDbBinarySize()
{
    // size is space taken by left side digits plus space for
    // right side digits.
    return Digits2DbBinarySize(GetLength() - GetScale()) + 
           Digits2DbBinarySize(GetScale());
}

FdoInt64 FdoSmPhMySqlColumnDecimal::Digits2DbBinarySize( FdoInt32 digits)
{
    // Find out how many groups of nine digits there are ...
    FdoInt32 novets = (FdoInt32) (digits / 9);
    // and the remainder if # digits not divisible by 9
    FdoInt32 remainder = digits - (novets * 9);

    // Determine number of bytes for storing remainder digits
    FdoInt32 remainderBytes = (FdoInt32)((remainder + 1)/2);

    // Total is 4 bytes per group of nine digits plus bytes for remainder.
    return (novets * 4 ) + remainderBytes;
}

