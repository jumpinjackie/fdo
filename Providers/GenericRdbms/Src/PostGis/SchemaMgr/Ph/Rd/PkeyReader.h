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
#ifndef FDORDBMSPHRDPOSTGISPKEYREADER_H
#define FDORDBMSPHRDPOSTGISPKEYREADER_H

#include <Sm/Ph/Rd/PkeyReader.h>
//#include <Inc/Ocidr/context.h>

/// Definition of primary key reader for PostGIS provider.
///
class FdoSmPhRdPostGisPkeyReader : public FdoSmPhRdPkeyReader
{
public:

    /// Create and execute the primary key reader.
    ///
    /// \param
    /// mgr [in] - pointer to instance of Physical Schema Manager.
    /// \param
    /// dbObject [in] - Retrieve primary keys for this database object.
    ///
    FdoSmPhRdPostGisPkeyReader(FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    /// Create and execute the primary key reader.
    ///
    /// \param
    /// mgr [in] - pointer to instance of Physical Schema Manager.
    /// \param
    /// owner [in] - 
    ///
    /// \todo To be documented
    FdoSmPhRdPostGisPkeyReader(FdoSmPhMgrP mgr, FdoSmPhOwnerP owner);

    // Deactivates the primary key reader.
    ~FdoSmPhRdPostGisPkeyReader();

private:

    /// 
    /// \param
    /// mgr [in] - pointer to instance of Physical Schema Manager.
    /// \param
    /// owner [in] - 
    /// \param
    /// dbObject [in] - 
    ///
    /// \todo To be documented.
    FdoSmPhReaderP MakeReader(FdoSmPhMgrP mgr,
        const FdoSmPhOwner* owner,
        FdoSmPhDbObjectP dbObject);

    /// \todo To be documented.
    FdoSmPhDbObjectP mDbObject;

};

#endif // FDORDBMSPHRDPOSTGISPKEYREADER_H
