#ifndef FDOSMPHRDPROPERTYREADER_H
#define FDOSMPHRDPROPERTYREADER_H		1
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/Table.h>

// This reader reads all of the properties for a class, when no MetaSchema
// information is present. It reads each column in the given database object and 
// automatically wraps a property around it.
//
// If the column contains geometry values then it is wrapped by a Geometric
// property. Otherwise, it is wrapped by a Data property. 
//
// Foreign key columns are skipped. Instead, this reader returns an 
// Association property for each foreign key for the database object.


class FdoSmPhRdPropertyReader : public FdoSmPhReader
{
public:
    /// Create and execute the property reader
	//
    /// Parameters:
	FdoSmPhRdPropertyReader(
        FdoSmPhDbObjectP dbObject,      // read properties for this object 
        FdoSmPhMgrP mgr             // Physical Schema Manager.
    );

    /// Deactivates the reader.
	~FdoSmPhRdPropertyReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();


protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdPropertyReader() {}

    /// Gets the identity columns for the current database object.
    const FdoSmPhColumnCollection* RefIdCols() const
    {
        return (FdoSmPhColumnCollection*) mIdCols;
    }

    bool IsOrdinate(FdoSmPhColumnP column);

private:
    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per primary key and primary key column 
    /// attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    /// Determines the identity columns of the current database object.
    /// If the object has a primary key then these identity columns are the 
    /// primary key columns.
    /// Otherwise the columns for the best unique index is chosen.
    /// The best unique index is the one with the fewest columns. If there is a tie,
    /// the index with the most compact columns is chosen.
    void ResolveIdentity();

    /// Get the 1-based position of the current property (column_ within the 
    /// identity column list. Returns 0 if the current property is not an
    /// identity property.
    FdoStringP GetIdPosn( FdoStringP colName );

    /// Returns an indicator of the compactness of an index's columns
    long IndexSize( FdoSmPhIndexP index );

    /// Returns true if the current column is a foreign key column.
    bool InFkey( FdoSmPhColumnP column );


    // Returns true if the given foreign key can be reverse-engineered 
    // into an association property. It can if all of its primary and 
    // foreign key columns can be reverse-engineered
    bool CheckFkey( FdoSmPhFkeyP fkey );

    // Returns true if the given column can be reverse-engineered into a
    // data or geometric property.
    bool CheckColumn( FdoSmPhColumnP column );

    FdoSmPhDbObjectP mDbObject;
    FdoSmPhTable* mpTable;
    FdoSmPhColumnsP mIdCols;
    int mFkeyCount;
    int mFieldIdx;
    int mFkeyIdx;

    FdoBoolean      m_IsGeometryFromOrdinatesWanted;
};

typedef FdoPtr<FdoSmPhRdPropertyReader> FdoSmPhRdPropertyReaderP;

#endif


