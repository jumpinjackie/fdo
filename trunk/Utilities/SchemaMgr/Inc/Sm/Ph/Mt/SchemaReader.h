#ifndef FDOSMPHMTSCHEMAREADER_H
#define FDOSMPHMTSCHEMAREADER_H		1
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

#include <Sm/Ph/Rd/QueryReader.h>

// This reader retrieves the list of Feature Schemas for the current connection.

class FdoSmPhMtSchemaReader : public FdoSmPhReader
{
public:
    /// Create a reader to retrieve the Feature Schemas.
    /// Call ReadNext() to read each schema.
    /// Schemas are retrieved ordered by name.
	//
    /// Parameters:
    ///      froms: defines the fields to read.
    /// 	owner: retrieve schemas for this datastore.
    ///      dsInfo: If true, retrieve only the special row that describes
    ///          the datastore.
    ///          Otherwise, retrieve all feature schemas for the datastore.
	FdoSmPhMtSchemaReader(FdoSmPhRowsP froms, FdoSmPhOwnerP owner, bool dsInfo = false);

    /// Deactivate the query
	~FdoSmPhMtSchemaReader(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhMtSchemaReader() {}

private:
    /// Helper for creating inner query reader
    FdoSmPhReaderP MakeReader(FdoSmPhRowsP froms, FdoSmPhOwnerP owner, bool dsInfo);
};

#endif


