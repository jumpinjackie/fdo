#ifndef FDORDBMSPHCLASSINDEXREADER_H
#define FDORDBMSPHCLASSINDEXREADER_H		1
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

#include "IndexReader.h"
#include "GroupReader.h"

// Retrieves all indexes for a given 
// Class. 

class FdoRdbmsPhIndexPropertyReader;

class FdoRdbmsPhClassIndexReader : public FdoRdbmsPhGroupReader
{
public:
    /// Attach to an index reader to read the indexes
    /// for a class. 
    /// Call ReadNext() to read each index.
    /// Indexes are ordered by index name.
	//
    /// Parameters:
    /// 	schemaName: the schema containing the class.
    /// 	className: get the properties for this class.
    /// 	pIndexReader: reads indexes and their properties ordered by class
    /// 		name, index name and property position within index. 
	FdoRdbmsPhClassIndexReader(
		FdoStringP schemaName, 
		FdoStringP className, 
		FdoRdbmsPhIndexReader* pIndexReader
	);

	~FdoRdbmsPhClassIndexReader(void);

    /// Used by base class to get the current row group. Rows are grouped by 
    /// class name.
	virtual FdoStringP GetGroupName();

	FdoStringP GetIndexName();
    bool GetIsUnique();
	FdoStringP GetFeatureClassName();
	FdoStringP GetAttributeName();
	int GetAttributePosition();

    /// Returns a reader to read the properties of the current index.
    FdoRdbmsPhIndexPropertyReader* GetPropertyReader();

private:
	FdoRdbmsPhIndexReader* mpIndexReader;
	FdoStringP mSchemaName;
	FdoStringP mClassName;
    FdoStringP mIndexName;
};

#endif


