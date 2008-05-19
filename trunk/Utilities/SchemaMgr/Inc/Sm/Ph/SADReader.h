#ifndef FDOSMPHSADREADER_H
#define FDOSMPHSADREADER_H		1
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
#include <Sm/Ph/ISADReader.h>

// Reads rows from the F_SAD ( Schema Attribute Dictionary ) table.

class FdoSmPhSADReader :
	public FdoSmPhReader,
    public FdoSmPhISADReader
{
public:
    /// Create and execute a query to list the SAD rows
    /// Call ReadNext() to read each SAD.
	//
    /// Parameters:
    /// 	elementType: one of 
    /// 		FdoSmPhysicalSchema::SchemaType - read SAD rows for schema(s).
    /// 		FdoSmPhysicalSchema::ClassType - read SAD rows for class(s).
    /// 		FdoSmPhysicalSchema::PropertyType - read SAD rows for property(s).
    /// 		L"" - read SAD rows for any element type.
    /// 	mgr - physical schema manager.
    /// 	schemaName: read only rows for this schema. 
    /// 		if L"" then read rows for any schema.
    /// 	className: read only rows for this class. Ignored when elementType is SchemaType.
    /// 		if L"" then read rows for any class.
    /// 	propertyName: read only rows for this property. Ignored when elementType is SchemaType
    /// 		or Classtype.
    /// 		if L"" then read rows for any property.
	FdoSmPhSADReader( 
		FdoStringP elementType, 
		FdoSmPhMgrP mgr,
		FdoStringP schemaName = L"",
		FdoStringP className = L"",
		FdoStringP propertyName = L"" 
	);

    /// Deactivate the query
	~FdoSmPhSADReader(void);

    virtual bool ReadNext();

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

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSADReader() {}

private:
    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( 
		FdoStringP elementType, 
		FdoSmPhMgrP mgr,
		FdoStringP schemaName = L"",
		FdoStringP className = L"",
		FdoStringP propertyName = L"" 
	);

    FdoStringP MakeClauses( 
		    FdoStringP elementType, 
		    FdoStringP schemaName,
		    FdoStringP className,
		    FdoStringP propertyName
    );
};

typedef FdoPtr<FdoSmPhSADReader> FdoSmPhSADReaderP;

#endif


