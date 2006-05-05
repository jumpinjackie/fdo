#ifndef FDOSMPHDEPENDENCY_H
#define FDOSMPHDEPENDENCY_H		1
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

#include <Sm/Ph/SchemaElement.h>

// Represents a single row from the F_AttributeDepencencies table.

class FdoSmPhDependency :
	public FdoSmPhSchemaElement
{
public:

    /// 	pkTableName: name of table for property's class.
    /// 	fkTableName: the property's table.
	FdoSmPhDependency(
		FdoStringP pkTableName, 
		FdoStringsP pPkColumnNames,
		FdoStringP fkTableName, 
		FdoStringsP pFkColumnNames,
		FdoStringP identityColumn,
		FdoStringP orderType,
		long lCardinality,
		const FdoSmPhSchemaElement* pParent
	);

    /// Various getters for dependency attributes

    /// Primary Key table name. This is also the class
    /// table for the property's class.
	FdoStringP GetPkTableName() const;

    /// Primary key columns. Columns in the Primary Key table
    /// that relate to the Foreign Key table.
	FdoStringsP GetPkColumnNames() const;

    /// Primary Key table name. This is also the table
    /// where the property is stored.
	FdoStringP GetFkTableName() const;

    /// Foreign key columns. Columns in the Foreign Key table
    /// that relate to the Primary Key table.
	FdoStringsP GetFkColumnNames() const;

    /// Order column from the Foreign Key table.
    /// Specified only when the property
    /// is a collection property.
	FdoStringP GetIdentityColumn() const;

    /// Order type for rows in the  Foreign Key table, applies only if 
    /// cardinality is not 1.
	//
    /// Returns:
    /// "a" - ascending
    ///  "d" - descending
    /// "" - unordered.
	FdoStringP GetOrderType() const;

    /// Cardinality:
    /// 1 - the property is a value property.
    ///  otherwise, the property is a collection property.
	long GetCardinality() const;

protected:
    /// unused constructor only for building on Linux
    FdoSmPhDependency() {}

	~FdoSmPhDependency(void);

private:
	FdoStringP mPkTableName; 
	FdoStringsP mpPkColumnNames;
	FdoStringP mFkTableName;
	FdoStringsP mpFkColumnNames;
	FdoStringP mIdentityColumn;
	FdoStringP mOrderType;
	long mlCardinality;
};

typedef FdoPtr<FdoSmPhDependency> FdoSmPhDependencyP;

#endif


