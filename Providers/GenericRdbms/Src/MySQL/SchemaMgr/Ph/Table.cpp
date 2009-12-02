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
#include "Rd/ConstraintReader.h"

FdoSmPhMySqlTable::FdoSmPhMySqlTable(
    FdoStringP name,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoStringP pkeyName,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdTable(name, pOwner, elementState, pkeyName),
    FdoSmPhMySqlDbObject(name, pOwner, reader),
    FdoSmPhDbObject(name, pOwner, elementState)
{
}

FdoSmPhMySqlTable::~FdoSmPhMySqlTable(void)
{
}

const FdoLockType* FdoSmPhMySqlTable::GetLockTypes(FdoInt32& size) const
{
    // Most types of MySql tables do not support transactions and thus
    // do not support any type of locking.
    size = 0;
    return (FdoLockType*) NULL;
    // TODO call base GetLockTypes when storage engine is InnoDB or BDB 
    // (these engines do support transactions).
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhMySqlTable::CreateConstraintReader( FdoString* type ) const
{
	FdoSmPhMySqlOwner* pMySqlOwner = static_cast<FdoSmPhMySqlOwner*>((FdoSmPhDbElement*) GetParent());

    return new FdoSmPhRdMySqlConstraintReader( pMySqlOwner, GetName(), type );
}

bool FdoSmPhMySqlTable::Add()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"%ls %ls",
        (FdoString*) GetAddSql(),
        (FdoString*) GetAddStorageSql()
    );


    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

bool FdoSmPhMySqlTable::Modify()
{
    // Nothing to modify yet.
    return true;
}

bool FdoSmPhMySqlTable::Delete()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = GetDeleteSql();

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

FdoSmPhFkeyP FdoSmPhMySqlTable::NewFkey(
    FdoStringP name,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
)
{
    return new FdoSmPhMySqlFkey( name, this, pkeyTableName, pkeyTableOwner, elementState );
}

FdoStringP FdoSmPhMySqlTable::GetAddStorageSql()
{
    FdoStringP storageSql;

    // Validate that the storage engine is acceptable for table creation:
    if (   (GetStorageEngine() == MySQLOvStorageEngineType_Memory)
        || (GetStorageEngine() == MySQLOvStorageEngineType_Archive)
        || (GetStorageEngine() == MySQLOvStorageEngineType_Example)
        )
        throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_436, "The MySQL storage engine '%1$ls' is not supported for creation of new tables", StorageEngineEnumToString(GetStorageEngine())));

    // Always specify the storage engine, since our default (MyISAM) differs from MySQL's default (InnoDB):
    storageSql += FdoStringP::Format(L" ENGINE=%ls", StorageEngineEnumToString(GetStorageEngine()));

    if (GetAutoIncrementColumnSeed() != 1)
        storageSql += FdoStringP::Format(L" AUTO_INCREMENT=%I64d", GetAutoIncrementColumnSeed());

    if (wcslen(GetDataDirectory()) > 0)
        storageSql += FdoStringP::Format(L" DATA DIRECTORY='%ls'", GetDataDirectory());

    if (wcslen(GetIndexDirectory()) > 0)
        storageSql += FdoStringP::Format(L" INDEX DIRECTORY='%ls'", GetIndexDirectory());

    return storageSql;
}

FdoStringP FdoSmPhMySqlTable::GetDropConstraintSql(FdoStringP constraintName )
{
    return FdoStringP::Format( 
        L"alter table %ls drop index %ls", 
        (FdoString*) GetDDLQName(),
		(FdoString *)GetConstraintDDLName(constraintName)
    );
}
