#ifndef FDORDBMSPHINDEXREADER_H
#define FDORDBMSPHINDEXREADER_H		1
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

#include "DependencyReader.h"

//class FdoRdbmsPhTable;

// This class retrieves all indexes and their properties

class FdoRdbmsPhIndexReader :
	public FdoRdbmsPhQueryReader
{
public:
    /// Create and execute a query to list all indexes and their properties for
    /// a given feature schema.
    /// Call ReadNext() to read each propertu.
    /// Properties are ordered by class name, Index name, and property position
    /// within index.
	//
    /// Parameters:
    /// 	schemaName: get the properties for this schema.
    /// 	physicalSchema: holds the connection for a database.
    /// 		All Feature Schemas in this database are retrieved.
	FdoRdbmsPhIndexReader(FdoStringP schemaName, FdoRdbmsPhysicalSchema* physicalSchema);

    /// Deactivate the query
	~FdoRdbmsPhIndexReader(void);

    /// Various getters for Index attributes

    /// Index name
	FdoStringP GetFeatureClassName();

    /// Index name
	FdoStringP GetIndexName();

    /// true if Index column has fixed name specified through 
    /// schema overrides
    bool GetIsUnique();

    /// Physical column name for the Index
	FdoStringP GetAttributeName();

    /// If the Index is an identity Index, this function returns its position
    /// within the class's identity Index collection.
    /// Otherwise, it returns 0.
	int GetAttributePosition();

private:
    FdoRdbmsPhFromIntoCollection* MakeTables( FdoRdbmsPhysicalSchema* physicalSchema );
    FdoStringP MakeClauses( FdoStringP schemaName, FdoRdbmsPhysicalSchema* physicalSchema );
};

#endif


