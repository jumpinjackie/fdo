#ifndef FDOSMPHCOLUMNDECIMAL_H
#define FDOSMPHCOLUMNDECIMAL_H		1
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

// This class represents a fixed precision decimal number.
class FdoSmPhColumnDecimal :
	public FdoSmPhColumnVarlen
{
public:
    /// Creates a new Decimal Column Definition.
	//
    /// Parameters:
    ///      length: maximum number of digits, including fractional part
    ///      scale: maximum number of digits in fractional part
    FdoSmPhColumnDecimal(
        int length,
        int scale
	);

	~FdoSmPhColumnDecimal(void) {}

    /// Returns the maximum number of digits to the right of the decimal point.
	virtual int GetScale() const
	{
		return( mScale );
	}

    virtual FdoStringP GetBestFdoType()
    {
        return L"decimal";
    }

    // Size in bytes.
    virtual int GetBinarySize()
    {
        return sizeof(double);
    }

    virtual FdoSmPhColType GetType()
    {
        return FdoSmPhColType_Decimal;
    }

    virtual FdoStringP GetTypeSql()
    {
        return FdoStringP::Format( L"%ls (%d, %d)", (FdoString*) GetTypeName(), GetLength(), GetScale() );
    }

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhColumnDecimal() {}

private:
	int mScale;
};

typedef FdoPtr<FdoSmPhColumnDecimal> FdoSmPhColumnDecimalP;

#endif


