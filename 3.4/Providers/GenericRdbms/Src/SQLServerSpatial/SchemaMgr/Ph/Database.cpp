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

#include "stdafx.h"
#include "Database.h"
#include "Owner.h"
#include "Mgr.h"
#include "Rd/OwnerReader.h"

FdoSmPhSqsDatabase::FdoSmPhSqsDatabase(
    FdoStringP name,
    FdoPtr<FdoSmPhMgr> mgr,
    FdoSchemaElementState elementState
) :
    FdoSmPhDatabase(name, mgr, elementState)
{
}

FdoSmPhSqsDatabase::~FdoSmPhSqsDatabase(void)
{
}

FdoPtr<FdoSmPhRdOwnerReader> FdoSmPhSqsDatabase::CreateOwnerReader( FdoStringP owner ) const
{
    FdoSmPhSqsDatabase* pDatabase = (FdoSmPhSqsDatabase*) this;

    return new FdoSmPhRdSqsOwnerReader( FDO_SAFE_ADDREF(pDatabase), owner );
}

void FdoSmPhSqsDatabase::UnsetCurrentOwner()
{
    FdoSmPhSqsMgrP          mgr             = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection*         gdbiConn        = mgr->GetGdbiConnection();

    gdbiConn->ExecuteNonQuery( "use master", true );
}

FdoSmPhOwnerP FdoSmPhSqsDatabase::NewOwner(
    FdoStringP owner,
    bool hasMetaSchema,
    FdoSchemaElementState elementState
)
{

    FdoStringP SqsSchemaName = GetManager()->GetDefaultOwnerName();

    return new FdoSmPhSqsOwner(
        owner.GetLength() > 0 ? owner : SqsSchemaName,
        hasMetaSchema,
        this,
        elementState
    );
}

