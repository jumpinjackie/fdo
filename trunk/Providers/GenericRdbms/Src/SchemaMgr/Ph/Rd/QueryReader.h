#ifndef FDORDBMSPHRDGRDQUERYREADER_H
#define FDORDBMSPHRDGRDQUERYREADER_H        1
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

#include <Sm/Ph/Rd/QueryReader.h>
#include "RowArrayCollection.h"

// Query reader class for RDBMS-type providers.
class FdoSmPhRdGrdQueryReader : public FdoSmPhRdQueryReader
{
public:
    // Create and execute a query from a full statement, plus field and bind lists.
    FdoSmPhRdGrdQueryReader(
        FdoSmPhRowP fields,         // query's select column list.
        FdoStringP sStatement,      // query statement. Must reference each column in the fields
                                    // collection and each field in the binds collection.
        FdoSmPhMgrP mgr,            // Physical Schema Manager
        FdoSmPhRowP binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
                                    // If NULL, then this is not a bound query.
    );

    // Create and execute a query from a where clause, plus row and bind lists.
    // In this case, the query statement is built from the row list (select columns and
    // from clause) and the where clause.
    FdoSmPhRdGrdQueryReader(
        FdoSmPhRowsP froms,         // query's select list organized by database object being
                                    // queried from.
        FdoStringP sClauses,        // where clause. L"" if there is no query criteria. If
                                    // froms has multiple rows then this where clause just
                                    // define a join between each row.
        FdoSmPhMgrP mgr,            // Physical Schema Manager.
        FdoSmPhRowP binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
                                    // If NULL, then this is not a bound query.

    );

    // Deactivates the query.
    ~FdoSmPhRdGrdQueryReader(void);

    // Get the next row and make it current.
    virtual bool ReadNext();

    // Get a string column value by name.
    // Value for current row is returned.
    // Returns L"" if value is null.
    FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

    // Get an integer column value by name.
    // Value for current row is returned.
    // Returns 0 if value is null.
    int GetInteger( FdoStringP tableName, FdoStringP fieldName );

    // Get a long column value by name.
    // Value for current row is returned.
    // Returns 0 if value is null.
    long GetLong( FdoStringP tableName, FdoStringP fieldName );

    // Get an double precision column value by name.
    // Value for current row is returned.
    // Returns 0.0 if value is null.
    double GetDouble( FdoStringP tableName, FdoStringP fieldName );

    // Get a boolean column value by name.
    // Value for current row is returned.
    // Returns false if value is null.
    bool GetBoolean( FdoStringP tableName, FdoStringP fieldName );

    // (re)-execute the query.
    // TODO: re-execute does not work yet.
    void Execute();

	// End select before end-of-fetch
	void EndSelect();

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdGrdQueryReader() {}

    // Returns true if the value for the given field was overridden after
    // it was fetched from the RDBMS.
    bool FieldIsModified( FdoStringP tableName, FdoStringP fieldName );

    FdoSmPhRdGrdRowArraysP mFetches;

private:
    FdoSmPhRdGrdFieldArrayP GetFieldArray( FdoStringP tableName, FdoStringP fieldName);

    GdbiStatement* mQuery;
    GdbiQueryResult* mResults;

    int  mQid;
};

typedef FdoPtr<FdoSmPhRdGrdQueryReader> FdoSmPhRdGrdQueryReaderP;

#endif
