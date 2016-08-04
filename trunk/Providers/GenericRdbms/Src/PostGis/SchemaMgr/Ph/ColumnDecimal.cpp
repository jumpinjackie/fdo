//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ColumnDecimal.h"

FdoSmPhPostGisColumnDecimal::FdoSmPhPostGisColumnDecimal(
    FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    int length,
    int scale,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
      FdoSmPhColumn(columnName, L"NUMERIC", state, parentObject, isNullable, rootColumnName, defaultValue),
      FdoSmPhColumnDecimal(length, scale)
{
    // idle
}

FdoSmPhPostGisColumnDecimal::~FdoSmPhPostGisColumnDecimal()
{
    // idle
}

int FdoSmPhPostGisColumnDecimal::GetRdbType()
{
    return RDBI_DOUBLE;
}

bool FdoSmPhPostGisColumnDecimal::HasMaxLength() const
{
    return true;
}

int FdoSmPhPostGisColumnDecimal::GetMaxLength() const
{
    return eMaxPrecision;
}

bool FdoSmPhPostGisColumnDecimal::HasMaxScale() const
{
    // TODO: mloskot - AFAIK there is no scale limit.

    return true;
}

int FdoSmPhPostGisColumnDecimal::GetMaxScale() const
{
    // TODO: mloskot - Check this value!

    return eMaxScale;
}

FdoInt64 FdoSmPhPostGisColumnDecimal::GetDbBinarySize()
{
    // size is space taken by left side digits plus space for
    // right side digits.

    FdoInt64 size = (Digits2DbBinarySize(GetLength() - GetScale())
                     + Digits2DbBinarySize(GetScale()));

    return size;    
}

FdoInt64 FdoSmPhPostGisColumnDecimal::Digits2DbBinarySize(FdoInt32 digits)
{
    // Count number of groups of nine digits chunks
    FdoInt32 novets = static_cast<FdoInt32>(digits / 9);

    // Get the remainder if # digits not divisible by 9
    FdoInt32 remainder = digits - (novets * 9);

    // Determine number of bytes for storing remainder digits
    FdoInt32 remainderBytes = static_cast<FdoInt32>((remainder + 1) / 2);

    // Total is 4 bytes per group of nine digits plus bytes for remainder.
    FdoInt64 total = (novets * 4 ) + remainderBytes;

    return total;
}

