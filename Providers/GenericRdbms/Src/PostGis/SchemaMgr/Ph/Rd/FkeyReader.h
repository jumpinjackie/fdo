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
#ifndef FDORDBMSPHRDPOSTGISFKEYREADER_H
#define FDORDBMSPHRDPOSTGISFKEYREADER_H

#include <Sm/Ph/Rd/FkeyReader.h>

/// Definition of foreign key reader for PostGIS provider.
///
class FdoSmPhRdPostGisFkeyReader : public FdoSmPhRdFkeyReader
{
public:

    /// Create and execute the foreign key reader.
    ///
    /// \param
    /// mgr [in] - pointer to instance of Physical Schema Manager.
    /// \param
    /// dbObject [in] - Retrieve foreign keys for this database object.
    ///
    FdoSmPhRdPostGisFkeyReader(FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    /// Create and execute the foreign key reader.
    ///
    /// \param
    /// mgr [in] - pointer to instance of Physical Schema Manager.
    /// \param
    /// owner [in] - 
    ///
    /// \todo To be documented
    FdoSmPhRdPostGisFkeyReader(FdoSmPhMgrP mgr, FdoSmPhOwnerP owner);

    /// Destructor deactivates the foreign key reader.
    ~FdoSmPhRdPostGisFkeyReader();

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

#endif // FDORDBMSPHRDPOSTGISFKEYREADER_H