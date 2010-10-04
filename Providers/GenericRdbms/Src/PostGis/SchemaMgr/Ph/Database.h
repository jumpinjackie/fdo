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
#ifndef FDOSMPHPOSTGISDATABASE_H
#define FDOSMPHPOSTGISDATABASE_H

#include <Sm/Ph/Mgr.h>

/// Defines database object.
/// Implementation used by PostGIS provider.
///
class FdoSmPhPostGisDatabase : public FdoSmPhDatabase
{
public:

    // Constructs an instance of a Database
    //
    // Parameters:
    //      name: database name. L"" for the default (current) database.
    //      mgr: the Physical Schema Manager.
    //      elementState: determines whether this database object is new
    FdoSmPhPostGisDatabase(
        FdoStringP name,
        FdoPtr<FdoSmPhMgr> mgr,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhPostGisDatabase();

    /// Create a reader to get all owner objects for this database.
    /// \todo TODO: add flag to cache owners
    virtual FdoPtr<FdoSmPhRdOwnerReader> CreateOwnerReader(
        FdoStringP owner = L"") const;

protected:

    virtual FdoSmPhOwnerP NewOwner(
        FdoStringP owner, 
        bool hasMetaSchema,
        FdoSchemaElementState state,
        FdoSmPhRdOwnerReader* reader = NULL);

    /// \todo To be documented.
    virtual bool Add();

    /// \todo To be documented.
    virtual bool Modify();

    /// \todo To be documented.
    virtual bool Delete();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisDatabase> FdoSmPhPostGisDatabaseP;

#endif // FDOSMPHPOSTGISDATABASE_H
