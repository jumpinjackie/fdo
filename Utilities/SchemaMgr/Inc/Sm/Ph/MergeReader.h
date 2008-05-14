#ifndef FDOSMPHMERGEREADER_H
#define FDOSMPHMERGEREADER_H		1
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

#include <Sm/Ph/Reader.h>

class FdoSmPhTable;

// Merges two readers
//
// This class can be wrapped around two readers that return similar information.
// Both readers must retrieve rows ordered by a certain key. This reader will
// effectively merge the results sets of both readers, maintaining the order by key.

class FdoSmPhMergeReader : public FdoSmPhReader
{
public:
    // Parameters:
    // 	reader1: first reader
    //  reader2: second reader
    //  unionAll: if true, return all rows from the two readers
    //   if false, then skip any rows from reader2 whose key is the key of at least
    //   one row from reader1. 
	FdoSmPhMergeReader(FdoSmPhReaderP reader1, FdoSmPhReaderP reader2, bool unionAll = false);

	~FdoSmPhMergeReader(void);

    // Get the next row and make it current.
	virtual bool ReadNext();

	// End of select
	virtual void EndSelect();

    /// Get the column's field definition.
    virtual FdoSmPhFieldP GetField( FdoStringP tableName, FdoStringP fieldName );

    // Get a string column value by name.
    // Value for current row is returned.
    // Returns L"" if value is null. 
	virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhMergeReader() {}

    // This function retrieves the key for the current row in the given reader. 
    //
    // Parameters:
    // 	reader: the reader
    //  firstReader: if true then this is the first reader. Otherwise, it is the second reader.
	virtual FdoStringP GetKeyValue( FdoSmPhReaderP reader, bool firstReader ) = 0;

    // returns the sub-reader to read the current row from.
    FdoSmPhReaderP GetCurrentReader();

private:
	FdoSmPhReaderP mReader1;
	FdoSmPhReaderP mReader2;
    bool           mUnionAll;
};

#endif


