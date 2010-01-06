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

#include "stdafx.h"
#include "ColTypeMapper.h"

// This Map currently has a few duplicates. This is ok since the column type
// mapper simply uses the first matched entry.
static FdoSmPhSqsColTypeMapEntry blobEntry( FdoSmPhColType_BLOB, L"image" );
static FdoSmPhSqsColTypeMapEntry binaryEntry( FdoSmPhColType_BLOB, L"binary" );
static FdoSmPhSqsColTypeMapEntry varbinaryEntry( FdoSmPhColType_BLOB, L"varbinary" );
static FdoSmPhSqsColTypeMapEntry dateEntry( FdoSmPhColType_Date, L"datetime" );
static FdoSmPhSqsColTypeMapEntry smallDateEntry( FdoSmPhColType_Date, L"smalldatetime" );
static FdoSmPhSqsColTypeMapEntry timestampEntry( FdoSmPhColType_Date, L"timestamp" );
static FdoSmPhSqsColTypeMapEntry decimalEntry( FdoSmPhColType_Decimal, L"decimal" );
static FdoSmPhSqsColTypeMapEntry numericEntry( FdoSmPhColType_Decimal, L"numeric" );
static FdoSmPhSqsColTypeMapEntry moneyEntry( FdoSmPhColType_Decimal, L"money" );
static FdoSmPhSqsColTypeMapEntry smallMoneyEntry( FdoSmPhColType_Decimal, L"smallmoney" );
static FdoSmPhSqsColTypeMapEntry doubleEntry( FdoSmPhColType_Double, L"float" );
static FdoSmPhSqsColTypeMapEntry realEntry( FdoSmPhColType_Single, L"real" );
static FdoSmPhSqsColTypeMapEntry geomEntry( FdoSmPhColType_Geom, L"geometry" );
static FdoSmPhSqsColTypeMapEntry geogEntry( FdoSmPhColType_Geom, L"geography" );
static FdoSmPhSqsColTypeMapEntry stringEntry( FdoSmPhColType_String, L"varchar" );
static FdoSmPhSqsColTypeMapEntry unicodeStringEntry( FdoSmPhColType_String, L"nvarchar" );
static FdoSmPhSqsColTypeMapEntry longIntEntry( FdoSmPhColType_Int32, L"integer" );
static FdoSmPhSqsColTypeMapEntry longIntEntryAbb( FdoSmPhColType_Int32, L"int" );
static FdoSmPhSqsColTypeMapEntry bigIntEntry( FdoSmPhColType_Int64, L"bigint" );
static FdoSmPhSqsColTypeMapEntry smallIntEntry( FdoSmPhColType_Int16, L"smallint" );
static FdoSmPhSqsColTypeMapEntry tinyIntEntry( FdoSmPhColType_Byte, L"tinyint" );
static FdoSmPhSqsColTypeMapEntry charEntry( FdoSmPhColType_String, L"char" );
static FdoSmPhSqsColTypeMapEntry ncharEntry( FdoSmPhColType_String, L"nchar" );
static FdoSmPhSqsColTypeMapEntry textEntry( FdoSmPhColType_String, L"text" );
static FdoSmPhSqsColTypeMapEntry ntextEntry( FdoSmPhColType_String, L"ntext" );
static FdoSmPhSqsColTypeMapEntry uniqueEntry( FdoSmPhColType_String, L"uniqueidentifier" );
static FdoSmPhSqsColTypeMapEntry bitEntry( FdoSmPhColType_Bool, L"bit" );

FdoSmPhSqsColTypeMapEntry* FdoSmPhSqsColTypeMapper::mMap[] = {
    &blobEntry,
    &binaryEntry,
    &varbinaryEntry,
    &dateEntry,
    &smallDateEntry,
    &timestampEntry,
    &decimalEntry,
    &numericEntry,
    &moneyEntry,
    &smallMoneyEntry,
    &doubleEntry,
    &realEntry,
    &geomEntry,
    &geogEntry,
    &stringEntry,
	&unicodeStringEntry,
    &longIntEntry,
    &longIntEntryAbb,
    &bigIntEntry,
    &smallIntEntry,
    &tinyIntEntry,
    &charEntry,
    &textEntry,
    &ncharEntry,
    &ntextEntry,
    &uniqueEntry,
    &bitEntry,
    NULL // This array must be null terminated.
};


FdoSmPhSqsColTypeMapEntry::FdoSmPhSqsColTypeMapEntry(FdoSmPhColType colType, FdoString* colTypeString, bool isUnsigned)
{
    mColType = colType;
    mColTypeString = FdoStringP(colTypeString, true);
    mIsUnsigned = isUnsigned;
}

FdoSmPhSqsColTypeMapEntry::~FdoSmPhSqsColTypeMapEntry(void)
{
}

FdoSmPhSqsColTypeMapper::FdoSmPhSqsColTypeMapper(void)
{
}

FdoSmPhSqsColTypeMapper::~FdoSmPhSqsColTypeMapper(void)
{
}

FdoSmPhColType FdoSmPhSqsColTypeMapper::String2Type( FdoString* colTypeString, bool isUnsigned, int size, int scale )
{
    int pass;
    int i;

    // Do two passes through the map:
    //  Pass 0 matches both type and unsigned setting.
    //  Pass 1 is done only if pass 0 didn't find a match. It
    //      just matches on type. For some SqlServer numeric types
    //      (e.g. double), they match to the same FDO type regardless
    //      of the signage.
    for ( pass = 0; pass < 2; pass++ ) {
        // Find the datatype enum facet in the map
        for ( i = 0; mMap[i] != NULL; i++ )
        {
            FdoSmPhSqsColTypeMapEntry* mapEntry = mMap[i];

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

FdoStringP FdoSmPhSqsColTypeMapper::Type2String( FdoSmPhColType colType )
{
    int i;

    // Find the datatype enum facet in the map
    for ( i = 0; mMap[i] != NULL; i++ )
    {
        FdoSmPhSqsColTypeMapEntry* mapEntry = mMap[i];

        if ( colType == mapEntry->mColType )
        {
            // found so return the string.
            return( (FdoString*) (mapEntry->mColTypeString) );
        }
    }

    return L""; 
}

