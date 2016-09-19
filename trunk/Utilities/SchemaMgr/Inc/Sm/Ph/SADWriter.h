#ifndef FDOSMPHSADWRITER_H
#define FDOSMPHSADWRITER_H		1
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

// This class writes Schema Attribute Dictionary entries 
// to the f_sad table.

class FdoSmPhSADWriter :
	public FdoSmPhWriter
{
public:
    /// Create an Schema Attribute Dictionary writer.
	//
    /// Parameters:
    /// 	mgr: Physical Schema Manager
	FdoSmPhSADWriter( 
		FdoSmPhMgrP mgr
	);

	~FdoSmPhSADWriter(void);

    /// Various field value getters

    /// Owner name ( Owner of the element )
    /// \li \c The schema name when elementType is FdoSmPhysicalSchema::SchemaType.
    /// \li \c A schema name when elementType is FdoSmPhysicalSchema::ClassType.
    /// \li \c The [schemaName:className] when elementType is FdoSmPhysicalSchema::PropertyType.
	FdoStringP GetOwnerName();

    /// Name of the element that owns the current SAD row.
	FdoStringP GetElementName();

    /// Type of the element that owns the current SAD row.
    /// 	FdoSmPhysicalSchema::SchemaType - when the element is a feature schema
    /// 	FdoSmPhysicalSchema::ClassType - when the element is a class
    /// 	FdoSmPhysicalSchema::PropertyType - when the element is a property
	FdoStringP GetElementType();

    /// Name of the current SAD row
	virtual FdoStringP GetName();

    /// Value for the current SAD row
	virtual FdoStringP GetValue();

    /// Corresponding setters.

    /// Owner name ( Owner of the element )
    /// \li \c The schema name when elementType is FdoSmPhysicalSchema::SchemaType.
    /// \li \c A schema name when elementType is FdoSmPhysicalSchema::ClassType.
    /// \li \c The [schemaName:className] when elementType is FdoSmPhysicalSchema::PropertyType.
	void SetOwnerName(FdoStringP sValue);

    /// Name of the element that owns the current SAD row.
	void SetElementName(FdoStringP sValue);

    /// Type of the element that owns the current SAD row.
    /// \li \c FdoSmPhysicalSchema::SchemaType - when the element is a feature schema.
    /// \li \c FdoSmPhysicalSchema::ClassType - when the element is a class.
    /// \li \c FdoSmPhysicalSchema::PropertyType - when the element is a property.
	void SetElementType(FdoStringP sValue);

    /// Name of the current SAD row
	virtual void SetName(FdoStringP sValue);

    /// Value for the current SAD row
	virtual void SetValue(FdoStringP sValue);

    /// Add the current row as a new SAD entry
	virtual void Add();

    /// Delete a row 
    //
    /// Parameters
    ///      sOwner, sName: primary key of the row to delete
    virtual void Delete( FdoStringP sOwner, FdoStringP sName );

    /// Get a field list for reading from or writing to 
    /// f_sad.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the current datastore to query.
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSADWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhSADWriter> FdoSmPhSADWriterP;

#endif


