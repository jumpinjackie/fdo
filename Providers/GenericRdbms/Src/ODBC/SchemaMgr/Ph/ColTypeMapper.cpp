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
#include "ColTypeMapper.h"

// A single entry for translating a column type between
// enum facet and Odbc string format.
struct FdoSmPhOdbcColTypeMapEntry
{
    FdoSmPhColType  m_colType;
    FdoStringP      m_colTypeString;
    FdoInt32        m_size;             // -1 == don't care
    FdoInt32        m_scale;            // -1 == don't care
};

// In the map below, any mappings that are not 1:1 should be arranged
// with the desired default one first.
static FdoSmPhOdbcColTypeMapEntry typeMap_S[] =
{
    { FdoSmPhColType_String,    L"varchar",      -1, -1 },
    { FdoSmPhColType_String,    L"char",         -1, -1 },
    { FdoSmPhColType_String,    L"longvarchar",  -1, -1 },
    { FdoSmPhColType_String,    L"wchar",        -1, -1 },
    { FdoSmPhColType_String,    L"wvarchar",     -1, -1 },
    { FdoSmPhColType_String,    L"wlongvarchar", -1, -1 },
    { FdoSmPhColType_Byte,      L"tinyint",      -1, -1 },
    { FdoSmPhColType_Int16,     L"smallint",     -1, -1 },
    { FdoSmPhColType_Int32,     L"integer",      -1, -1 },
    { FdoSmPhColType_Int64,     L"bigint",       -1, -1 },
    { FdoSmPhColType_Decimal,   L"decimal",      -1, -1 },
    { FdoSmPhColType_Decimal,   L"numeric",      -1, -1 },
    { FdoSmPhColType_Single,    L"real",         -1, -1 },
    { FdoSmPhColType_Single,    L"float",        -1, -1 },
    { FdoSmPhColType_Double,    L"double",       -1, -1 },
    { FdoSmPhColType_String,    L"unknown",       0,  0 },
    { FdoSmPhColType_Date,      L"timestamp",    -1, -1 },
    { FdoSmPhColType_Date,      L"date",         -1, -1 },
    { FdoSmPhColType_Date,      L"time",         -1, -1 },
    { FdoSmPhColType_Bool,      L"bit",          -1, -1 }
};
static FdoInt32 typeMapSize_S = sizeof(typeMap_S) / sizeof(typeMap_S[0]);


// NOTE:  for "unknown" case above, we optimistically assume that
// the low-level ODBC driver supports a string mapping.  The type of
// "unknown" and having precision and scale as zero is the best 
// description of an Oracle ROWID that we get from ODBC, so the
// mapping is needed at least for this type.


FdoSmPhOdbcColTypeMapper::FdoSmPhOdbcColTypeMapper(void)
{
}

FdoSmPhOdbcColTypeMapper::~FdoSmPhOdbcColTypeMapper(void)
{
}

FdoSmPhColType FdoSmPhOdbcColTypeMapper::String2Type( FdoString* colTypeString, int size, int scale )
{
    bool found = false;
    FdoSmPhColType type = FdoSmPhColType_Unknown;

    for ( int i=0;  !found && i < typeMapSize_S;  i++ )
    {
        FdoSmPhOdbcColTypeMapEntry* mapEntry = &typeMap_S[i];

        if ( mapEntry->m_colTypeString == colTypeString &&
            (mapEntry->m_scale == -1 || mapEntry->m_scale == scale) &&
            (mapEntry->m_size == -1 || mapEntry->m_size == size) )
        {
            type = typeMap_S[i].m_colType;
            found = true;
        }
    }

    return type;
}

FdoStringP FdoSmPhOdbcColTypeMapper::Type2String( FdoSmPhColType colType )
{
    bool found = false;
    FdoStringP name = FdoStringP::mEmptyString;

    for ( int i=0;  !found && i < typeMapSize_S;  i++ )
    {
        if ( colType == typeMap_S[i].m_colType )
        {
            name = typeMap_S[i].m_colTypeString;
            found = true;
        }
    }

    return name;
}

