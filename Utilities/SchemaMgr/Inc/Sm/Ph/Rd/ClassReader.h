#ifndef FDOSMPHRDCLASSREADER_H
#define FDOSMPHRDCLASSREADER_H		1
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
#include <Sm/Ph/Rd/DbObjectReader.h>

// This reader reads all of the classes for a schema, when no MetaSchema
// information is present. It reads each table in the given database schema and 
// automatically wraps a class around it.


class FdoSmPhRdClassReader : public FdoSmPhReader
{
public:
    /// Create and execute the reader
	FdoSmPhRdClassReader(
        FdoSmPhRowsP froms,         // rows and fields to retrieve.
        FdoStringP schemaName,      // name of Feature Schema that will contain the classes
        FdoSmPhMgrP mgr,            // Physical Schema Manager
        FdoBoolean keyedOnly = true,// If true, skip tables without key.
        FdoStringP database = L"",  // Database where RDBMS schema resides (current connection by default)
        FdoStringP owner = L""      // the RDBMS schema (defaults to current schema)
    );

    /// Deactivates the query.
	~FdoSmPhRdClassReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdClassReader() {}

    bool IsOrdinate(FdoSmPhColumnP column);

    virtual FdoStringP ClassifyObject( FdoStringP objectName, FdoSmPhDbObjType objectType );

    // Find the main geometric property for the class from the given column collection.
    // Returns the main geometric property, L"" if none could be determined
    // (non geometric properties, or there was a tie for best main geometry).
    // hasGeom is set to true, on return, if the input column has any geometric
    // columns.
    virtual FdoStringP FindGeometryProperty( FdoSmPhColumnsP cols,  bool& hasGeom );

private:

    FdoStringP mSchemaName;
    FdoBoolean mKeyedOnly;

    FdoSmPhOwnerP mOwner;
    FdoSmPhDbObjectsP mDbObjects;
    FdoInt32   mCurrDbObject;

    FdoBoolean      m_IsGeometryFromOrdinatesWanted;
};

// convenient smart pointer
typedef FdoPtr<FdoSmPhRdClassReader> FdoSmPhRdClassReaderP;

#endif


