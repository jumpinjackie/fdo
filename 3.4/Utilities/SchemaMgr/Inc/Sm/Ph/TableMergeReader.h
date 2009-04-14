#ifndef FDOSMPHTABLEMERGEREADER_H
#define FDOSMPHTABLEMERGEREADER_H		1
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

#include <Sm/Ph/MergeReader.h>
#include <Sm/Ph/Rd/IndexReader.h>

// Specialized merge reader that merges two readers of table information.

class FdoSmPhTableMergeReader : public FdoSmPhMergeReader
{
public:
	FdoSmPhTableMergeReader(
        // keyTableName and keyFieldName indicate the key field for the readers' result sets.
        FdoStringP keyTableName, 
        FdoStringP keyFieldName, 
        // The two readers to merge.
        FdoSmPhReaderP reader1,
        FdoSmPhReaderP reader2
    );

	virtual ~FdoSmPhTableMergeReader(void);

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableMergeReader() {}

    // This function retrieves the key for the current row in the given reader. 
    //
    // Parameters:
    // 	reader: the reader
    //  firstReader: if true then this is the first reader. Otherwise, it is the second reader.
	virtual FdoStringP GetKeyValue( FdoSmPhReaderP reader, bool firstReader );


private:
	FdoStringP mKeyTableName;
	FdoStringP mKeyFieldName;
};

typedef FdoPtr<FdoSmPhTableMergeReader> FdoSmPhTableMergeReaderP;

#endif


