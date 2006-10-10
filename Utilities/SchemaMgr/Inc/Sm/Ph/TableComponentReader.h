#ifndef FDOSMPHTABLECOMPONENTREADER_H
#define FDOSMPHTABLECOMPONENTREADER_H		1
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

#include <Sm/Ph/GroupReader.h>
#include <Sm/Ph/DependencyReader.h>
#include <Sm/Ph/Rd/IndexReader.h>
#include <Sm/Ph/Rd/ColumnReader.h>

// This class is used to read components (columns, indexes, constraints, keys ) for a particular
// table. It wraps around a component reader for a set of tables, and extracts the rows for a
// particular table. 
class FdoSmPhTableComponentReader : public FdoSmPhGroupReader
{
public:
    // Attaches to a query reader. 
    // Call ReadNext() to read each row.
	//
    // Parameters:
    // 	tableName: retrieve the rows for this table.
    //  tableRowName: row, in underlying reader, where table name field is kept.
    //  tableFieldName: field, in underlyingreader, specifying the table name for the 
    //      current row. (i.e. the table that owns the current table component being read).
    // 	pReader: the underlying reader to attach to. The reader must return
    // 		rows in ascending order by table name. Note that if this
    // 		reader is currently positioned after the table to retrieve
    // 		then this reader returns no rows.
	FdoSmPhTableComponentReader(
        FdoStringP tableName, 
        FdoStringP tableRowName, 
        FdoStringP tableFieldName, 
        FdoSmPhReaderP reader
    );

	virtual ~FdoSmPhTableComponentReader(void);

    // This function determines the table (group) name for the attached reader's
    // current row. 
	virtual FdoStringP GetGroupName();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableComponentReader() {}

private:
	FdoStringP mTableRowName;
	FdoStringP mTableFieldName;
};

typedef FdoPtr<FdoSmPhTableComponentReader> FdoSmPhTableComponentReaderP;

// Specialized table component reader for indexes
class FdoSmPhTableIndexReader : public FdoSmPhTableComponentReader
{
public:
	FdoSmPhTableIndexReader(
        FdoStringP tableFieldName, 
        FdoPtr<FdoSmPhRdIndexReader> reader
    );

	virtual ~FdoSmPhTableIndexReader(void);

    // Returns the type of the current index (Scalar or Spatial)
    virtual FdoSmPhIndexType GetIndexType();

protected:
    // Unused constructor needed only to build on Linux
    FdoSmPhTableIndexReader() {}

private:
	FdoPtr<FdoSmPhRdIndexReader> mIndexReader;
};

typedef FdoPtr<FdoSmPhTableIndexReader> FdoSmPhTableIndexReaderP;

// Specialized table component reader for columns
class FdoSmPhTableColumnReader : public FdoSmPhTableComponentReader
{
public:
	FdoSmPhTableColumnReader(
        FdoStringP groupFieldName, 
        FdoPtr<FdoSmPhRdColumnReader> reader
    );

	virtual ~FdoSmPhTableColumnReader(void);

    /// Get the type of the current column
    virtual FdoSmPhColType GetType();

    FdoPtr<FdoSmPhRdColumnReader> GetColumnReader();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableColumnReader() {}

private:
	FdoPtr<FdoSmPhRdColumnReader> mColumnReader;
};

typedef FdoPtr<FdoSmPhTableColumnReader> FdoSmPhTableColumnReaderP;

// Specialized table component reader for attribute dependencies
class FdoSmPhTableDependencyReader : public FdoSmPhTableComponentReader
{
public:
	FdoSmPhTableDependencyReader(
        FdoStringP tableName, 
        FdoStringP rowName,
        FdoStringP fieldName,
        FdoPtr<FdoSmPhDependencyReader> reader
    );

	virtual ~FdoSmPhTableDependencyReader(void);

    FdoPtr<FdoSmPhDependencyReader> GetDependencyReader();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableDependencyReader() {}

private:
	FdoPtr<FdoSmPhDependencyReader> mDependencyReader;
};

typedef FdoPtr<FdoSmPhTableDependencyReader> FdoSmPhTableDependencyReaderP;

#endif


