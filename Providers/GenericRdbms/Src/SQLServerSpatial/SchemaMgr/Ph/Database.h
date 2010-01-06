#ifndef FDOSMPHSQSDATABASE_H
#define FDOSMPHSQSDATABASE_H        1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Mgr.h>

// SqlServer Provider implementation of a Database object.
class FdoSmPhSqsDatabase : public FdoSmPhDatabase
{
public:

    // Constructs an instance of a Database
    //
    // Parameters:
    //      name: database name. L"" for the default (current) database.
    //      mgr: the Physical Schema Manager.
    //      elementState: determines whether this database object is new
    FdoSmPhSqsDatabase(
        FdoStringP name,
        FdoPtr<FdoSmPhMgr> mgr,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhSqsDatabase(void);

    // Create a reader to get all owner objects for this database.
    // TODO: add flag to cache owners
    virtual FdoPtr<FdoSmPhRdOwnerReader> CreateOwnerReader( FdoStringP owner = L"") const;

    virtual void UnsetCurrentOwner();

protected:
    virtual FdoSmPhOwnerP NewOwner(
        FdoStringP owner,
        bool hasMetaSchema,
        FdoSchemaElementState elementState
    );

    virtual bool Add() {return true;}
    virtual bool Modify() {return true;}
    virtual bool Delete() {return true;}

};

typedef FdoPtr<FdoSmPhSqsDatabase> FdoSmPhSqsDatabaseP;

#endif
