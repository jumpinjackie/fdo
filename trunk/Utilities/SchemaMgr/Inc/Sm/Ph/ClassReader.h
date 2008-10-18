#ifndef FDOSMPHCLASSREADER_H
#define FDOSMPHCLASSREADER_H		1
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

#include <Sm/Ph/Reader.h>

#include <Sm/Ph/ClassPropertyReader.h>
/*
#include <Sm/Ph/IndexPropertyReader.h>
*/
#include <Sm/Ph/ClassSADReader.h>
#include <Sm/Ph/SOReader.h>
#include <Sm/Ph/ClassSOReader.h>


// Retrieves all classes for a given Feature schema. 

class FdoSmPhClassReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to get the classes.
    /// Call ReadNext() to read each class.
    /// Classes are ordered by class name .
	//
    /// Parameters:
    /// 	schemaName: get the classes for this schema.
    /// 	mgr: holds the connection for the database
    /// 		to retrieve the classes from.
	FdoSmPhClassReader(FdoStringP schemaName, FdoSmPhMgrP mgr);

    /// Create and execute a query to get the classes.
    /// Call ReadNext() to read each class.
    /// Classes are ordered by class name .
	//
    /// Parameters:
    /// 	schemaName: get the classes for this schema.
    /// 	mgr: holds the connection for the database
    /// 		to retrieve the classes from.
	FdoSmPhClassReader(FdoStringP schemaName, FdoStringP className, FdoSmPhMgrP mgr);

    /// Deactivate the class reader
	~FdoSmPhClassReader(void);

    /// Various getters

    /// Class ID - internal to Provider
	FdoInt64 GetId();

    /// Class name
	FdoStringP GetName();

    /// Base Class name
	FdoStringP GetBaseName();

    /// Class description
	FdoStringP GetDescription();

    /// Class table name (actually a view when table is foreign).
	FdoStringP GetTableName();

    /// Root table name (the table name when table is foreign).
    /// L"" when table not foreign.
	FdoStringP GetRootTableName();

    /// Get the table mapping.
    /// This is only set of table mapping was explicitly 
    /// specified by Schema Overrides.
	FdoStringP GetTableMapping();

    /// Get the table storage (corresponds to tablespace in Oracle, table filegroup in SQL Server, etc)
	FdoStringP GetTableStorage();

    /// Class Type ( Class, Feature, Tin, etc. )
	FdoStringP GetClassType();

    /// true if the class is abstract
	bool GetIsAbstract();

    /// Class table name.
	FdoStringP GetDatabase();
	FdoStringP GetOwner();

    /// Main geometry for feature class
    /// if L"" then there is no main geometry
	FdoStringP GetGeometryProperty();

    /// true if class table has fixed name specified through 
    /// schema overrides
	bool GetIsFixedTable();

    /// true if this class owns the class table (table was created when class was created)
    /// false if this class attached itself to pre-existing table when it was created. 
	bool GetIsTableCreator();

    /// Schema Option
    FdoStringP GetOption( FdoStringP optionName );

    /// Gets a reader for retrieving the class properties
	FdoSmPhClassPropertyReaderP CreateClassPropertyReader();

    /// Gets a reader for retrieving the class properties
//TODO	FdoSmPhClassIndexReaderP GetClassIndexReader();

    /// Gets a reader for retrieving the Schema Attribute Dictionary
	FdoSmPhClassSADReaderP GetClassSADReader();
   
    /// Get the next row and make it current.
	virtual bool ReadNext();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhClassReader() {}

    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className = NULL );

    /// Top level function for creating inner reader
    //FdoSmPhReaderP MakeReader( FdoStringP schemaName, FdoStringP className, FdoSmPhMgrP mgr );

    /// Create inner reader for MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className = NULL );

    /// Create inner reader for Physical Schema (When MetaSchema not present).
    FdoSmPhReaderP MakeRdReader( FdoSmPhRowsP rows, FdoStringP schemaName, FdoSmPhMgrP mgr, FdoString* className = NULL );

private:

    /// Current property reader
    FdoSmPhPropertyReaderP	mPropReader;
    FdoSmPhSADReaderP mClassSADReader;
    FdoSmPhSADReaderP mPropSADReader;
    /// Schema from where classes are read.
	FdoStringP mSchemaName;

    bool mbTableCreatorDefined;
    bool mbReadFromMetadata; // indicates whether or not we are reading from metadata tables

    /// Schema Options reader
	bool mbSchemaOptionsTableDefined;
	FdoPtr<FdoSmPhSOReader> mpSOReader;
	FdoPtr<FdoSmPhClassSOReader> mpClassSOReader;
};

typedef FdoPtr<FdoSmPhClassReader> FdoSmPhClassReaderP;

#endif


