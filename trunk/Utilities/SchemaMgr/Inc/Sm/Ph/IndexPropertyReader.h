#ifndef FDORDBMSPHINDEXPROPERTYREADER_H
#define FDORDBMSPHINDEXPROPERTYREADER_H		1
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

#include "ClassIndexReader.h"

// Retrieves all properties for a given 
// Index. 

class FdoRdbmsPhIndexPropertyReader : public FdoRdbmsPhGroupReader
{
public:
    /// Attach to an index class reader to read the properties
    /// for the current index for this reader. 
    /// Call ReadNext() to read each property.
    /// Properties are ordered by position within the index.
	//
    /// Parameters:
    /// 	schemaName: the schema containing the class.
    /// 	className: class for this index.
    /// 	indexName: get the properties for this index.
    /// 	pIndexReader: the class index reader.
	FdoRdbmsPhIndexPropertyReader(
		FdoStringP schemaName, 
		FdoStringP className, 
		FdoStringP indexName, 
		FdoRdbmsPhClassIndexReader* pIndexReader
	);

	~FdoRdbmsPhIndexPropertyReader(void);

    /// Used by base class to get the current row group. Rows are grouped by 
    /// class name.
	virtual FdoStringP GetGroupName();


	FdoStringP GetFeatureClassName();
	FdoStringP GetIndexName();
    bool GetIsUnique();
	FdoStringP GetAttributeName();
	int GetPosition();

private:
	FdoRdbmsPhClassIndexReader* mpIndexReader;
	FdoStringP mSchemaName;
	FdoStringP mClassName;
    FdoStringP mIndexName;
};

#endif


