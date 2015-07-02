#ifndef FDOSMPHWRITER_H
#define FDOSMPHWRITER_H		1
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

#include <Sm/Ph/ReadWrite.h>

// This interface must be implemented by all physical schema writers. 

class FdoSmPhWriter : public FdoSmPhReadWrite
{
public:
    virtual ~FdoSmPhWriter() {}

    /// Get the specification for the table being written to
    FdoSmPhRowP GetRow();

    /// Add the current row to the table.
	virtual void Add();

    /// Modify a row or set of rows.
    //
    /// Parameters:
    ///      sClauses: a where clause that identifies the row(s) to modify.
    virtual void Modify( FdoStringP sClauses );

    /// Delete a row or set of rows.
    //
    /// Parameters:
    ///      sClauses: a where clause that identifies the row(s) to delete.
    virtual void Delete( FdoStringP sClauses );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhWriter() {}
	FdoSmPhWriter(FdoPtr<FdoSmPhWriter> subWriter);
	FdoSmPhWriter(FdoSmPhMgrP mgr, FdoSmPhRowP row);

	void SetSubWriter(FdoPtr<FdoSmPhWriter> subWriter);

private:
    FdoSmPhRowsP MakeCollection(FdoSmPhRowP row);

    FdoPtr<FdoSmPhWriter> mSubWriter;
};

typedef FdoPtr<FdoSmPhWriter> FdoSmPhWriterP;

#endif


