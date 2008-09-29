#ifndef FDOSMPHRDINDEXREADER_H
#define FDOSMPHRDINDEXREADER_H		1
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/Index.h>

// This abstract class reads indexes. It retrieves one row per index-column
// combination. Each provider must provide its own implementing class.

class FdoSmPhRdIndexReader : public FdoSmPhReader
{
public:
    /// Returns the type of the current index (Scalar or Spatial)
    virtual FdoSmPhIndexType GetIndexType();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdIndexReader() {}

    /// Create an index reader
	//
	FdoSmPhRdIndexReader(
        FdoSmPhReaderP reader    // inner reader created by implementing class
    );

	FdoSmPhRdIndexReader(
        FdoSmPhMgrP         mgr,                // Physical Schema Manager
        FdoSmPhRowsP        rows                // row and field layout
    );

    /// Deactivates the reader.
	~FdoSmPhRdIndexReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per index and index column attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhRdIndexReader> FdoSmPhRdIndexReaderP;

#endif


