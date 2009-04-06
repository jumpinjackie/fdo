#ifndef FDOSMPHCOLUMNBLOB_H
#define FDOSMPHCOLUMNBLOB_H		1
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

// This class represents a BLOB type column
class FdoSmPhColumnBLOB :
	public virtual FdoSmPhColumn
{
public:
    /// Creates a new BLOB Column Definition.
    FdoSmPhColumnBLOB() {}

    virtual FdoSmPhColType GetType() const
    {
        return FdoSmPhColType_BLOB;
    }

    /// Returns the maximun length for this type.
	virtual int GetLength()
	{
		return 0; // Size is not known.
	}

    virtual FdoStringP GetBestFdoType()
    {
        return L"blob";
    }

	~FdoSmPhColumnBLOB(void) {}
private:

};

typedef FdoPtr<FdoSmPhColumnBLOB> FdoSmPhColumnBLOBP;

#endif


