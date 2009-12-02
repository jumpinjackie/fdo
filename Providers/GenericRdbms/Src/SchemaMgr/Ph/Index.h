#ifndef FDOSMPHGRDINDEX_H
#define FDOSMPHGRDINDEX_H       1
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

#include <Sm/Ph/Index.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Rd/DbObjectReader.h>

// Oracle Provider implementation of an Index.
class FdoSmPhGrdIndex : public FdoSmPhIndex
{
public:
    // Constructs an instance of a Index object.
    //
    // Parameters:
    //      name: Index name
    //      pParent: the index's table.
    //      isUnique: true if the index is unique, false if non-unique
    //      elementState: Indicates whether the index is new or already exists.

    FdoSmPhGrdIndex(
        FdoStringP name,
        FdoSmPhDbObject* pParent,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhGrdIndex(void);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdIndex() {}

    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

};

typedef FdoPtr<FdoSmPhGrdIndex> FdoSmPhGrdIndexP;

#endif
