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
#ifndef FDOSMPHPOSTGISVIEW_H
#define FDOSMPHPOSTGISVIEW_H

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/View.h"

// PostGis Provider implementation of a View.
class FdoSmPhPostGisView :
    public FdoSmPhGrdView,
    public FdoSmPhPostGisDbObject
{
public:
    
    /// Create an instance of a view.
    ///
    /// \param
    /// viewName [in] - name of the view to be created.
    /// \param
    /// rootDatabase [in] - RDBMS instance where foreign table resides.
    /// \param
    /// rootOwner [in] - RDBMS schema where foreign table resides.
    /// \param
    /// rootObjectName [in] - the name of the foreign table.
    /// \param
    /// owner [in] - owning schema.
    /// \param
    /// state [in] - indicates whether view is new.
    ///
    FdoSmPhPostGisView(FdoStringP viewName,
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
        const FdoSmPhOwner* owner,
        FdoSchemaElementState state = FdoSchemaElementState_Added,
        FdoSmPhRdDbObjectReader* reader = NULL);

    /// Destructor.
    ~FdoSmPhPostGisView();

protected:

    /// Get SQL name for the root object.
    ///
    /// \return
    /// String with name of root object for view.
    virtual FdoStringP GetRootNameSql();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisView> FdoSmPhPostGisViewP;

#endif // FDOSMPHPOSTGISVIEW_H