//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDORDBMSPHRDPOSTGISCONSTRAINTREADER_H
#define FDORDBMSPHRDPOSTGISCONSTRAINTREADER_H

#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

/// Defines constraint reader.
/// This class is used by PostGIS provider.
///
class FdoSmPhRdPostGisConstraintReader : public FdoSmPhRdConstraintReader
{
public:

    /// Create and execute the constraint reader.
    ///
    /// \param:
    /// owner [in] - get constraints owned by this owner.
    /// \param
    /// constraintName [in] - If empty string then get all constraints
    /// for the owner. Otherwise retrieve only this constraint.
    ///
    FdoSmPhRdPostGisConstraintReader(FdoSmPhOwnerP owner,
        FdoStringP constraintName);

    /// \todo To be documented.
    FdoSmPhRdPostGisConstraintReader(FdoSmPhOwnerP owner, FdoStringP tableName,
        FdoStringP constraintType);

    FdoSmPhRdPostGisConstraintReader(
        FdoSmPhOwnerP owner,
		FdoStringsP tableNames,
        FdoStringP constraintType
    );

    /// \todo To be documented.
    FdoSmPhRdPostGisConstraintReader(FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join,
        FdoStringP constraintType);

    FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

    /// Destructor deactivates the constraint reader.
    /// \todo To be documented.
    ~FdoSmPhRdPostGisConstraintReader();

protected:
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );
    
private:

    /// \todo To be documented.
    FdoSmPhReaderP MakeReader(FdoSmPhOwnerP owner, FdoStringP constraintName);

    /// \todo To be documented.
    FdoSmPhReaderP MakeReader(FdoSmPhOwnerP owner, FdoStringsP tableNames,
        FdoSmPhRdTableJoinP join,
        FdoStringP constraintType);

    FdoSmPhOwnerP mOwner;
    FdoStringP mConstraintName;
	FdoStringP mTableName;

};

#endif // FDORDBMSPHRDPOSTGISCONSTRAINTREADER_H
