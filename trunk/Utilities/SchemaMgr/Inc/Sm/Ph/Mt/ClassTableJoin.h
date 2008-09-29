#ifndef FDOSMPHMTCLASSTABLEJOIN_H
#define FDOSMPHMTCLASSTABLEJOIN_H		1
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

#include <Sm/Ph/Rd/TableJoin.h>

// This class represents a join on tablename to the f_classdefinition table
// It can be passed to various Physical Object Readers (e.g. FdoSmPhRdDbObjectReader)
// to only read rows for tables associated with a particular feature schema.

class FdoSmPhMtClassTableJoin : public FdoSmPhRdTableJoin
{
public:
	FdoSmPhMtClassTableJoin(
        FdoSmPhOwnerP owner,        // Datastore containing f_classdefinition
        FdoStringP schemaName       // Join against f_classdefinition rows for this feature schema.
    );

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhMtClassTableJoin() {}

	virtual ~FdoSmPhMtClassTableJoin(void);

    // returns the f_classdefinition table being joined to.
    FdoSmPhDbObjectP GetTable(
        FdoSmPhOwnerP owner        // Datastore containing f_classdefinition
    );

    // returns the join column (always f_classdefinition.tablename
    FdoSmPhColumnP GetColumn(
        FdoSmPhOwnerP owner        // Datastore containing f_classdefinition
    );

    // returns a where cause to only select f_classdefinition rows for the 
    // given feature schema
    FdoStringP MakeWhere(
        FdoSmPhOwnerP owner,        // Datastore containing f_classdefinition
        FdoStringP schemaName       // the feature schema
    );

private:
    FdoSmPhDbObjectP mClassDefTable;
    FdoSmPhColumnP mTableNameColumn;
};

typedef FdoPtr<FdoSmPhMtClassTableJoin> FdoSmPhMtClassTableJoinP;

#endif


