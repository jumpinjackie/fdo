#ifndef FDOSMPHCOLUMNUNKNOWN_H
#define FDOSMPHCOLUMNUNKNOWN_H		1
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

// This class represents a type of column not known to the current
// FDO provider. Therefore it is handled in a generic manner, meaning
// that the caller must provide the native RDBMS type for this column.
//
// The caller may also provide the length and scale if relevant. These are
// used to format the SQL clause for adding the column if new. Therefore, this
// column type allows callers to created columns of types not explicitly
// handled by the current provider.
class FdoSmPhColumnUnknown :
	public virtual FdoSmPhColumn
{
public:
	virtual	~FdoSmPhColumnUnknown(void) 
	{
	}

    virtual FdoSmPhColType GetType()
    {
        return FdoSmPhColType_Unknown;
    }

    /// Returns the maximun length for this type.
	virtual int GetLength() const
	{
		return mLength;
	}

    /// Returns the maximum number of digits to the right of the decimal point.
	virtual int GetScale() const
	{
		return( mScale );
	}

    /// Length is likely sufficient for bind size but be conservative
    /// and add scale.
    virtual int GetBindSize()
    {
        return GetLength() + GetScale() + 1;
    }

    /// Length is likely sufficient for bind size but be conservative
    /// and add scale.
    virtual int GetDefineSize()
    {
        return GetLength() + GetScale() + 1;
    }


    virtual FdoStringP GetBestFdoType()
    {
        return L"string";
    }

    /// returns the given value, formatted for inclusion in a SQL statement
    virtual FdoStringP GetValueSql( FdoStringP val );

    /// Get type and size in SQL format
    virtual FdoStringP GetTypeSql();

protected:

    /// Creates a new Unknown Column Definition.
	//
    /// Parameters:
    ///      length: total size of the column. Set to 0 if not applicable
    ///      scale: If column numeric, the number of digits to the right
    ///          of the decimal point. Set to 0 if not applicable
    FdoSmPhColumnUnknown(
        int length = 0,
        int scale = 0
	);

private:
    int mLength;
    int mScale;
};

typedef FdoPtr<FdoSmPhColumnUnknown> FdoSmPhColumnUnknownP;

#endif


