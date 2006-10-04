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
#include "Owner.h"
#include "Table.h"
#include "View.h"
#include "Mgr.h"
#include "Rd/DbObjectReader.h"
#include "Rd/ConstraintReader.h"
#include "Inc/Rdbi/proto.h"

struct odbcdr_context_def;

extern "C" {
    int odbcdr_run_sql (
        odbcdr_context_def *context,
        char *sql
    );
}

FdoSmPhOdbcOwner::FdoSmPhOdbcOwner(
    FdoStringP name,
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
    FdoSchemaElementState elementState
) :
    FdoSmPhGrdOwner(name, hasMetaSchema, pDatabase, elementState)
{
}

FdoSmPhOdbcOwner::~FdoSmPhOdbcOwner(void)
{
}

void FdoSmPhOdbcOwner::SetCurrent()
{
    FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();

    if ( RDBI_SUCCESS != rdbi_set_schema( mgr->GetRdbiContext(), (char*)(const char*)FdoStringP(GetName()) ))
    {
        rdbi_get_msg(mgr->GetRdbiContext()); 
        throw FdoSchemaException::Create( mgr->GetRdbiContext()->last_error_msg ); 
    }
}

FdoSmPhDbObjectP FdoSmPhOdbcOwner::NewTable(
    FdoStringP tableName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhOdbcTable(tableName, this, elementState, L"", reader);
}

FdoSmPhDbObjectP FdoSmPhOdbcOwner::NewView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhOdbcView(viewName, rootDatabase, rootOwner, rootObjectName, this, elementState, reader);
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhOdbcOwner::CreateDbObjectReader( FdoStringP dbObject) const
{
    FdoSmPhOdbcOwner* pOwner = (FdoSmPhOdbcOwner*) this;

    return new FdoSmPhRdOdbcDbObjectReader( FDO_SAFE_ADDREF(pOwner), dbObject );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOdbcOwner::CreateConstraintReader( FdoStringP constraintName) const
{
    FdoSmPhOdbcOwner* pOwner = (FdoSmPhOdbcOwner*) this;

    return new FdoSmPhRdOdbcConstraintReader( FDO_SAFE_ADDREF(pOwner), constraintName );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhOdbcOwner::CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType) const
{
    FdoSmPhOdbcOwner* pOwner = (FdoSmPhOdbcOwner*) this;

    return new FdoSmPhRdOdbcConstraintReader( FDO_SAFE_ADDREF(pOwner), tableName, constraintType );
}

FdoStringP FdoSmPhOdbcOwner::GetBestSchemaName() const
{
    return ( wcslen(GetName()) > 0 ) ? 
        FdoStringP(GetName())
// TODO: It is currently not possible to bulk copy
// a schema named "Default" to the Shape Provider.
// In the meantime, default the schema name to "Fdo".
//        : L"Default";
        : FdoSmPhMgr::RdSchemaPrefix;
}

bool FdoSmPhOdbcOwner::Add()
{
    FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create database %ls",
        GetName()
    );

    // Create the owner (datastore)
    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

    if ( GetHasMetaSchema() ) {
        FdoStringsP keywords = FdoStringCollection::Create();
        keywords->Add( rdbi_vndr_name(mgr->GetRdbiContext()) );
        keywords->Add( L"SQLServer" );

        // The following keywords cause the long transaction and locking
        // system properties to be added and inherited by each class table.
        if ( GetLtMode() == FdoMode )
            keywords->Add( L"FdoLt" );

        if ( GetLckMode() == FdoMode )
            keywords->Add( L"FdoLock" );

        // Switch to newly created owner and add the MetaSchema

        SetCurrent();

        try {
            AddMetaSchema( keywords, GetIsSystem());
        }
        catch ( ... ) {
            try {
                FdoSmPhOwnerP prevOwner = mgr->FindOwner();
                
                if ( prevOwner )
                    prevOwner->SetCurrent();
            }
            catch (...) {
            }
            throw;
        }

        // Switch back to default owner
        FdoSmPhOwnerP prevOwner = mgr->FindOwner();
                
        if ( prevOwner && FdoStringP(prevOwner->GetName()).GetLength() != 0)
            prevOwner->SetCurrent();

        //TODO: spatial handling?
    }
    
    return true;
}

bool FdoSmPhOdbcOwner::Delete()
{
    FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoSmPhOwnerP currOwner = mgr->FindOwner();

    // Do use master when the datastore to delete is the current one. 
    if ( currOwner && FdoStringP(currOwner->GetName()).ICompare(GetName()) == 0 ) 
        gdbiConn->ExecuteNonQuery( "USE master", TRUE );

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop database %ls",
        GetName()
    );

    gdbiConn->ExecuteNonQuery( (const wchar_t*) sqlStmt, TRUE);

    return true;
}

void FdoSmPhOdbcOwner::SetOptions()
{
	bool validOption;

	// validate LT method
	validOption = (GetLtMode() == NoLtLock || GetLtMode() == FdoMode);
	if (validOption == false)	
		SetLtMode(FdoMode);
	FdoStringP tempValue = FdoStringP::Format(L"%d", (int)GetLtMode());
	FdoStringP sql_stmt = FdoStringP::Format(L"update F_OPTIONS set value = '%ls' where name = 'LT_MODE'",
							 (FdoString *) tempValue);

	FdoSmPhOdbcMgrP mgr = GetManager()->SmartCast<FdoSmPhOdbcMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt );

	// validate locking method
	validOption = (GetLckMode() == NoLtLock || GetLckMode() == FdoMode);
	if (validOption == false)	
		SetLckMode(NoLtLock);
	tempValue = FdoStringP::Format(L"%d", (int)GetLckMode());
	sql_stmt = FdoStringP::Format(L"update F_OPTIONS set value = '%ls' where name ='LOCKING_MODE'",
							(FdoString *) tempValue);

	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt );

}

FdoString* FdoSmPhOdbcOwner::GetDbNameClause(bool isEqual)
{
	FdoStringP schemaCond = FdoStringP::Format(L"schemaname %ls '%ls'",
												isEqual ? L"=" : L"!=",
												GetName());
	return (FdoString*) schemaCond;
}

