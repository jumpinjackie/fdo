#ifndef FDOSMPHODBCVIEW_H
#define FDOSMPHODBCVIEW_H        1
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

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/View.h"

// Odbc Provider implementation of a View.
class FdoSmPhOdbcView : public FdoSmPhGrdView, public FdoSmPhOdbcDbObject
{
public:
    // Create an instance of a view.
    //
    // Parameters:
    //      viewName: the name of the view to created.
    //      rootDatabase: RDBMS instance where foreign table resides.
    //      rootOwner:  RDBMS schema where foreign table resides.
    //      rootObjectName: the name of the foreign table.
    //      pOwner: owning schema
    //      elementState: indicates whether view is new.
    FdoSmPhOdbcView(
        FdoStringP viewName,
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
        const FdoSmPhOwner* pOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoSmPhRdDbObjectReader* reader = NULL
    );

    ~FdoSmPhOdbcView(void);

protected:
    // Get SQL name for the root object
    virtual FdoStringP GetRootNameSql();

private:
    virtual bool Add();
    virtual bool Delete();
};

typedef FdoPtr<FdoSmPhOdbcView> FdoSmPhOdbcViewP;

#endif
