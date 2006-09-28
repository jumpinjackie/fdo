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

// This Map currently has a few duplicates. This is ok since the column type
// mapper simply uses the first matched entry.
static FdoSmPhMySqlColTypeMapEntry blobEntry( FdoSmPhColType_BLOB, L"longblob" );
static FdoSmPhMySqlColTypeMapEntry binaryEntry( FdoSmPhColType_BLOB, L"binary" );
static FdoSmPhMySqlColTypeMapEntry mediumBlobEntry( FdoSmPhColType_BLOB, L"mediumblob" );
static FdoSmPhMySqlColTypeMapEntry tinyBlobEntry( FdoSmPhColType_BLOB, L"tinyblob" );
static FdoSmPhMySqlColTypeMapEntry varbinaryEntry( FdoSmPhColType_BLOB, L"varbinary" );
static FdoSmPhMySqlColTypeMapEntry dateTimeEntry( FdoSmPhColType_Date, L"datetime" );
static FdoSmPhMySqlColTypeMapEntry dateEntry( FdoSmPhColType_Date, L"date" );
static FdoSmPhMySqlColTypeMapEntry timeStampEntry( FdoSmPhColType_Date, L"timestamp" );
static FdoSmPhMySqlColTypeMapEntry timeEntry( FdoSmPhColType_Date, L"time" );
static FdoSmPhMySqlColTypeMapEntry decimalEntry( FdoSmPhColType_Decimal, L"decimal" );
static FdoSmPhMySqlColTypeMapEntry singleEntry( FdoSmPhColType_Single, L"single" );
static FdoSmPhMySqlColTypeMapEntry doubleEntry( FdoSmPhColType_Double, L"double" );
static FdoSmPhMySqlColTypeMapEntry geomEntry( FdoSmPhColType_Geom, L"geometry", FdoGeometryType_MultiGeometry, FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface);
static FdoSmPhMySqlColTypeMapEntry pointEntry( FdoSmPhColType_Geom, L"point", FdoGeometryType_Point, FdoGeometricType_Point);
static FdoSmPhMySqlColTypeMapEntry lineStringEntry( FdoSmPhColType_Geom, L"linestring", FdoGeometryType_LineString, FdoGeometricType_Curve);
static FdoSmPhMySqlColTypeMapEntry polygonEntry( FdoSmPhColType_Geom, L"polygon", FdoGeometryType_Polygon, FdoGeometricType_Surface);
static FdoSmPhMySqlColTypeMapEntry geomCollectionEntry( FdoSmPhColType_Geom, L"geometrycollection", FdoGeometryType_MultiGeometry, FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface);
static FdoSmPhMySqlColTypeMapEntry multiPointEntry( FdoSmPhColType_Geom, L"multipoint", FdoGeometryType_MultiPoint, FdoGeometricType_Point);
static FdoSmPhMySqlColTypeMapEntry multiLineStringEntry( FdoSmPhColType_Geom, L"multilinestring", FdoGeometryType_MultiLineString, FdoGeometricType_Curve);
static FdoSmPhMySqlColTypeMapEntry multiPolygonEntry( FdoSmPhColType_Geom, L"multipolygon", FdoGeometryType_MultiPolygon, FdoGeometricType_Surface);
static FdoSmPhMySqlColTypeMapEntry stringEntry( FdoSmPhColType_String, L"varchar" );
static FdoSmPhMySqlColTypeMapEntry longIntEntry( FdoSmPhColType_Int32, L"int" );
static FdoSmPhMySqlColTypeMapEntry bigIntEntry( FdoSmPhColType_Int64, L"bigint" );
static FdoSmPhMySqlColTypeMapEntry medIntEntry( FdoSmPhColType_Int32, L"mediumint" );
static FdoSmPhMySqlColTypeMapEntry smallIntEntry( FdoSmPhColType_Int16, L"smallint" );
static FdoSmPhMySqlColTypeMapEntry byteEntry( FdoSmPhColType_Byte, L"tinyint", true );
static FdoSmPhMySqlColTypeMapEntry tinyIntEntry( FdoSmPhColType_Int16, L"tinyint" );
static FdoSmPhMySqlColTypeMapEntry charEntry( FdoSmPhColType_String, L"char" );
static FdoSmPhMySqlColTypeMapEntry textEntry( FdoSmPhColType_String, L"text" );
static FdoSmPhMySqlColTypeMapEntry longTextEntry( FdoSmPhColType_String, L"longtext" );
static FdoSmPhMySqlColTypeMapEntry mediumTextEntry( FdoSmPhColType_String, L"mediumtext" );
static FdoSmPhMySqlColTypeMapEntry enumEntry( FdoSmPhColType_String, L"enum" );
static FdoSmPhMySqlColTypeMapEntry setEntry( FdoSmPhColType_String, L"set" );
static FdoSmPhMySqlColTypeMapEntry yearEntry( FdoSmPhColType_Int32, L"year" );
// value range for unsigned long is not inside range for FDO int32 so map to 
// int64 instead
static FdoSmPhMySqlColTypeMapEntry ulongIntEntry( FdoSmPhColType_Int64, L"int", true );
// similarly, map unsigned small to int32 instead of int16
static FdoSmPhMySqlColTypeMapEntry usmallIntEntry( FdoSmPhColType_Int32, L"smallint", true );

