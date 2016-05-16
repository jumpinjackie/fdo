#ifndef FDOSMPHSOWRITER_H
#define FDOSMPHSOWRITER_H		1
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

#include <Sm/Ph/Row.h>
#include <Sm/Ph/CommandWriter.h>

// This class writes Schema Options entries 
// to the f_schemaoptions table.

class FdoSmPhSOWriter :
	public FdoSmPhWriter
{
public:
    /// Create a Schema Options writer.
	//
    /// Parameters:
    /// 	mgr: Physical Schema Manager
	FdoSmPhSOWriter( 
		FdoSmPhMgrP mgr
	);

	~FdoSmPhSOWriter(void);

    /// Various field value getters

    /// Owner name ( Owner of the element )
    /// \li \c The schema name when elementType is FdoSmPhysicalSchema::SchemaType.
    /// \li \c A schema name when elementType is FdoSmPhysicalSchema::ClassType.
    /// \li \c The [schemaName:className] when elementType is FdoSmPhysicalSchema::PropertyType.
	FdoStringP GetOwnerName();

    /// Name of the element that owns the current Schema Option row.
	FdoStringP GetElementName();

    /// Type of the element that owns the current Schema Option row.
    /// \li \c FdoSmPhysicalSchema::SchemaType - when the element is a feature schema.
    /// \li \c FdoSmPhysicalSchema::ClassType - when the element is a class.
    /// \li \c FdoSmPhysicalSchema::PropertyType - when the element is a property.
	FdoStringP GetElementType();

    /// Name of the current Schema Option row
	virtual FdoStringP GetName();

    /// Value for the current Schema Option row
	virtual FdoStringP GetValue();

    /// Corresponding setters.

    /// Owner name ( Owner of the element )
    /// \li \c The schema name when elementType is FdoSmPhysicalSchema::SchemaType.
    /// \li \c A schema name when elementType is FdoSmPhysicalSchema::ClassType.
    /// \li \c The [schemaName:className] when elementType is FdoSmPhysicalSchema::PropertyType.
	void SetOwnerName(FdoStringP sValue);

    /// Name of the element that owns the current Schema Option row.
	void SetElementName(FdoStringP sValue);

    /// Type of the element that owns the current Schema Option row.
    /// \li \c FdoSmPhysicalSchema::SchemaType - when the element is a feature schema/.
    /// \li \c FdoSmPhysicalSchema::ClassType - when the element is a class.
    /// \li \c FdoSmPhysicalSchema::PropertyType - when the element is a property.
	void SetElementType(FdoStringP sValue);

    /// Name of the current Schema Option row
	virtual void SetName(FdoStringP sValue);

    /// Value for the current Schema Option row
	virtual void SetValue(FdoStringP sValue);

    /// Add the current row as a new Schema Option entry
	virtual void Add();

    /// Delete a row 
    //
    /// Parameters
    ///      sOwner, sName: primary key of the row to delete
    virtual void Delete( FdoStringP sOwner, FdoStringP sName );

    /// Get a field list for reading from or writing to 
    /// f_schemaoptions.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the current datastore to query.
    static FdoSmPhRowP MakeRow( FdoSmPhOwnerP owner );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSOWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhSOWriter> FdoSmPhSOWriterP;
typedef std::map<FdoStringP, FdoStringP> SchemaOptionsMap;
typedef std::map<FdoStringP, FdoStringP>::iterator SchemaOptionsMapIter;


class FdoSmPhElementSOWriter : public FdoSmPhWriter
{
public:
    /// SchemaOptions entry value for given name
	virtual FdoStringP GetOption(FdoString* optionName);
	virtual void SetOption(FdoStringP optionName, FdoStringP optionValue);

    /// Writer methods:
	void Add(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType);
	void Modify(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType);
	void Delete(FdoStringP ownerName, FdoStringP elementName, FdoStringP elementType);

	FdoSmPhWriterP MakeWriter(FdoSmPhMgrP mgr);

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhElementSOWriter() {}

    /// Create and execute a query to read the SchemaOptions rows for a given schema element
    /// Call ReadNext() to read each row.
    /// SchemaOptions rows are ordered by name
	//
    /// Parameters:
    /// 	schemaName: the schema to write options for
	FdoSmPhElementSOWriter(FdoSmPhMgrP mgr);

    /// Deactivate the query
	~FdoSmPhElementSOWriter(void);

private:
	FdoSmPhSOWriterP mpSOWriter;
	SchemaOptionsMap mOptionsMap;
};

#endif


