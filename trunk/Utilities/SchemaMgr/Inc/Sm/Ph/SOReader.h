#ifndef FDOSMPHSOREADER_H
#define FDOSMPHSOREADER_H		1
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
#include <Sm/Ph/GroupReader.h>



// Reads rows from the F_SCHEMAOPTIONS table.

class FdoSmPhSOReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to list the Schema Options rows
    /// Call ReadNext() to read each Schema Option.
	//
    /// Parameters:
    /// 	elementType: one of 
    /// 		FdoSmPhysicalSchema::SchemaType - read Schema Option rows for schema(s).
    /// 		FdoSmPhysicalSchema::ClassType - read Schema Option rows for class(s).
    /// 		FdoSmPhysicalSchema::PropertyType - read Schema Option rows for property(s).
    /// 		L"" - read Schema Option rows for any element type.
    /// 	mgr - physical schema manager.
    /// 	schemaName: read only rows for this schema. 
    /// 		if L"" then read rows for any schema.
    /// 	className: read only rows for this class. Ignored when elementType is SchemaType.
    /// 		if L"" then read rows for any class.
    /// 	propertyName: read only rows for this property. Ignored when elementType is SchemaType
    /// 		or Classtype.
    /// 		if L"" then read rows for any property.
	FdoSmPhSOReader( 
		FdoStringP elementType, 
		FdoSmPhOwnerP owner,
		FdoStringP schemaName = L"",
		FdoStringP className = L"",
		FdoStringP propertyName = L"" 
	);

    /// Deactivate the query
	virtual ~FdoSmPhSOReader(void);

    virtual bool ReadNext();

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

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSOReader() {}

private:
    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( 
		FdoStringP elementType, 
		FdoSmPhOwnerP owner,
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

typedef FdoPtr<FdoSmPhSOReader> FdoSmPhSOReaderP;
typedef std::map<FdoStringP, FdoStringP> SchemaOptionsMap;
typedef std::map<FdoStringP, FdoStringP>::iterator SchemaOptionsMapIter;


class FdoSmPhElementSOReader :
	public FdoSmPhGroupReader
{
public:
    /// Get the next SchemaOptions row and make it current.
	virtual bool ReadNext();

    /// SchemaOptions entry value for given name
	virtual FdoStringP GetOption(FdoString* optionName);

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhElementSOReader() {}

    /// Create and execute a query to read the SchemaOptions rows for a given schema element
    /// Call ReadNext() to read the *singleton* row.
    /// Parameters:
    /// 	groupName: the schema element name to read options for (schema name, or schema:class name, etc)
    /// 	pSOReader: get the rows from this SchemaOptions reader. This reader must
    /// 		return the rows ordered by schema, class and name.
    /// 		Caller is responsible for ensuring that this reader has not already
    /// 		passed the given class. If it has, then no rows are read.
	FdoSmPhElementSOReader(FdoStringP groupName, FdoSmPhSOReaderP pSOReader);
	virtual ~FdoSmPhElementSOReader();
	FdoSmPhSOReaderP mpSOReader;

private:
	SchemaOptionsMap mOptionsMap;
	bool mbBeforeFirstReadNext;
};

#endif


