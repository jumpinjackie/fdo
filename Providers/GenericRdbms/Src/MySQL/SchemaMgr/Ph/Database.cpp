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

FdoSmPhMySqlDatabase::FdoSmPhMySqlDatabase(
    FdoStringP name,
    FdoPtr<FdoSmPhMgr> mgr,
    FdoSchemaElementState elementState
) :
    FdoSmPhDatabase(name, mgr, elementState)
{
}

FdoSmPhMySqlDatabase::~FdoSmPhMySqlDatabase(void)
{
}

FdoPtr<FdoSmPhRdOwnerReader> FdoSmPhMySqlDatabase::CreateOwnerReader( FdoStringP owner ) const
{
    FdoSmPhMySqlDatabase* pDatabase = (FdoSmPhMySqlDatabase*) this;

    return new FdoSmPhRdMySqlOwnerReader( FDO_SAFE_ADDREF(pDatabase), owner );
}

FdoSmPhOwnerP FdoSmPhMySqlDatabase::NewOwner(
    FdoStringP owner,
    bool hasMetaSchema,
    FdoSchemaElementState elementState
)
{

    FdoStringP mqlSchemaName = GetManager()->GetDefaultOwnerName();

    return new FdoSmPhMySqlOwner(
        owner.GetLength() > 0 ? owner : mqlSchemaName,
        hasMetaSchema,
        this,
        elementState
    );
}

