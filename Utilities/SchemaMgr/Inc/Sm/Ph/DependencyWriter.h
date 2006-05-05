#ifndef FDOSMPHDEPENDENCYWRITER_H
#define FDOSMPHDEPENDENCYWRITER_H		1
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

#include <Sm/Ph/Writer.h>
#include <Sm/Ph/Dependency.h>

// This class writes attribute dependencies 
// to the f_attributedependencies table.

class FdoSmPhDependencyWriter :
	public FdoSmPhWriter
{
public:
    /// Create an attribute dependency writer.
	//
    /// Parameters:
    /// 	mgr: physical schema manager.
	FdoSmPhDependencyWriter(FdoSmPhMgrP mgr);

	~FdoSmPhDependencyWriter(void);

    /// Various getters for dependency attributes

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

    /// Corresponding setter functions

    /// Primary Key table name. This is also the class
    /// table for the property's class.
	void SetPkTableName(FdoStringP sValue);

    /// Primary key columns. Columns in the Primary Key table
    /// that relate to the Foreign Key table.
	void SetPkColumnNames(FdoStringsP sValues);

    /// Primary Key table name. This is also the table
    /// where the property is stored.
	void SetFkTableName(FdoStringP sValue);

    /// Foreign key columns. Columns in the Foreign Key table
    /// that relate to the Primary Key table.
	void SetFkColumnNames(FdoStringsP sValues);

    /// Identity column from the Foreign Key table. Specified only when the 
    /// property is a collection property.
	void SetIdentityColumn(FdoStringP sValue);

    /// Order type for rows in the Foreign Key table.
	void SetOrderType(FdoStringP sValue);

    /// Cardinality:
    /// 1 - the property is a value property.
    ///  otherwise, the property is a collection property.
	void SetCardinality(long lValue);

    /// Modify a row with the current field values
    //
    /// Parameters
    ///      pkTableName, fkTableName: primary key of the row to modify
    virtual void Modify( FdoStringP pkTableName, FdoStringP fkTableName );

    /// Delete a row 
    //
    /// Parameters
    ///      pkTableName, fkTableName: primary key of the row to delete
    virtual void Delete( FdoStringP pkTableName, FdoStringP fkTableName );

    /// Get a field list for reading from or writing to 
    /// f_attributedependencies.
	//
    /// Parameters:
    /// 	mgr: physical schema manager.
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhDependencyWriter() {}

    /// Create the inner command writer.
    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );    
};

typedef FdoPtr<FdoSmPhDependencyWriter> FdoSmPhDependencyWriterP;

#endif


