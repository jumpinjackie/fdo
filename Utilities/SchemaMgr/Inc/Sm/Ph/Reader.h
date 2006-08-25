#ifndef FDOSMPHREADER_H
#define FDOSMPHREADER_H		1
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

class FdoSmPhReader;  //forward declaration
typedef FdoPtr<FdoSmPhReader> FdoSmPhReaderP;

#include <Sm/Ph/ReadWrite.h>


//Abstract Physical Schema Reader class.
class FdoSmPhReader : public FdoSmPhReadWrite
{
public:
    /// Create a telescoped reader (wraps around an inner reader)
	FdoSmPhReader(FdoPtr<FdoSmPhReader> subReader);

    /// Create a non-telescoped reader
	FdoSmPhReader(
        FdoSmPhMgrP mgr,        // Physical Schema Manager
        FdoSmPhRowsP pRow       // row collection describing the field list.
        );
	~FdoSmPhReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();

    /// Returns true if query is before first row.
	bool IsBOF();

    /// Returns true if query is after last row.
	bool IsEOF();

	/// End select before end-of-fetch
	virtual void EndSelect();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhReader() {}

    /// Set the before-first and after end indicators
	void SetBOF(bool bBOF);
	void SetEOF(bool bEOF);

    /// Validate that the reader is positioned on a row.
    /// Throws an exception if not. 
	void CheckGet();

    /// Setter:
    void SetSubReader(FdoPtr<FdoSmPhReader> subReader);

private:
    /// at start and at end indicators
	bool mbBOF;
	bool mbEOF;

    FdoPtr<FdoSmPhReader> mSubReader;
};


#endif


