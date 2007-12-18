#ifndef FDOSMPHSQSCOLTYPEMAPPER_H
#define FDOSMPHSQSCOLTYPEMAPPER_H       1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/NamedCollection.h>
#include <Sm/Ph/Coltype.h>

// A single entry for translating a column type between
// enum facet and SqlServer string format.
class FdoSmPhSqsColTypeMapEntry //: public FdoSmDisposable
{
public:
    // Create a column type mapping
    //
    // Parameters:
    //      colType: the enum facet
    //      colTypeString: the corresponding string.

    FdoSmPhSqsColTypeMapEntry(FdoSmPhColType colType, FdoString* colTypeString, bool isUnsigned = false);
    ~FdoSmPhSqsColTypeMapEntry(void);

    // Get the Map entry in string form
    FdoString* GetName() const
    {
        return( mColTypeString );
    }

    FdoSmPhColType mColType;
    FdoStringP mColTypeString;
    bool mIsUnsigned;
};

// Lists all FDO Property types with their corresponding
// enum facet and string representations.
class FdoSmPhSqsColTypeMapper
{
public:
    FdoSmPhSqsColTypeMapper(void);
    ~FdoSmPhSqsColTypeMapper(void);

    // Converts string to facet
    //
    // Parameters:
    //      colTypeString: the string.
    static FdoSmPhColType String2Type(
        FdoString* colTypeString,
        bool isUnsigned = false,
        int size = 0,
        int scale = 0
    );
    // Converts facet to string
    static FdoStringP Type2String( FdoSmPhColType colType );

private:
    // Enum-String mappings for each property type.
    static FdoSmPhSqsColTypeMapEntry* mMap[];
};

#endif
