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
#include "ColTypeMapper.h"

// This Map currently has a few duplicates.
// This is OK since the column type mapper simply
// uses the first matched entry.

namespace
{
    // Short alias provided for convenience.
    typedef FdoSmPhPostGisColTypeMapEntry TypeEntry;
}

//
// Binary Data Types
//

static TypeEntry byteaEntry(FdoSmPhColType_BLOB, L"bytea");
//static TypeEntry byteaEntry(FdoSmPhColType_Byte, L"bytea");

//
// Date/Time Types
//

static TypeEntry dateEntry(FdoSmPhColType_Date, L"date");
static TypeEntry timeEntry(FdoSmPhColType_Date, L"time");
static TypeEntry abstimeEntry(FdoSmPhColType_Date, L"abstime");
static TypeEntry timestampEntry(FdoSmPhColType_Date, L"timestamp");
static TypeEntry timestamptzEntry(FdoSmPhColType_Date, L"timestamp with time zone");
static TypeEntry timestampwtzEntry(FdoSmPhColType_Date, L"timestamp without time zone");

//
// Numeric Types
//

// Integer Types
static TypeEntry smallintEntry(FdoSmPhColType_Int16, L"smallint");
static TypeEntry integerEntry(FdoSmPhColType_Int32, L"integer");
static TypeEntry bigintEntry(FdoSmPhColType_Int64, L"bigint");
static TypeEntry int2Entry(FdoSmPhColType_Int16, L"int2");
static TypeEntry int4Entry(FdoSmPhColType_Int32, L"int4");
static TypeEntry int8Entry(FdoSmPhColType_Int64, L"int8");

// Serial Types
// serial type is mapped automatically to integer

// Arbitrary Precision Numbers
static TypeEntry numericEntry(FdoSmPhColType_Decimal, L"numeric");
static TypeEntry decimalEntry(FdoSmPhColType_Decimal, L"decimal");
// Note:  The money type is deprecated.
static TypeEntry moneyEntry(FdoSmPhColType_Decimal, L"money");

// Floating-Point Types
static TypeEntry realEntry(FdoSmPhColType_Single, L"real");
static TypeEntry doubleEntry(FdoSmPhColType_Double, L"double precision");
static TypeEntry float4Entry(FdoSmPhColType_Single, L"float4");
static TypeEntry float8Entry(FdoSmPhColType_Double, L"float8");

//
// Text
//
// Default size for fixed character types is 1.
// Default for varying types is unlimited. Set "practical" limit of 1Gb.
static TypeEntry characterEntry(FdoSmPhColType_String, L"character", 1);
static TypeEntry charactervaryingEntry(FdoSmPhColType_String, L"character varying", 1073741824);
static TypeEntry nameEntry(FdoSmPhColType_String, L"name", 63);
static TypeEntry textEntry(FdoSmPhColType_String, L"text", 1073741824);
static TypeEntry charEntry(FdoSmPhColType_String, L"char", 1);
static TypeEntry bpcharEntry(FdoSmPhColType_String, L"bpchar", 1);
static TypeEntry varcharEntry(FdoSmPhColType_String, L"varchar", 1073741824);
// Note: 'cstring' is a pseudo-type and represents a null-terminated C string
static TypeEntry cstringEntry(FdoSmPhColType_String, L"cstring",1073741824);
// Note: 'bit' is a bit string (sequence of '0's and '1's ).
static TypeEntry bitEntry(FdoSmPhColType_String, L"bit",1);
static TypeEntry bitVaryingEntry(FdoSmPhColType_String, L"varbit",1073741824);

//
// Boolean Type
//

static TypeEntry boolEntry(FdoSmPhColType_Bool, L"bool");
static TypeEntry booleanEntry(FdoSmPhColType_Bool, L"boolean");

//
// Object Identifier Types
//
// Note: There are no FDO types for PostgreSQL identifiers,
//       so they're mapped to integer types

static TypeEntry oidEntry(FdoSmPhColType_Int32, L"oid");
static TypeEntry cidEntry(FdoSmPhColType_Int32, L"cid");
static TypeEntry xidEntry(FdoSmPhColType_Int32, L"xid");
// Note: tid is 6-bytes long
static TypeEntry tidEntry(FdoSmPhColType_Int64, L"tid");

//
// Geometry
//

static TypeEntry geometryEntry(FdoSmPhColType_Geom, L"geometry");

//
// Unknown Types
//

