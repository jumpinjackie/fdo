#ifndef FDORDBMSPHRDODBCCONSTRAINTREADER_H
#define FDORDBMSPHRDODBCCONSTRAINTREADER_H       1
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

#include <Sm/Ph/Rd/ConstraintReader.h>

// Odbc constraint reader implementation

class FdoSmPhRdOdbcConstraintReader : public FdoSmPhRdConstraintReader
{
public:
    // Create and execute the constraint reader
    //
    // Parameters:
    //      owner: get constraints owned by this owner.
    //      constraintName: If L"" then get all constraints for the owner.
    //          Otherwise retrieve only this constraint.
    FdoSmPhRdOdbcConstraintReader(
        FdoSmPhOwnerP owner,
        FdoStringP constraintName
    );

    FdoSmPhRdOdbcConstraintReader(
        FdoSmPhOwnerP owner,
		FdoStringP tableName,
        FdoStringP constraintType
    );

    // Deactivates the constraint reader.
    ~FdoSmPhRdOdbcConstraintReader(void);

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhOwnerP owner,
        FdoStringP constraintName
    );

    FdoSmPhReaderP MakeReader(
        FdoSmPhOwnerP owner,
		FdoStringP	tableName,
        FdoStringP constraintName
    );

    FdoSmPhOwnerP mOwner;
    FdoStringP mConstraintName;
	FdoStringP	mTableName;
};

#endif
