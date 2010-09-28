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
#include "stdafx.h"
#include "Database.h"
#include "Owner.h"
#include "Mgr.h"
#include "Rd/OwnerReader.h"

FdoSmPhPostGisDatabase::FdoSmPhPostGisDatabase(FdoStringP name,
    FdoPtr<FdoSmPhMgr> mgr,
    FdoSchemaElementState elementState)
    : FdoSmPhDatabase(name, mgr, elementState)
{
    // idle
}

FdoSmPhPostGisDatabase::~FdoSmPhPostGisDatabase()
{
    // idle
}

FdoPtr<FdoSmPhRdOwnerReader> FdoSmPhPostGisDatabase::CreateOwnerReader(
    FdoStringP owner) const
{
    FdoSmPhPostGisDatabase* pgDb = NULL;
    pgDb = const_cast<FdoSmPhPostGisDatabase*>(this);
    FDO_SAFE_ADDREF(pgDb);

    // Create owner reader instance used to read collection of datastores
    // (schemas) available in currently connected PostgreSQL database.
    FdoSmPhRdPostGisOwnerReader* reader = NULL;
    reader = new FdoSmPhRdPostGisOwnerReader(pgDb, owner);

    return reader;
}

FdoSmPhOwnerP FdoSmPhPostGisDatabase::NewOwner(FdoStringP owner,
    bool hasMetaSchema,
    FdoSchemaElementState state)
{

    FdoStringP pgSchemaName = GetManager()->GetDefaultOwnerName();

    // Select final owner name
    FdoStringP schemaName = (owner.GetLength() > 0 ? owner : pgSchemaName);

    FdoSmPhPostGisOwner* pgOwner = NULL;
    pgOwner = new FdoSmPhPostGisOwner(schemaName, hasMetaSchema, this, state);

    return pgOwner;
}

bool FdoSmPhPostGisDatabase::Add()
{
    return true;
}

bool FdoSmPhPostGisDatabase::Modify()
{
    return true;
}

bool FdoSmPhPostGisDatabase::Delete()
{
    return true;
}