static TypeEntry anyEntry(FdoSmPhColType_Unknown, L"any");
static TypeEntry anyarrayEntry(FdoSmPhColType_Unknown, L"anyarray");
static TypeEntry anyelementEntry(FdoSmPhColType_Unknown, L"anyelement");
static TypeEntry arrayEntry(FdoSmPhColType_Unknown, L"ARRAY");
static TypeEntry cidrEntry(FdoSmPhColType_Unknown, L"cidr");
static TypeEntry inetEntry(FdoSmPhColType_Unknown, L"inet");
static TypeEntry macaddrEntry(FdoSmPhColType_Unknown, L"macaddr");
static TypeEntry internalEntry(FdoSmPhColType_Unknown, L"internal");
static TypeEntry voidEntry(FdoSmPhColType_Unknown, L"void");


TypeEntry* FdoSmPhPostGisColTypeMapper::mMap[] =
{
    &byteaEntry,
    &dateEntry,
    &timeEntry,
    &abstimeEntry,
    &timestampEntry,
    &timestamptzEntry,
    &timestampwtzEntry,
    &smallintEntry,
    &integerEntry,
    &bigintEntry,
    &int2Entry,
    &int4Entry,
    &int8Entry,
    &numericEntry,
    &decimalEntry,
    &moneyEntry,
    &realEntry,
    &doubleEntry,
    &float4Entry,
    &float8Entry,
    &characterEntry,
    &charactervaryingEntry,
    &nameEntry,
    &textEntry,
    &charEntry,
    &bpcharEntry,
    &varcharEntry,
    &cstringEntry,
    &bitEntry,
    &bitVaryingEntry,
    &boolEntry,
    &booleanEntry,
    &oidEntry,
    &cidEntry,
    &xidEntry,
    &tidEntry,
    &geometryEntry,

    NULL // This array must be null terminated.
};

FdoSmPhPostGisColTypeMapEntry::FdoSmPhPostGisColTypeMapEntry(
    FdoSmPhColType colType,
    FdoString* colTypeString,
    FdoInt32 defaultSize)
    : mColType(colType), 
      mColTypeString(FdoStringP(colTypeString, false)),
      mDefaultSize(defaultSize)
{
    // idle
}

FdoSmPhPostGisColTypeMapEntry::~FdoSmPhPostGisColTypeMapEntry()
{
    // idle
}

FdoSmPhPostGisColTypeMapper::FdoSmPhPostGisColTypeMapper()
{
    // idle
}

FdoSmPhPostGisColTypeMapper::~FdoSmPhPostGisColTypeMapper()
{
    // idle
}

FdoSmPhColType FdoSmPhPostGisColTypeMapper::String2Type(
    FdoString* colTypeString,
    int typmod,
    FdoInt32& size,
    FdoInt32& scale)
{
    size = 0;
    scale = 0;

    // Make sure decimal types without defined precision become FDO double type 
    // instead of FDO decimal
    if ((0 == FdoStringP(colTypeString).ICompare("numeric")) && (typmod <= 0))
    {
        return FdoSmPhColType_Double;
    }

    if ((0 == FdoStringP(colTypeString).ICompare("decimal")) && (typmod <= 0))
    {
        return FdoSmPhColType_Double;
    }

    // Find the datatype enum facet in the map
    for (int i = 0; mMap[i] != NULL; i++)
    {
        TypeEntry* mapEntry = mMap[i];

        if (mapEntry->mColTypeString == colTypeString)
        {
            if ( (mapEntry->mColType == FdoSmPhColType_Decimal) && (typmod > 0)) {
                // For decimal types, size (precision) is in high-order 16bits of typemod
                size = typmod / 65536;
                // For decimal types, scale is in lower 16 bits.
                scale = typmod - (size * 65536) - 4;
            }
            else if ( (mapEntry->mColType == FdoSmPhColType_String) && (typmod > 0) && (mapEntry->mColTypeString == L"bit" || mapEntry->mColTypeString == L"varbit") ) {
                // For bit types, size is typmod
                size = typmod;
            }
            else if ( (mapEntry->mColType == FdoSmPhColType_String) && (typmod > 4) ) {
                // For other string types, need to subtract 4
                size = typmod - 4;
            }
            else {
                size = mapEntry->mDefaultSize;
            }

            // found so return the string.
            return mapEntry->mColType;
        }
    }

    // Get to here only if type not found.
    // Return the Unknown type.
    return FdoSmPhColType_Unknown;
}

FdoStringP FdoSmPhPostGisColTypeMapper::Type2String(
    FdoSmPhColType colType)
{
    // Find the datatype enum facet in the map
    for (int i = 0; mMap[i] != NULL; i++)
    {
        TypeEntry* mapEntry = mMap[i];

        if (colType == mapEntry->mColType)
        {
            // Found, so return the string.
            return (static_cast<FdoString*>(mapEntry->mColTypeString));
        }
    }

    return L"";
}

