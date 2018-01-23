#ifndef FDOSMPHSCHEMAWRITER_H
#define FDOSMPHSCHEMAWRITER_H		1
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

#include <Sm/Ph/CommandWriter.h>
#include <Sm/Ph/SOWriter.h>
#include <Sm/Ph/SchemaSOWriter.h>


// This class writes Feature Schemas 
// to the f_schemainfo table.

class FdoSmPhSchemaWriter : public FdoSmPhWriter
{
public:

    /// Create a Feature Schema writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore to query.
	FdoSmPhSchemaWriter(FdoSmPhMgrP mgr);

	~FdoSmPhSchemaWriter(void);

    /// Various field value getters

    /// Get the name of the current schema.
	FdoStringP GetName();

    /// Get the description of the current schema.
	FdoStringP GetDescription();

    /// Get the default database of the current schema.
	FdoStringP GetDatabase();

    /// Get the default owner of the current schema.
	FdoStringP GetOwner();

    /// Get the default table storage of the current schema.
	FdoStringP GetTableStorage();

    /// Get the default table mapping of the current schema.
	FdoStringP GetTableMapping();

    /// Get the default index storage of the current schema
	FdoStringP GetIndexStorage();

    /// Get the default text storage of the current schema
	FdoStringP GetTextStorage();

    /// Get the default text-in-row option of the current schema
	FdoStringP GetTextInRow();

    /// Get the default table storage engine of the current schema
	FdoStringP GetTableStorageEngine();

    /// Get the description of the current schema.
	FdoStringP GetUser();

    /// Corresponding field value setters

	void SetName(FdoStringP sValue);

	void SetDescription(FdoStringP sValue);

	void SetDatabase(FdoStringP sValue);

	void SetOwner(FdoStringP sValue);

	void SetTableStorage(FdoStringP sValue);

	void SetTableMapping(FdoStringP sValue);

	void SetIndexStorage(FdoStringP sValue);

	void SetTextStorage(FdoStringP sValue);

	void SetTextInRow(FdoStringP sValue);

	void SetTableStorageEngine(FdoStringP sValue);

	void SetUser(FdoStringP sValue);

    /// Add the current row as a new Feature Schema
	virtual void Add();

    /// Modify a row using the current field values
    //
    /// Parameters
    ///      sName: primary key (schema name) of the row to modify
    virtual void Modify( FdoStringP sName );

    virtual void Delete( FdoStringP sName );

    /// Get a field list for reading from or writing to 
    /// f_schemainfo.
	//
    /// Parameters:
    /// 	mgr: Physical Schema Manager
    static FdoSmPhRowP MakeRow( FdoSmPhOwnerP owner );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSchemaWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );

private:
	bool mbSchemaOptionsTableDefined;
	FdoSmPhSchemaSOWriterP mpSchemaSOWriter;
};

typedef FdoPtr<FdoSmPhSchemaWriter> FdoSmPhSchemaWriterP;

#endif


