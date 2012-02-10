#ifndef FDOSMPHRDJOIN_H
#define FDOSMPHRDJOIN_H		1
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

#include <Sm/Ph/Column.h>

// This class represents a join to a table
// It can be used to add sub-clauses to a select to add a join to the table. 
// Functions are provided to build these sub-clauses.
class FdoSmPhRdJoin : public FdoSmPhSchemaElement
{
public:
	FdoSmPhRdJoin(
        FdoStringP name,        // name assigned to this join. Becomes the alias for the join table.
                                // when the join is added to a select.
        FdoSmPhColumnsP columns,// join columns. All must be columns in the join table
        FdoStringP whereClause  // clause for qualifying the rows selected from the join table.
    );

    // returns a clause for adding the join table to the from clause of a select.
    FdoStringP GetFrom();

    // returns a clause for adding this join to a where clause of a select.
    // Includes the whereClause passed into the constructor plus join expressions for
    // joining the join table to the main table in the select.
    FdoStringP GetWhere( 
        FdoStringsP joinColumn //list of columns to join to the join columns. One column
                               // per join column must be supplied
    );

    // returns this join's where clause but does not add the join expressions to it.
    FdoStringP GetWhere();

    // returns the table or view that is being joined to.
    FdoSmPhDbObjectP GetDbObject();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdJoin() {}

	virtual ~FdoSmPhRdJoin(void);

    // get a join column qualified by join table alias.
    FdoStringP GetJoinColumn( FdoInt32 ix );

private:
    FdoSmPhColumnsP mColumns;
    FdoStringP mWhereClause;
};

typedef FdoPtr<FdoSmPhRdJoin> FdoSmPhRdJoinP;

#endif


