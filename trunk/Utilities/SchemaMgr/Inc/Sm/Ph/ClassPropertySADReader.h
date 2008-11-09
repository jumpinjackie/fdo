#ifndef FDOSMPHCLASSPROPERTYSADREADER_H
#define FDOSMPHCLASSPROPERTYSADREADER_H		1
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

#include <Sm/Ph/SADReader.h>
#include <Sm/Ph/GroupReader.h>

// Reads the SAD ( Schema Attribute Dictionary ) rows for a given property.

class FdoSmPhClassPropertySADReader :
	public FdoSmPhGroupReader,
	public FdoSmPhISADReader
{
public:
    /// Create and execute a query to read the SAD rows for a given property
    /// Call ReadNext() to read each SAD row.
    /// SAD rows are ordered by name
	//
    /// Parameters:
    /// 	schemaName: property's schema
    /// 	className: property's class
    /// 	propertyName: get SAD rows for this property
    /// 	pSADReader: get the rows from this SAD reader. This reader must
    /// 		return the rows ordered by schema, class, property and name.
    /// 		Caller is responsible for ensuring that this reader has not already
    /// 		passed the given property. If it has, then no rows are read.
	FdoSmPhClassPropertySADReader( FdoStringP schemaName, FdoStringP className, FdoStringP propertyName, FdoSmPhSADReaderP pSADReader );

    /// Deactivate the query
	~FdoSmPhClassPropertySADReader(void);

    /// Used by base class to get the current row group. Rows are grouped by 
    /// Schema, Class and property.
	virtual FdoStringP GetGroupName();

    /// Get the next SAD row and make it current.
	virtual bool ReadNext() 
	{
		return FdoSmPhGroupReader::ReadNext();
	}

    /// Various getters for SAD attributes

    /// SAD entry name
	virtual FdoStringP GetName();

    /// SAD entry value
	virtual FdoStringP GetValue();

private:
	FdoSmPhSADReaderP mpSADReader;
};

typedef FdoPtr<FdoSmPhClassPropertySADReader> FdoSmPhClassPropertySADReaderP;

#endif


