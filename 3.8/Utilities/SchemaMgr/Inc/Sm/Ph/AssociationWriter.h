#ifndef FDOSMPHASSOCIATIONWRITER_H
#define FDOSMPHASSOCIATIONWRITER_H		1
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
#include <Sm/Ph/ColumnList.h>

// This class writes association definitions 
// to the f_associationdefinition table.
class FdoSmPhAssociationWriter :
	public FdoSmPhWriter
{
public:
    /// Create an association definition writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the database to query.
	FdoSmPhAssociationWriter(FdoSmPhMgrP mgr);

	~FdoSmPhAssociationWriter(void);

    /// Various getters for field values

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

    /// The reverse name column.
	FdoStringP GetPseudoColumnName();

    /// The cascade lock column
	//
    /// Returns:
    /// true or false
	bool GetCascadeLock();

    /// The delete rule:
    /// Returns:
    /// An FdoDeleteRule database representation(1=FdoDeleteRule_Cascade, 2=FdoDeleteRule_Prevent, 3=FdoDeleteRule_Break)
	int GetDeleteRule();

    /// Corresponding field setters.
	void SetPkTableName(FdoStringP sValue);

    /// Primary key columns. Columns in the Primary Key table
    /// that relate to the Foreign Key table.
	void SetPkColumnNames(FdoSmPhColumnListP sValues);

    /// Primary Key table name. This is also the table
    /// where the property is stored.
	void SetFkTableName(FdoStringP sValue);

    /// Foreign key columns. Columns in the Foreign Key table
    /// that relate to the Primary Key table.
	void SetFkColumnNames(FdoSmPhColumnListP sValues);

    /// The multiplicity column.
	void SetMultiplicity(FdoStringP sValue);

    /// The reverse multiplicity column.
	void SetReverseMultiplicity(FdoStringP sValue);

    /// The reverse name column.
	void SetReverseName(FdoStringP sValue);

    void SetPseudoColumnName(FdoStringP sValue);

    /// The cascade lock column
	//
    /// Returns:
    /// true or false
	void SetCascadeLock(bool bValue);

    /// The delete rule:
    /// Returns:
    /// An FdoDeleteRule database representation(1=FdoDeleteRule_Cascade, 2=FdoDeleteRule_Prevent, 3=FdoDeleteRule_Break)
	void SetDeleteRule(int iValue);

    /// Add the current row with the current field values
	virtual void Add();

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


    /// Get a field list for reading from or writing to f_associationdefinition.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the FDO database to query.
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhAssociationWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhAssociationWriter> FdoSmPhAssociationWriterP;

#endif


