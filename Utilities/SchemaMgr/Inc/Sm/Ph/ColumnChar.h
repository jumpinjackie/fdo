#ifndef FDOSMPHCOLUMNCHAR_H
#define FDOSMPHCOLUMNCHAR_H		1
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
#include <Sm/Ph/ColumnVarlen.h>

// This class represents a Character (string) type column
class FdoSmPhColumnChar :
	public FdoSmPhColumnVarlen
{
public:
    /// Creates a new BLOB Column Definition.
	//
    /// Parameters:
    ///      length: maximum number of characters.
    FdoSmPhColumnChar(
        int length
	);

	virtual	~FdoSmPhColumnChar(void) 
	{
	}

    virtual FdoSmPhColType GetType()
    {
        return FdoSmPhColType_String;
    }

    /// returns required size for bind variable for this type.
    /// Number of characters plus 1 for null terminator.
    virtual int GetBindSize()
    {
        return GetLength() + 1;
    }

    /// Default define size
    virtual int GetDefineSize()
    {
    /// pick a large size to be safe.
    /// derived types must override when larger size required,
        return GetLength() + 1;
    }


    virtual FdoStringP GetBestFdoType()
    {
        return L"string";
    }

    virtual FdoStringP GetValueSql( FdoStringP val );

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhColumnChar() {}
};

typedef FdoPtr<FdoSmPhColumnChar> FdoSmPhColumnCharP;

#endif


