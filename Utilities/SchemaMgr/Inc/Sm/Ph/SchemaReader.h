#ifndef FDOSMPHSCHEMAREADER_H
#define FDOSMPHSCHEMAREADER_H		1
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/SOReader.h>
#include <Sm/Ph/SchemaSOReader.h>

// This class retrieves the list of Feature Schemas present in a database.

class FdoSmPhSchemaReader : public FdoSmPhReader
{
public:
    /// Create and execute a query to list the Feature Schemas.
    /// Call ReadNext() to read each schema.
    /// Schemas are retrieved ordered by name.
	//
    /// Parameters:
    /// 	owner: retrieve schemas for this datastore.
    ///      dsInfo: If true, retrieve only the special row that describes
    ///          the datastore.
    ///          Otherwise, retrieve all feature schemas for the datastore.
	FdoSmPhSchemaReader(FdoSmPhOwnerP owner, bool dsInfo = false);

    /// Deactivate the query
	~FdoSmPhSchemaReader(void);

    /// Get the name of the current schema.
	FdoStringP GetName();

    /// Get the description of the current schema.
	FdoStringP GetDescription();

    /// Gets the MetaSchema version (applicable only when dsInfo = true).
    double GetVersion();

    /// Get the default Oracle database of the current schema.
	FdoStringP GetDatabase();

    /// Get the default Oracle schema of the current schema.
	FdoStringP GetOwner();

    /// Get the default table mapping of the current schema.
	FdoStringP GetTableMapping();

    /// Get the default table storage of the current schema.
	FdoStringP GetTableStorage();

    /// Get the default index storage of the current schema.
	FdoStringP GetIndexStorage();

    /// Get the default text storage of the current schema.
	FdoStringP GetTextStorage();

    /// Get the default text-in-row option of the current schema.
	FdoStringP GetTextInRow();

    /// Get the default table storage engine of the current schema.
	FdoStringP GetTableStorageEngine();

    /// Get the owning user of the current schema.
	FdoStringP GetUser();

    /// Get the next row and make it current.
	virtual bool ReadNext();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSchemaReader() {}

    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoSmPhOwnerP owner, bool dsInfo );

    /// Create inner reader for MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhRowsP pTables, FdoSmPhOwnerP owner, bool dsInfo );

    /// Create inner reader for native physical schema
    FdoSmPhReaderP MakeRdReader( FdoSmPhRowsP pTables, FdoSmPhOwnerP owner, bool dsInfo );

private:
	bool mbSchemaOptionsTableDefined;
	FdoPtr<FdoSmPhSOReader> mpSOReader;
	FdoPtr<FdoSmPhSchemaSOReader> mpSchemaSOReader;
    bool mbReadFromMetadata; // indicates whether or not we are reading from metadata tables
};

typedef FdoPtr<FdoSmPhSchemaReader> FdoSmPhSchemaReaderP;

#endif


