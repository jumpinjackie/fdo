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
#ifndef FDOSMPHPOSTGISINDEX_H
#define FDOSMPHPOSTGISINDEX_H

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Index.h"
#include <Sm/Ph/Table.h>

/// Definition of database index for PostGIS provider.
///
class FdoSmPhPostGisIndex :
    public FdoSmPhGrdIndex,
    public FdoSmPhPostGisDbObject
{
public:
   
    /// Constructs an instance of a Index object.
    ///
    /// \param
    /// name [in] - name of index.
    /// \param
    /// parent [in] - index parent table.
    /// \param
    /// isUnique [in] - true if the index is unique, false if non-unique.
    /// \param
    /// state [in] - indicates whether the index is new or already exists.
    ///
    FdoSmPhPostGisIndex(FdoStringP name,
        FdoSmPhDbObject* parent,
        bool isUnique,
        FdoSchemaElementState state = FdoSchemaElementState_Added,
        FdoSmPhRdDbObjectReader* reader = NULL);

    /// Destructor.
    ~FdoSmPhPostGisIndex(void);

protected:

    // Get unqualified index name for use in DDL statement
    virtual FdoStringP GetDDLName() const;

    /// Override column reader creator to return empty reader.
    /// Parent table handles the adding of the right columns to each index.
    ///
    /// \return
    /// Column reader.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();

    /// \todo To be documented.
    virtual bool Add();

    /// \todo To be documented.
    virtual bool Delete();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisIndex> FdoSmPhPostGisIndexP;

#endif // FDOSMPHPOSTGISINDEX_H
