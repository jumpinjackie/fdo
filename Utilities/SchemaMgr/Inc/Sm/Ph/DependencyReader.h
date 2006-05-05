#ifndef FDOSMPHDEPENDENCYREADER_H
#define FDOSMPHDEPENDENCYREADER_H		1
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
#include <Sm/Ph/Dependency.h>

// This class retrieves all attribute dependencies for a
// given property ( usually an object property ).
// The attribute dependencies come from f_attributedependencies.
// They describe how the property's table and the class table
// for the property are joined.

class FdoSmPhDependencyReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to list the dependencies
    /// for a property.
    /// Call ReadNext() to read each dependency.
	//
    /// Parameters:
    /// 	classId: id of the property's class.
    /// 	fkTableName: the property's table.
    /// 	mgr: physical schema manager.
	FdoSmPhDependencyReader(long classId, FdoStringP fkTableName, FdoSmPhMgrP mgr);

    /// Create and execute a query to dependencies for the given tables.
    /// The query depends on the parameters below.
    /// 
    /// When pkTableName and fkTableName are both L"" then all dependencies are
    /// retrieved.
	//
    /// When only fkTableName is L"" then all dependencies referenced by 
    /// pkTableName are selected.
	//
    /// When only pkTableName is L"" then all dependencies referencing 
    /// fkTableName are selected.
	//
    /// When neither is L"" then the select depends on bAnd:
    /// 	- if true then dependencies that match the pkTableName and the
    /// 	fkTableName are selected.
    /// 	- if falsethen dependencies that match the pkTableName or the
    /// 	fkTableName are selected.
	//
    /// Parameters:
    /// 	pkTableName: primary key table name
    /// 	fkTableName: foreign key table name.
    /// 	bAnd: 
    /// 		True: select dependencies for the given pkTable and fkTable.
    /// 		False: select dependencies for the given pkTable or fkTable.
    /// 	mgr: physical schema manager.
	FdoSmPhDependencyReader(FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP mgr);

    /// Deactivate the query
	~FdoSmPhDependencyReader(void);

    /// Various getters for dependency attributes

    /// Get a complete dependency object
	FdoSmPhDependencyP GetDependency( FdoSmPhSchemaElement* pParent );

    /// Primary Key table name. This is also the class
    /// table for the property's class.
	FdoStringP GetPkTableName();

    /// Primary key columns. Columns in the Primary Key table
    /// that relate to the Foreign Key table.
	FdoStringsP GetPkColumnNames();

    /// Primary Key table name. This is also the table
    /// where the property is stored.
	FdoStringP GetFkTableName();

    /// Foreign key columns. Columns in the Foreign Key table
    /// that relate to the Primary Key table.
	FdoStringsP GetFkColumnNames();

    /// Identity column from the Foreign Key table. Specified only when the 
    /// property is a collection property.
	FdoStringP GetIdentityColumn();

    /// Order type for rows in the  Foreign Key table, applies only if 
    /// cardinality is not 1.
	//
    /// Returns:
    /// "a" - ascending
    ///  "d" - descending
    /// "" - unordered.
	FdoStringP GetOrderType();

    /// Cardinality:
    /// 1 - the property is a value property.
    ///  otherwise, the property is a collection property.
	long GetCardinality();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhDependencyReader() {}

private:
    /// Create the inner query reader for retrieving the dependencies
    FdoSmPhReaderP MakeReader( FdoStringP where, FdoSmPhMgrP mgr, bool bAddClassDef = false );
    /// Make where clause for retrieving by property classid and property table
    FdoStringP MakeClauses( long classId, FdoStringP fkTableName );
    /// Make where clause for retrieving by primary and foreign table
    FdoStringP MakeClauses( FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );
};

typedef FdoPtr<FdoSmPhDependencyReader> FdoSmPhDependencyReaderP;

#endif


