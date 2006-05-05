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

struct mysql_context_def;

extern "C" {
    int mysql_run_sql (
        mysql_context_def *context,
        char *sql
    );
}

FdoSmPhMySqlOwner::FdoSmPhMySqlOwner(
    FdoStringP name,
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdOwner(name, hasMetaSchema, pDatabase, elementState)
{
}

FdoSmPhMySqlOwner::~FdoSmPhMySqlOwner(void)
{
}

void FdoSmPhMySqlOwner::SetCurrent()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    rdbi_set_schema( mgr->GetRdbiContext(), (char*)(const char*)FdoStringP(GetName()) );
}

FdoSmPhDbObjectP FdoSmPhMySqlOwner::NewTable(
    FdoStringP tableName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhMySqlTable(tableName, this, elementState, L"", reader);
}

FdoSmPhDbObjectP FdoSmPhMySqlOwner::NewView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
)
{
    return new FdoSmPhMySqlView(viewName, rootDatabase, rootOwner, rootObjectName, this, elementState, reader );
}

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhMySqlOwner::CreateDbObjectReader( FdoStringP dbObject) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlDbObjectReader( FDO_SAFE_ADDREF(pOwner), dbObject );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhMySqlOwner::CreateConstraintReader( FdoStringP constraintName) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlConstraintReader( FDO_SAFE_ADDREF(pOwner), constraintName );
}

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhMySqlOwner::CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlConstraintReader( FDO_SAFE_ADDREF(pOwner), tableName, constraintType );
}

bool FdoSmPhMySqlOwner::Add()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    // TODO: Always latin1 ok for now since data is always utf8.
    // Need to revisit this issue at some point.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"create database %ls character set latin1 collate latin1_bin",
        GetName()
    );

    // Create the owner (datastore)
    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

    if ( GetHasMetaSchema() ) {
        FdoStringsP keywords = FdoStringCollection::Create();
        keywords->Add( rdbi_vndr_name(mgr->GetRdbiContext()) );
        keywords->Add( L"MySQL" );

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
		
		if (GetIsSystem() == false)
			SetOptions();

        // Switch back to default owner
        FdoSmPhOwnerP prevOwner = mgr->FindOwner();
                
        if ( prevOwner )
            prevOwner->SetCurrent();

        //TODO: spatial handling?
    }
    
    return true;
}

bool FdoSmPhMySqlOwner::Delete()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop database if exists %ls",
        GetName()
    );

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

    return true;
}
void FdoSmPhMySqlOwner::SetOptions()
{
	bool validOption;

	// validate LT method
	validOption = (GetLtMode() == NoLtLock || GetLtMode() == FdoMode);
	if (validOption == false)	
		SetLtMode(NoLtLock);
	FdoStringP tempValue = FdoStringP::Format(L"%d", (int)GetLtMode());
	FdoStringP sql_stmt = FdoStringP::Format(L"update f_options set value = '%ls' where name = 'LT_MODE'",
							 (FdoString *) tempValue);

	FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt );

	// validate locking method
	validOption = (GetLckMode() == NoLtLock || GetLckMode() == FdoMode);
	if (validOption == false)	
		SetLckMode(NoLtLock);
	tempValue = FdoStringP::Format(L"%d", (int)GetLckMode());
	sql_stmt = FdoStringP::Format(L"update f_options set value = '%ls' where name ='LOCKING_MODE'",
							(FdoString *) tempValue);

	gdbiConn->ExecuteNonQuery( (const char*) sql_stmt );

}
