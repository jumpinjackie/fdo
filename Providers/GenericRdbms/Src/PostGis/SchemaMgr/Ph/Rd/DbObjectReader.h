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
#ifndef FDOSMPHRDPOSTGISDBOBJECTREADER_H
#define FDOSMPHRDPOSTGISDBOBJECTREADER_H

#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

/// Definition of database object reader.
/// This class is used by PostGIS provider.
///
class FdoSmPhRdPostGisDbObjectReader : public FdoSmPhRdDbObjectReader
{
public:

    /// Create and execute the database object reader.
    ///
    /// \param
    /// owner [in] - get objects owned by this owner.
    /// \param
    /// objectName [in] - If empty string then get all objects
    /// for the owner. Otherwise retrieve only this object.
    ///
    FdoSmPhRdPostGisDbObjectReader(FdoSmPhOwnerP owner,
        FdoStringP objectName = L"");

    /// \todo To be documented.
    FdoSmPhRdPostGisDbObjectReader(FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join);

    /// Destructor the database object reader.
    ~FdoSmPhRdPostGisDbObjectReader();

    /// Get the type of the current database object.
    /// Currently, the reader recognizes following types:
    /// - FdoSmPhDbObjType_Table
    /// - FdoSmPhDbObjType_View
    /// - FdoSmPhDbObjType_Unknown
    ///
    /// \return
    /// Type of current database object.
    ///
    virtual FdoSmPhDbObjType GetType();

   // virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

protected:

    /// Generates SQL query and attaches it to the database object reader.
    ///
    /// \param
    /// owner [in] - get objects owned by this owner.
    /// \param
    /// objectName [in] - If empty string then get all objects
    /// for the owner. Otherwise retrieve only this object.
    /// \param
    /// join [in] - object representing a join on table name to
    /// a table that contains a column that has table names as its values.
    /// \return
    /// Instance of Physical Schema reader for a database object.
    ///
    FdoSmPhReaderP MakeQueryReader(FdoSmPhOwnerP owner,
        FdoStringP objectName = L"",
        FdoSmPhRdTableJoinP join = NULL);

    /// \todo To be documented.
    FdoSmPhRowP MakeBinds(FdoSmPhMgrP mgr, FdoStringP ownerName,
        FdoStringP objectName);

    /// Creates a set of rows describing the fields for this
    /// reader.
    /// There is one field per database object attribute.
    ///
    /// \param
    /// mgr [in] - instance of physical schema manager.
    /// \return
    /// ???
    ///
    /// \todo To be documented.
    ///
    //virtual FdoSmPhRowsP MakeRows(FdoSmPhMgrP mgr);

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhRdPostGisDbObjectReader> FdoSmPhRdPostGisDbObjectReaderP;

#endif // FDOSMPHRDPOSTGISDBOBJECTREADER_H