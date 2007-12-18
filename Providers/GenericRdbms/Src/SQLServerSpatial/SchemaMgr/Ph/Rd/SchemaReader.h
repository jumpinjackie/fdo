#ifndef FDOSMPHRDSQSSCHEMAREADER_H
#define FDOSMPHRDSQSSCHEMAREADER_H		1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Sm/Ph/Rd/SchemaReader.h>

// This reader reads all of the feature schemas for the current datastore, when no MetaSchema
// information is present. It returns a schema for each user that owns objects
// in the datatore.
//

class FdoSmPhRdSqsSchemaReader : public FdoSmPhRdSchemaReader
{
public:
    /// Create a reader to autogenerate feature schemas from the physical schema.
	//
    /// Parameters:
    /// 	rows: rows and fields to retrieve.
    /// 	owner: retrieve schemas for this datastore.
    ///      dsInfo: If true, retrieve only the special row that describes
    ///          the datastore.
    ///          Otherwise, retrieve all feature schemas for the datastore.
	FdoSmPhRdSqsSchemaReader(
        FdoSmPhRowsP rows,      // Schema reader fields.
        FdoSmPhOwnerP owner, 
        bool dsInfo = false
    );

    /// Deactivates the reader
	~FdoSmPhRdSqsSchemaReader(void);

    bool ReadNext();

    // Get a string column value by name.
	// Value for current row is returned.
	// Returns L"" if value is null. 
	virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdSqsSchemaReader() {}

private:
    FdoSmPhRowsP    mRows;
    FdoSmPhOwnerP   mOwner;
    bool            mDsInfo;
};

typedef FdoPtr<FdoSmPhRdSqsSchemaReader> FdoSmPhRdSqsSchemaReaderP;

#endif


