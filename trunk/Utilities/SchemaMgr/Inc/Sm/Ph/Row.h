#ifndef FDOSMPHROW_H
#define FDOSMPHROW_H		1
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

#include <Sm/Ph/ColumnGeom.h>       // For FdoSmPhScInfoP
#include <Sm/Ph/FieldCollection.h>
#include <Sm/Ph/Mgr.h>

// Row represents a set of fields for a particular database object. 
// It represents a table in a from clause of a select statement
// or the into clause of an insert statement or a table in an
// update statement
class FdoSmPhRow : public FdoSmPhSchemaElement
{
public:
    /// Constructs an instance of a Row object.
    /// 
    /// Parameters:
    /// 	mgr: Physical Schema Manager
    /// 	rowName: the underlying database object name.
    ///      dbObject: the underlying database object.
    ///          if NULL then a temporary database object is 
    ///          automatically created.
    FdoSmPhRow(
        FdoSmPhMgrP mgr, 
        FdoStringP rowName,
        FdoSmPhDbObjectP dbObject = (FdoSmPhDbObject*) NULL
    );

	~FdoSmPhRow(void);

    /// Returns all the fields in this Row.
    const FdoSmPhFieldCollection* RefFields() const;
    FdoSmPhFieldsP GetFields();

    /// Returns the database object represented by this Row.
    FdoSmPhDbObjectP GetDbObject();

    // Clears the values of all fields for this row.
    void Clear();

    /// Various functions for creating columns.
    /// These functions are used to create field columns
    /// for columns that do not actually reside in the database
    /// (computed columns).

    FdoSmPhColumnP CreateColumnBLOB(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
    );

    FdoSmPhColumnP CreateColumnChar(
		FdoStringP columnName, 
		bool bNullable, 
        int length,
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnDate(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
    );

    FdoSmPhColumnP CreateColumnDecimal(
		FdoStringP columnName, 
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnSingle(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
    );

    FdoSmPhColumnP CreateColumnDouble(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
    );

    FdoSmPhColumnP CreateColumnGeom(
		FdoStringP columnName, 
        FdoSmPhScInfoP AssociatedSCInfo,        
        bool bNullable = true, 
        bool bHasElevation = true, 
        bool bHasMeasure = false, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnBool(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnByte(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnInt16(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnInt32(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnInt64(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnUnknown(
		FdoStringP columnName, 
        FdoStringP typeName,
		bool bNullable, 
        int length,
        int scale,
        FdoStringP rootColumnName = L""
	);

    FdoSmPhColumnP CreateColumnDbObject(
		FdoStringP columnName, 
		bool bNullable, 
        FdoStringP rootColumnName = L""
	);

protected:
    /// unused constructor only for building on Linux
    FdoSmPhRow() {}

    // Find a column in this row. Does case sensitive match
    // first, and then tries match to column whose name is 
    // of the default case, if the case sensitive match fails.
    FdoSmPhColumnP FindColumn( FdoStringP columnName );

private:
    FdoSmPhDbObjectP mDbObject;

	FdoSmPhFieldsP mFields;
};

typedef FdoPtr<FdoSmPhRow> FdoSmPhRowP;

#endif


