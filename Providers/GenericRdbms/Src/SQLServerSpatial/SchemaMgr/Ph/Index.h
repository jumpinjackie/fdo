#ifndef FDOSMPHSQSINDEX_H
#define FDOSMPHSQSINDEX_H       1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Index.h"
#include <Sm/Ph/Table.h>

// SqlServer Provider implementation of an Index.
class FdoSmPhSqsIndex : public FdoSmPhGrdIndex, public FdoSmPhSqsDbObject
{
public:
    // Constructs an instance of a Index object.
    //
    // Parameters:
    //      name: Index name
    //      pParent: the index's table.
    //      isUnique: true if the index is unique, false if non-unique
    //      elementState: Indicates whether the index is new or already exists.

    FdoSmPhSqsIndex(
        FdoStringP name,
        FdoSmPhDbObject* pParent,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoSmPhRdDbObjectReader* reader = NULL
    );

    ~FdoSmPhSqsIndex(void);

    FdoStringP GetDbName() const;

protected:
    // Override column reader creator to return empty reader.
    // Parent table handles the adding of the right columns to each index.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();

    virtual bool Add();
    virtual bool Delete();
};

typedef FdoPtr<FdoSmPhSqsIndex> FdoSmPhSqsIndexP;

#endif
