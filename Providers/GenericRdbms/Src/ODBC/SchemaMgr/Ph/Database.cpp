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

FdoSmPhOwnerP FdoSmPhOdbcDatabase::FindOwner(FdoStringP owner)
{
    // By default, GenericRdbms providers support a Pending connection state,
    // wherein the "datastore" connection property (seen here as the "owner"
    // input parameter) is an empty string and this method (in the base class)
    // would return NULL.  Otherwise, the owner name would be expected
    // to match one from the underlying database and the result would be non-NULL.
    //
    // For ODBC Provider, we skip the Pending state, and do not support
    // the "datastore" connection property.  The targetted owner name would 
    // always be passed here as empty, unless there is a schema override
    // specifying it.  3rd-party ODBC drivers that do not support owners
    // (schemas) typically return an empty string when requesting them,
    // so there is a match for the empty string case.  However, some drivers
    // support owners (schemas) and so getting owners won't match the empty string.
    //
    // To take care of the owner-supporting drivers, we'll add a special case
    // here to make sure that an empty owner name still results in a non-NULL
    // return value.  The database will not be searched in this case.

    FdoSmPhOwnersP owners = GetOwners();
    FdoSmPhOwnerP pOwner = owners->FindItem(owner);

    if (pOwner == NULL)
    {
        if (owner == NULL || owner.GetLength() <= 0)
        {
            pOwner = NewOwner(owner, false, FdoSchemaElementState_Unchanged);
            owners->Add( pOwner );  // Add to cache.
        }
        else
        {
            pOwner = FdoSmPhDatabase::FindOwner(owner);
        }
    }

    return(pOwner);
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

