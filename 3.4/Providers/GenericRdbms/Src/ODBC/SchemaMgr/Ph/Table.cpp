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
#include "Table.h"
#include "Owner.h"
#include "Mgr.h"
#include "Rd/PkeyReader.h"
#include "Rd/FkeyReader.h"
#include "Rd/ConstraintReader.h"
#include "Rd/IndexReader.h"
#include "Rd/DbObjectReader.h"


FdoSmPhOdbcTable::FdoSmPhOdbcTable(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoStringP pkeyName,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdTable(name, pOwner, elementState, pkeyName),
    FdoSmPhOdbcDbObject(name, pOwner, reader),
    FdoSmPhDbObject(name, pOwner, elementState)
{
}

FdoSmPhOdbcTable::~FdoSmPhOdbcTable(void)
{
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhOdbcTable::CreatePkeyReader() const
{
    FdoSmPhOdbcTable* pTable = (FdoSmPhOdbcTable*) this;

    return new FdoSmPhRdOdbcPkeyReader( pTable->GetManager(), FDO_SAFE_ADDREF(pTable) );
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhOdbcTable::CreateFkeyReader() const
{
    FdoSmPhOdbcTable* pTable = (FdoSmPhOdbcTable*) this;

    return new FdoSmPhRdOdbcFkeyReader( pTable->GetManager(), FDO_SAFE_ADDREF(pTable) );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOdbcTable::CreateConstraintReader( FdoString* type ) const
{
	FdoSmPhOdbcOwner* pOdbcOwner = static_cast<FdoSmPhOdbcOwner*>((FdoSmPhDbElement*) GetParent());

    return new FdoSmPhRdOdbcConstraintReader( pOdbcOwner, GetName(), type );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhOdbcTable::CreateIndexReader() const
{
    FdoSmPhOdbcTable* pTable = (FdoSmPhOdbcTable*) this;

    return new FdoSmPhRdOdbcIndexReader( pTable->GetManager(), FDO_SAFE_ADDREF(pTable) );
}

bool FdoSmPhOdbcTable::Add()
{
    FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    // Create table:
    FdoStringP sqlStmt = FdoStringP::Format(
        L"%ls %ls",
        (FdoString*) GetAddSql(),
        (FdoString*) GetAddStorageSql()
    );
    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

    return true;
}

bool FdoSmPhOdbcTable::Modify()
{
    // Nothing to modify yet.
    return true;
}

bool FdoSmPhOdbcTable::Delete()
{
    FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = GetDeleteSql();

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

    return true;
}

FdoSmPhIndexP FdoSmPhOdbcTable::NewIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhOdbcIndex( name, this, isUnique, elementState );
}

FdoSmPhIndexP FdoSmPhOdbcTable::NewSpatialIndex(
    FdoStringP name,
    bool isUnique,
    FdoSchemaElementState elementState
)
{
    throw FdoSchemaException::Create();
    return (FdoSmPhIndex*)NULL;
}

FdoSmPhFkeyP FdoSmPhOdbcTable::NewFkey(
    FdoStringP name,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhOdbcFkey( name, this, pkeyTableName, pkeyTableOwner, elementState );
}

FdoStringP FdoSmPhOdbcTable::GetAddHdgSql()
{
    return FdoStringP::Format( 
        L"create table %ls", 
        (FdoString*) GetDbQName() 
    );
}

FdoStringP FdoSmPhOdbcTable::GetAddStorageSql()
{
    return FdoStringP::mEmptyString;
}


FdoStringP FdoSmPhOdbcTable::GetAddSql()
{
    return FdoSmPhTable::GetAddSql();
}
