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
#ifndef FDOSMPHRDPOSTGISCOLUMNREADER_H
#define FDOSMPHRDPOSTGISCOLUMNREADER_H

#include <Sm/Ph/Rd/ColumnReader.h>
#include <Inc/Rdbi/context.h>
#include <Sm/Ph/Rd/TableJoin.h>

/// Definition of column reader class.
/// This class is used by PostGIS provider.
///
class FdoSmPhRdPostGisColumnReader : public FdoSmPhRdColumnReader
{
public:

    /// Alternative constructor.
    /// \todo To be documented.
    FdoSmPhRdPostGisColumnReader(FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    /// Alternative constructor.
    /// \todo To be documented.
    FdoSmPhRdPostGisColumnReader(FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join);

    /// Destructor deactivates the query.
    ~FdoSmPhRdPostGisColumnReader();

	/// Fetches next row and generates appropriate Schema Manager
    /// based on the type retrieved from PostGIS datastore.
    ///
    /// \return
    /// true - next row is returned, false - no row returned, end of result.
    ///
	virtual bool ReadNext();

    /// Get physical type for the current column.
    /// \return
    /// Type enumerator defined by Generic Schema Manager.
    virtual FdoSmPhColType GetType();

    virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:

    /// Create the column reader.
    /// \todo To be documented.
    FdoSmPhReaderP MakeQueryReader(FdoSmPhMgrP mgr, const FdoSmPhOwner* owner,
        FdoSmPhDbObjectP dbObject,
        FdoSmPhRdTableJoinP join = NULL);

    /// \todo To be documented.
    FdoSmPhRowP MakeBinds(FdoSmPhMgrP mgr, FdoStringP tableOwner,
        FdoStringP tableName);

    /// Type of physical column.
    FdoSmPhColType mColType;

};

#endif // FDOSMPHRDPOSTGISCOLUMNREADER_H