#ifndef FDOSMPHCOLUMNINT32_H
#define FDOSMPHCOLUMNINT32_H		1
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

// Represents an int32 column.
class FdoSmPhColumnInt32 :
	public virtual FdoSmPhColumn
{
public:
    /// Creates an int32 type column definition.
    FdoSmPhColumnInt32() {}

	virtual ~FdoSmPhColumnInt32(void) {}

    virtual FdoSmPhColType GetType()
    {
        return FdoSmPhColType_Int32;
    }

    // Size in bytes
    virtual int GetBinarySize()
    {
        return sizeof(long);
    }

    virtual FdoStringP GetBestFdoType()
    {
        return L"int32";
    }


};

typedef FdoPtr<FdoSmPhColumnInt32> FdoSmPhColumnInt32P;

#endif


