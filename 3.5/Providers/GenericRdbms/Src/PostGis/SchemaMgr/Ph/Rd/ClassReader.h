#ifndef FDOSMPHRDPOSTGISCLASSREADER_H
#define FDOSMPHRDPOSTGISCLASSREADER_H		1
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

#include <Sm/Ph/Rd/ClassReader.h>

// This reader reads all of the classes for a schema, when no MetaSchema
// information is present. It reads each table in the given database schema and 
// automatically wraps a class around it.


class FdoSmPhRdPostGisClassReader : public FdoSmPhRdClassReader
{
public:
    /// Create and execute the reader
	FdoSmPhRdPostGisClassReader(
        FdoSmPhRowsP froms,         // rows and fields to retrieve.
        FdoStringP schemaName,      // name of Feature Schema that will contain the classes
        FdoStringP className,      // name of Feature class 
        FdoSmPhMgrP mgr,            // Physical Schema Manager
        FdoBoolean classifyDefaultTypes = true,// If true, classify only types of objects that are classified by default
                                               // If false, classify all types of object than can be classified
        FdoStringP database = L"",  // Database where RDBMS schema resides (current connection by default)
        FdoStringP owner = L""      // the RDBMS schema (defaults to current schema)
    );

    /// Deactivates the query.
	~FdoSmPhRdPostGisClassReader(void);

    virtual bool ReadNext();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdPostGisClassReader() {}

    // Returns true if the given database object's corresponding class is in the Feature Schema
    // for this class reader. 
    // No checking is done against the object's type. This is performed by ClassifyObjectType()
    virtual FdoStringP ClassifyObject( FdoSmPhDbObjectP dbObject );

private:
    FdoStringP mPgSchemaName;
    FdoSmPhDbObjectP mDbObject;
};

// convenient smart pointer
typedef FdoPtr<FdoSmPhRdPostGisClassReader> FdoSmPhRdPostGisClassReaderP;

#endif


