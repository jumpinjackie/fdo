#ifndef FDOSMPHRDASSOCIATIONREADER_H
#define FDOSMPHRDASSOCIATIONREADER_H		1
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

// This class is used by the Physical Schema object to query the 
// database for schema information.
//
// To perform the query, the caller creates this object with a
// GQL statement, which is parsed and executed. The caller then
// can call ReadNext() to get each row returned by the query and 
// the various Get functions to get the row data.
//
// The query is initially positioned before the first row, so 
// ReadNext must be called before data can be retrieved.

class FdoSmPhRdAssociationReader : public FdoSmPhReader
{
public:
    /// Create and execute a query
	//
    /// Parameters:
    /// 	sStatement: GQL statement to execute
    /// 	physicalSchema: holds the connection for the database to query.
	FdoSmPhRdAssociationReader(FdoSmPhRowsP froms, FdoStringP pkTableName, FdoStringP fkTableName, bool bAnd, FdoSmPhMgrP physicalSchema);

    /// Deactivates the query.
	~FdoSmPhRdAssociationReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdAssociationReader() {}

private:

    FdoSmPhTableP mpTable;
    FdoStringP mPkTableName;
    int mFkeyIdx;

};

typedef FdoPtr<FdoSmPhRdAssociationReader> FdoSmPhRdAssociationReaderP;

#endif


