#ifndef FDOSMPHGRDOWNER_H
#define FDOSMPHGRDOWNER_H       1
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

#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>

// Generic RDBMS implementation of an Owner.
// Represents an Oracle schema (user)
class FdoSmPhGrdOwner : public FdoSmPhOwner
{
public:

    // Constructs an instance of an Owner object.
    //
    // Parameters:
    //      name: owner name
    //      pDatabase: database containing this owner.
    //      elementState: determines whether this database object is new
    FdoSmPhGrdOwner(
        FdoStringP name,
        bool hasMetaSchema,
        const FdoSmPhDatabase* pDatabase,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhGrdOwner(void);

    // Adds the MetaSchema tables and indexes to this owner.
    void AddMetaSchema( FdoStringsP keywords, bool IsSystem = false );

    // Some provider-specific statements cannot reference qualified object names, meaning
    // that we must activate the object's owner before executing.
    // This function activates this owner, executes the given statement and then switches
    // back to the original active owner.
    void ActivateAndExecute( FdoStringP sqlStmt );

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdOwner() {}

private:
};

typedef FdoPtr<FdoSmPhGrdOwner> FdoSmPhGrdOwnerP;

#endif