FdoSmPhMySqlColTypeMapEntry* FdoSmPhMySqlColTypeMapper::mMap[] = {
    &blobEntry,
    &binaryEntry,
    &mediumBlobEntry,
    &tinyBlobEntry,
    &varbinaryEntry,
    &dateTimeEntry,
    &dateEntry,
    &timeStampEntry,
    &timeEntry,
    &decimalEntry,
    &singleEntry,
    &doubleEntry,
    &geomEntry,
    &pointEntry,
    &lineStringEntry,
    &polygonEntry,
    &geomCollectionEntry,
    &multiPointEntry,
    &multiLineStringEntry,
    &multiPolygonEntry,
    &stringEntry,
    &longIntEntry,
    &bigIntEntry,
    &medIntEntry,
    &smallIntEntry,
    &byteEntry,
    &tinyIntEntry,
    &charEntry,
    &textEntry,
    &longTextEntry,
    &mediumTextEntry,
    &enumEntry,
    &setEntry,
    &yearEntry,
    &ulongIntEntry,
    &usmallIntEntry,
    NULL // This array must be null terminated.
};


FdoSmPhMySqlColTypeMapEntry::FdoSmPhMySqlColTypeMapEntry(FdoSmPhColType colType, FdoString* colTypeString, bool isUnsigned)
{
    mColType = colType;
    mColTypeString = FdoStringP(colTypeString, false);
    mIsUnsigned = isUnsigned;
    mColFdoGeometryType = 0;
    mColFdoGeometricType = 0;
}

FdoSmPhMySqlColTypeMapEntry::FdoSmPhMySqlColTypeMapEntry(FdoSmPhColType colType, FdoString* colTypeString, FdoInt32 colFdoGeometryType, FdoInt32 colFdoGeometricType)
{
    mColType = colType;
    mColTypeString = FdoStringP(colTypeString, false);
    mIsUnsigned = false;
    mColFdoGeometryType = colFdoGeometryType;
    mColFdoGeometricType = colFdoGeometricType;
}

FdoSmPhMySqlColTypeMapEntry::~FdoSmPhMySqlColTypeMapEntry(void)
{
}

FdoSmPhMySqlColTypeMapper::FdoSmPhMySqlColTypeMapper(void)
{
}

FdoSmPhMySqlColTypeMapper::~FdoSmPhMySqlColTypeMapper(void)
{
}

FdoSmPhColType FdoSmPhMySqlColTypeMapper::String2Type( FdoString* colTypeString, bool isUnsigned, int size, int scale )
{
    int pass;
    int i;

    // The mapping of bit columns depends on the number of bits.

    if ( FdoStringP(colTypeString).ICompare("bit") == 0 ) {
        if ( size <= 1 ) 
            return FdoSmPhColType_Bool;
        if ( size <= 8 ) 
            return FdoSmPhColType_Byte;
        if ( size <= 15 ) 
            return FdoSmPhColType_Int16;
        if ( size <= 31 ) 
            return FdoSmPhColType_Int32;
    
        return FdoSmPhColType_Int64;
    }

    // Do two passes through the map:
    //  Pass 0 matches both type and unsigned setting.
    //  Pass 1 is done only if pass 0 didn't find a match. It
    //      just matches on type. For some Mysql numeric types
    //      (e.g. double), they match to the same FDO type regardless
    //      of the signage.
    for ( pass = 0; pass < 2; pass++ ) {
        // Find the datatype enum facet in the map
        for ( i = 0; mMap[i] != NULL; i++ )
        {
            FdoSmPhMySqlColTypeMapEntry* mapEntry = mMap[i];

            // pass 0 matches the sign, pass 1 does not.
            if ( (mapEntry->mColTypeString == colTypeString) &&
                 ((pass == 1) || (mapEntry->mIsUnsigned == isUnsigned))
            ) {
                // found so return the string.
                return( mapEntry->mColType );
            }
        }
    }

    // Get to here only if type not found.
    // Return the Unknown type.
    return(FdoSmPhColType_Unknown);
}

FdoStringP FdoSmPhMySqlColTypeMapper::Type2String( FdoSmPhColType colType )
{
    int i;

    // Find the datatype enum facet in the map
    for ( i = 0; mMap[i] != NULL; i++ )
    {
        FdoSmPhMySqlColTypeMapEntry* mapEntry = mMap[i];

        if ( colType == mapEntry->mColType )
        {
            // found so return the string.
            return( (FdoString*) (mapEntry->mColTypeString) );
        }
    }

    return L"";
}

FdoInt32 FdoSmPhMySqlColTypeMapper::GetColFdoGeometryType(FdoString* colTypeString)
{
    for (int  i = 0; mMap[i] != NULL; i++ )
    {
        FdoSmPhMySqlColTypeMapEntry* mapEntry = mMap[i];
        if ( mapEntry->mColTypeString == colTypeString )
            return mapEntry->mColFdoGeometryType;
    }
    return FdoGeometryType_MultiGeometry;
}

FdoInt32 FdoSmPhMySqlColTypeMapper::GetColFdoGeometricType(FdoString* colTypeString)
{
    for (int i = 0; mMap[i] != NULL; i++ )
    {
        FdoSmPhMySqlColTypeMapEntry* mapEntry = mMap[i];
        if ( mapEntry->mColTypeString == colTypeString )
            return mapEntry->mColFdoGeometricType;
    }
    return (FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface);
}
