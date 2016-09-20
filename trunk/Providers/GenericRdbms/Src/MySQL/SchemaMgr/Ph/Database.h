#ifndef FDOSMPHMYSQLDATABASE_H
#define FDOSMPHMYSQLDATABASE_H        1
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

#include <Sm/Ph/Mgr.h>

// MySql Provider implementation of a Database object.
class FdoSmPhMySqlDatabase : public FdoSmPhDatabase
{
public:

    // Constructs an instance of a Database
    //
    // Parameters:
    //      name: database name. L"" for the default (current) database.
    //      mgr: the Physical Schema Manager.
    //      elementState: determines whether this database object is new
    FdoSmPhMySqlDatabase(
        FdoStringP name,
        FdoPtr<FdoSmPhMgr> mgr,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhMySqlDatabase(void);

    /// Create a reader to get owner objects for this database.
    virtual FdoPtr<FdoSmPhRdOwnerReader> CreateOwnerReader( FdoStringP owner = L"") const;

    /// Create a reader to get character sets for this database.
    virtual FdoPtr<FdoSmPhRdCharacterSetReader> CreateCharacterSetReader( FdoStringP characterSetName = L"") const;

    /// Create a reader to get collations for this database.
    virtual FdoPtr<FdoSmPhRdCollationReader> CreateCollationReader( FdoStringP collationName = L"") const;

protected:
    /// Instantiate an owner (can be new or existing).
    virtual FdoSmPhOwnerP NewOwner(
        FdoStringP owner,
        bool hasMetaSchema,
        FdoSchemaElementState elementState,
        FdoSmPhRdOwnerReader* reader
    );

    // Instantiate a character set (must exist in RDBMS).
    virtual FdoSmPhCharacterSetP NewCharacterSet(
        FdoStringP characterSetName,
        FdoSmPhRdCharacterSetReader* reader
    );

    // Instantiate a collation (must exist in RDBMS).
    virtual FdoSmPhCollationP NewCollation(
        FdoStringP collationName,
        FdoSmPhRdCollationReader* reader
    );

    virtual bool Add() {return true;}
    virtual bool Modify() {return true;}
    virtual bool Delete() {return true;}

};

typedef FdoPtr<FdoSmPhMySqlDatabase> FdoSmPhMySqlDatabaseP;

#endif
