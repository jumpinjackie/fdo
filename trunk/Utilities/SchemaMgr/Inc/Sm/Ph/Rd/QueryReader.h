#ifndef FDOSMPHRDQUERYREADER_H
#define FDOSMPHRDQUERYREADER_H		1
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

// This reader reads all of the rows returned by an arbitrary database query


class FdoSmPhRdQueryReader : public FdoSmPhReader
{
public:
    /// Execute the query. If this is a bound query, the current values of the 
    /// bind fields are used as the bind values.
    /// Execute can be called multiple times (usually only for bound queries). The 
    /// bind field values would typically be modified before each call to execute.
    virtual void Execute() = 0;

    /// Get the next row and make it current.
	virtual bool ReadNext() = 0;

    /// Get a string column value by name.
    /// Value for current row is returned.
    /// Returns L"" if value is null. 
	FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName ) = 0;

    /// Get an integer column value by name.
    /// Value for current row is returned.
    /// Returns 0 if value is null. 
	int GetInteger( FdoStringP tableName, FdoStringP fieldName ) = 0;
	
    /// Get a long column value by name.
    /// Value for current row is returned.
    /// Returns 0 if value is null. 
    long GetLong( FdoStringP tableName, FdoStringP fieldName ) = 0;

    /// Get an double precision column value by name.
    /// Value for current row is returned.
    /// Returns 0.0 if value is null. 
	double GetDouble( FdoStringP tableName, FdoStringP fieldName ) = 0;
	
    /// Get a boolean column value by name.
    /// Value for current row is returned.
    /// Returns false if value is null. 
	bool GetBoolean( FdoStringP tableName, FdoStringP fieldName ) = 0;

    /// Gets the bind fields, if this is a bound query.
    FdoSmPhRowP GetBinds()
    {
        return mBindFields;
    }

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdQueryReader() {}

    /// Create and execute a query from a full statement, plus field and bind lists.
	FdoSmPhRdQueryReader(
        FdoSmPhRowP fields,         // query's select column list.
        FdoStringP sStatement,      // query statement. Must reference each column in the fields
    /// collection and each field in the binds collection.
        FdoSmPhMgrP mgr,            // Physical Schema Manager
        FdoSmPhRowP binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
    /// If NULL, then this is not a bound query.
    );
	
    /// Create and execute a query from a where clause, plus row and bind lists.
    /// In this case, the query statement is built from the row list (select columns and 
    /// from clause) and the where clause.
	FdoSmPhRdQueryReader(
        FdoSmPhRowsP froms,         // query's select list organized by database object being
    /// queried from.
        FdoStringP sClauses,        // where clause. L"" if there is no query criteria. If
    /// froms has multiple rows then this where clause just 
    /// define a join between each row.
        FdoSmPhMgrP mgr,            // Physical Schema Manager.
        FdoSmPhRowP binds = (FdoSmPhRow*) NULL    // Fields to bind into query (Query parameters).
    /// If NULL, then this is not a bound query.

    );

    /// Deactivates the query.
	~FdoSmPhRdQueryReader(void);

	FdoStringP mStatement;						// current query statement
    FdoSmPhRowP mBindFields;                    // current bind fields

private:
    /// Helper for creating query statement from a set or rows and where clause.
	FdoStringP MakeStatement( FdoSmPhRowsP rows, FdoStringP sClauses );
    /// Helper for creating a row list from a single row.
    FdoSmPhRowsP MakeRows(FdoSmPhRowP row, FdoSmPhMgrP physicalSchema);
};

typedef FdoPtr<FdoSmPhRdQueryReader> FdoSmPhRdQueryReaderP;

#endif


