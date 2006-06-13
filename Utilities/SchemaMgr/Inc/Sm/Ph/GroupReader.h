#ifndef FDOSMPHGROUPREADER_H
#define FDOSMPHGROUPREADER_H		1
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

// Retrieves sub-section (group) of rows from 
// a reader.
//
// This class can be attached to a Query Reader that returns order
// rows. It retrieves the sub-set of rows for a specified group. A group
// is a set of rows whose order columns have the same value

class FdoSmPhGroupReader : public FdoSmPhReader
{
public:
    /// Attaches to a query reader. 
    /// Call ReadNext() to read each row.
	//
    /// Parameters:
    /// 	groupName: the name of the group of rows to retrieve.
    /// 		only rows whose ordering columns equal this value 
    /// 		will be retrieved. Must be a concatenation of the 
    /// 		of the ordering column values for the rows to retrieve.
    /// 	pReader: the reader to attach to. The reader must return
    /// 		rows in ascending order by group name. Note that if this
    /// 		reader is currently positioned after the group to retrieve
    /// 		then this reader returns no rows.
	FdoSmPhGroupReader(FdoStringP groupName, FdoSmPhReaderP reader);

	~FdoSmPhGroupReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();

    /// Get a string column value by name.
    /// Value for current row is returned.
    /// Returns L"" if value is null. 
	virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

    /// This function determines the group name for the attached reader's
    /// current row. Classes derived from this class can implement this 
    /// function in any way, as long as the reader retrieves rows in 
    /// ascending order by group name.
	virtual FdoStringP GetGroupName() = 0;

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhGroupReader() {}

private:
	FdoStringP mGroupName;
	FdoSmPhReaderP mReader;
};

#endif


