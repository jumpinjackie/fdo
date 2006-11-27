#ifndef FDOSMPHCOLUMNVARLEN_H
#define FDOSMPHCOLUMNVARLEN_H		1
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

#include <Sm/Ph/Column.h>

// base class for column types that need a length.
class FdoSmPhColumnVarlen :
	public virtual FdoSmPhColumn
{
public:
    /// Returns the maximum length for this column.
	virtual int GetLength() const
	{
		return mLength;
	}

    /// Returns the minimum possible length for this type of column.
    virtual int GetMinLength() const;

    /// Get SQL sub-clause for column type indicator
    virtual FdoStringP GetTypeSql()
    {
        return FdoStringP::Format(
            L"%ls (%ld)",
            (FdoString*) GetTypeName(),
            GetLength()
        );
    }

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhColumnVarlen() {}

    /// Creates a length-dependent type of column
	//
    /// Parameters:
    ///      length: maximum length of the column.
    FdoSmPhColumnVarlen(
        int length
	);


	virtual ~FdoSmPhColumnVarlen(void) {}

	int mLength;
};

#endif


