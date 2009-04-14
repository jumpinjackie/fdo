#ifndef FDOSMPHMTPROPERTYREADER_H
#define FDOSMPHMTPROPERTYREADER_H		1
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

//#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/Rd/QueryReader.h>

// This reader retrieves all class properties for a given 
// Feature schema. 

class FdoSmPhMtPropertyReader :
	public FdoSmPhReader
{
public:
    /// Create a reader to retrieve the properties.
    /// Call ReadNext() to read each property.
    /// Properties are ordered by class name and 
    /// property name.
	//
    /// Parameters:
    ///      froms: defines the fields to read.
    /// 	schemaName: get the properties for this feature schema.
    /// 	mgr: Physical Schema Manager.
	FdoSmPhMtPropertyReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr );

    /// Deactivate the query
	~FdoSmPhMtPropertyReader(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhMtPropertyReader() {}

private:
    /// Helper for creating inner query reader
    FdoSmPhReaderP MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhMtPropertyReader> FdoSmPhMtPropertyReaderP;

#endif


