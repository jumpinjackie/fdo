#ifndef FDOSMLPCLASSTYPEMAPPER_H
#define FDOSMLPCLASSTYPEMAPPER_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Disposable.h>
#include <Sm/NamedCollection.h>

// A single entry for translating an FDO class type between
// enum facet and string format.
class FdoSmLpClassTypeMapEntry 
{
public:
    /// Create a class type mapping
	//
    /// Parameters:
    /// 	dataType: the enum facet
    /// 	dataTypeString: the corresponding string.

	FdoSmLpClassTypeMapEntry(FdoClassType classType, wchar_t* propTypeString);
	~FdoSmLpClassTypeMapEntry(void);

    /// Get the Map entry in string form
	const wchar_t* GetName() const
	{
		return( mClassTypeString );
	}

	FdoClassType mClassType;
	FdoStringP mClassTypeString;
};

// Lists all FDO Class types with their corresponding
// enum facet and string representations.
class FdoSmLpClassTypeMapper
{
public:
	FdoSmLpClassTypeMapper(void);
	~FdoSmLpClassTypeMapper(void);

    /// Converts string to facet
	//
    /// Parameters:
    /// 	classTypeString: the string.
    /// 	pbFound: (output)
    /// 		true: if the type was found.
    /// 		false: otherwise. Return value is meanless in this case
    /// 			and must not be used.
    /// 		if not specified (NULL) then an exception is thrown
    /// 		when the type is not found.
	static FdoClassType String2Type( FdoString* classTypeString, bool* pbFound = NULL );
    /// Converts facet to string
	static FdoStringP Type2String( FdoClassType classType );

private:
    /// Enum-String mappings for each class type.
	static FdoSmLpClassTypeMapEntry* mMap[];
};

#endif


