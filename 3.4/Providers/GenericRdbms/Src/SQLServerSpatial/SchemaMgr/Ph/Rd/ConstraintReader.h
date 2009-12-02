#ifndef FDORDBMSPHRDSQSCONSTRAINTREADER_H
#define FDORDBMSPHRDSQSCONSTRAINTREADER_H       1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer constraint reader implementation

class FdoSmPhRdSqsConstraintReader : public FdoSmPhRdConstraintReader
{
public:
    // Create and execute the constraint reader
    //
    // Parameters:
    //      owner: get constraints owned by this owner.
    //      constraintName: If L"" then get all constraints for the owner.
    //          Otherwise retrieve only this constraint.
    FdoSmPhRdSqsConstraintReader(
        FdoSmPhOwnerP owner,
        FdoStringP constraintName
    );

    FdoSmPhRdSqsConstraintReader(
        FdoSmPhOwnerP owner,
		FdoStringP tableName,
        FdoStringP constraintType
    );

    FdoSmPhRdSqsConstraintReader(
        FdoSmPhOwnerP owner,
		FdoStringsP tableNames,
        FdoStringP constraintType
    );

    FdoSmPhRdSqsConstraintReader(
        FdoSmPhOwnerP owner,
		FdoSmPhRdTableJoinP join,
        FdoStringP constraintType
    );

    // Deactivates the constraint reader.
    ~FdoSmPhRdSqsConstraintReader(void);

    FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhOwnerP owner,
        FdoStringP constraintName
    );

    FdoSmPhReaderP MakeReader(
        FdoSmPhOwnerP owner,
		FdoStringsP	tableNames,
		FdoSmPhRdTableJoinP join,
        FdoStringP constraintType
    );

    FdoStringsP Table2Tables( FdoStringP tableName );

    FdoSmPhOwnerP mOwner;
    FdoStringP mConstraintName;
	FdoStringP mTableName;
};

#endif
