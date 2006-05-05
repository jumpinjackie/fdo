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

#include "stdafx.h"
#include "Database.h"
#include "Owner.h"
#include "Mgr.h"
#include "Rd/OwnerReader.h"

FdoSmPhOdbcDatabase::FdoSmPhOdbcDatabase(
    FdoStringP name,
    FdoPtr<FdoSmPhMgr> mgr,
    FdoSchemaElementState elementState
) :
    FdoSmPhDatabase(name, mgr, elementState)
{
}

FdoSmPhOdbcDatabase::~FdoSmPhOdbcDatabase(void)
{
}

FdoPtr<FdoSmPhRdOwnerReader> FdoSmPhOdbcDatabase::CreateOwnerReader( FdoStringP owner ) const
{
    FdoSmPhOdbcDatabase* pDatabase = (FdoSmPhOdbcDatabase*) this;

    return new FdoSmPhRdOdbcOwnerReader( FDO_SAFE_ADDREF(pDatabase), owner );
}

void FdoSmPhOdbcDatabase::UnsetCurrentOwner()
{
    FdoSmPhOdbcMgrP          mgr             = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection*         gdbiConn        = mgr->GetGdbiConnection();

    gdbiConn->ExecuteNonQuery( "use master", true );
}

FdoSmPhOwnerP FdoSmPhOdbcDatabase::NewOwner(
    FdoStringP owner,
    bool hasMetaSchema,
    FdoSchemaElementState elementState
)
{

    FdoStringP OdbcSchemaName = GetManager()->GetDefaultOwnerName();

    return new FdoSmPhOdbcOwner(
        owner.GetLength() > 0 ? owner : OdbcSchemaName,
        false,  // hasMetaSchema
        this,
        elementState
    );
}

