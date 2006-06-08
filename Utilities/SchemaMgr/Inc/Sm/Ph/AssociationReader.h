#ifndef FDOSMPHASSOCIATIONREADER_H
#define FDOSMPHASSOCIATIONREADER_H		1
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
#include <Sm/Ph/ColumnList.h>

// This class retrieves all the association definitions from the 
// current connection
class FdoSmPhAssociationReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to list the association definition
    /// for a property.
    /// Call ReadNext() to read each row.
	//
    /// Parameters:
    /// 	classId: id of the property's class.
    /// 	fkTableName: the associate dclass table.
	FdoSmPhAssociationReader(long classId, FdoStringP fkTableName, FdoSmPhMgrP mgr);

    /// Create and execute a query to retrieve the association definition for the given tables.
    /// The query depends on the parameters below.
    /// 
    /// When pkTableName and fkTableName are both L"" then all definitions are
    /// retrieved.
	//
    /// When only fkTableName is L"" then all definitions referenced by 
    /// pkTableName are selected.
	//
    /// When only pkTableName is L"" then all definitions referencing 
    /// fkTableName are selected.
	//
    /// When neither is L"" then the select depends on bAnd:
    /// 	- if true then definitions that match the pkTableName and the
    /// 	fkTableName are selected.
    /// 	- if false then definitions that match the pkTableName or the
    /// 	fkTableName are selected.
	//
    /// Parameters:
    /// 	pkTableName: primary key table name
    /// 	fkTableName: foreign key table name.
    /// 	bAnd: 
    /// 		True: select definitions for the given pkTable and fkTable.
    /// 		False: select definitions for the given pkTable or fkTable.
    /// 	mgr: the current physical schema manager.
	FdoSmPhAssociationReader(FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP mgr);

    /// Deactivate the query
	~FdoSmPhAssociationReader(void);

    /// Various getters for dependency attributes

    /// Primary Key table name. This is also the class
    /// table for the property's class.
	FdoStringP GetPkTableName();

    /// Primary key columns. Columns in the Primary Key table
    /// that relate to the Foreign Key table.
	FdoSmPhColumnListP GetPkColumnNames();

    /// Primary Key table name. This is also the table
    /// of the associated class.
	FdoStringP GetFkTableName();

    /// Foreign key columns. Columns in the Foreign Key table
    /// that relate to the Primary Key table.
	FdoSmPhColumnListP GetFkColumnNames();

    /// The multiplicity column.
	FdoStringP GetMultiplicity();

    /// The reverse multiplicity column.
	FdoStringP GetReverseMultiplicity();

    /// The reverse name column.
	FdoStringP GetReverseName();

    FdoStringP GetPseudoColumnName();

    /// The cascade lock column
	//
    /// Returns:
    /// true or false
	bool GetCascadelock();

    /// The delete rule:
    /// Returns:
    /// An FdoDeleteRule database representation(1=FdoDeleteRule_Cascade, 2=FdoDeleteRule_Prevent, 3=FdoDeleteRule_Break)
	int GetDeleteRule();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhAssociationReader() {}

    /// Helpers for the inner reader.

    /// Top level inner reader creation function
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr, long classId, FdoStringP fkTableName );
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );
    
    /// Create reader to read from MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName );
    FdoSmPhReaderP MakeMtReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );
    
    /// Create reader to read directly from physical schema. Called when 
    /// there is no MetaSchema present.
    FdoSmPhReaderP MakeRdReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, long classId, FdoStringP fkTableName );
    FdoSmPhReaderP MakeRdReader( FdoSmPhMgrP mgr, FdoSmPhRowsP rows, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd );
    
    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per association attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr, bool bAddClassDef = false );
};

typedef FdoPtr<FdoSmPhAssociationReader> FdoSmPhAssociationReaderP;

#endif


