#ifndef FDOSMPHMTCLASSREADER_H
#define FDOSMPHMTCLASSREADER_H		1
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

#include <Sm/Ph/Mt/ClassTableJoin.h>
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/QueryReader.h>

// Retrieves all classes for a given Feature schema. 

class FdoSmPhMtClassReader :
	public FdoSmPhReader
{
public:
    /// Create a reader to get the classes.
    /// Call ReadNext() to read each class.
    /// Classes are ordered by class name .
	//
	FdoSmPhMtClassReader( 
        FdoSmPhRowsP froms,         // defines the fields to read
        FdoStringP schemaName,      // get classes for this feature schema
        FdoStringP className,
        FdoSmPhMgrP mgr             // Physical Schema Manager
    );

    /// Deactivate the class reader
	~FdoSmPhMtClassReader(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhMtClassReader() {}

    // Bulk pre-fetches physical objects for the given feature schema.
    // Helps DescribeSchema performance.
    void CachePhysical( 
        FdoStringP schemaName,      // get objects for this feature schema
        FdoSmPhMgrP mgr             // Physical Schema Manager
    );

private:
    /// Helper for creating inner query reader
    FdoSmPhReaderP MakeReader( FdoSmPhRowsP froms, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className=NULL );
};

typedef FdoPtr<FdoSmPhMtClassReader> FdoSmPhMtClassReaderP;

#endif


