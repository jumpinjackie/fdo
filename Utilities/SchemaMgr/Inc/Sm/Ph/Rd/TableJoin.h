#ifndef FDOSMPHRDTABLEJOIN_H
#define FDOSMPHRDTABLEJOIN_H		1
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

#include <Sm/Ph/Rd/Join.h>

// This class represents a join on table name to a table that contains a column
// that has table names as its values.
// It can be passed to various Physical Object Readers (e.g. FdoSmPhRdDbObjectReader)
// to only read rows that correspond to selected rows in the join table. 
class FdoSmPhRdTableJoin : public FdoSmPhRdJoin
{
public:
	FdoSmPhRdTableJoin(
        FdoStringP name,                // name assigned to this join. Becomes the alias for the join table.
                                        // when the join is added to a select.
        FdoSmPhColumnP tableNameColumn, // name of the column containing table names. The join column.
        FdoStringP whereClause          // clause for qualifying the rows selected from the join table.
    );

    // returns a clause for adding this join to a where clause of a select.
    // Includes the whereClause passed into the constructor plus a join expressionsfor
    // joining the join table to the main table in the select.
    FdoStringP GetWhere( 
        FdoStringP joinColumn // column from main table in select, that corresponds to join column.
    );

    // Same as previous function but used by providers that have physical schemas.
    // The join is done by schema and table name.
    FdoStringP GetWhere( 
        FdoSmPhMgrP mgr, 
        FdoStringP schemaColumn, 
        FdoStringP tableColumn 
    );
    // Get the join column for this table join..
    FdoStringP GetJoinColumn();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdTableJoin() {}

    virtual ~FdoSmPhRdTableJoin();

    // Turn the join column into a single element column name list.
    FdoSmPhColumnsP MakeColumnList( FdoSmPhColumnP tableNameColumn );
};

typedef FdoPtr<FdoSmPhRdTableJoin> FdoSmPhRdTableJoinP;

#endif


