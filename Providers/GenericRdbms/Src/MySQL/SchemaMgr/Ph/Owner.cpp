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
#include "Rd/ColumnReader.h"
#include "Rd/ConstraintReader.h"
#include "Rd/FkeyReader.h"
#include "Rd/IndexReader.h"
#include "Rd/PkeyReader.h"
#include "Inc/Rdbi/proto.h"

struct mysql_context_def;

extern "C" {
    int mysql_run_sql (
        mysql_context_def *context,
        char *sql
    );
}

FdoInt32 FdoSmPhMySqlOwner::mTempTableNum = 0;
FdoCommonThreadMutex FdoSmPhMySqlOwner::mMutex;

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

FdoStringP FdoSmPhMySqlOwner::GetKeyColumnUsageTable()
{
    if ( mKeyColumnUsageTable == L"" ) {
        // Temporary table not created yet

        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

        // Generate unique name for temporary table
        mKeyColumnUsageTable = FdoStringP::Format( L"fdo_%ls_key_column_usage%d", GetName(), NextTempTableNum() );

        // It shouldn't already exist but drop it just in case it does.
        DropTempTable( mKeyColumnUsageTable );

        // Create the temporary table from the key_column_usage table.
        // Make all string fields case sensitive
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"create temporary table %ls.%ls ( "
            L" constraint_schema varchar(64) not null collate utf8_bin,"
            L" constraint_name varchar(64) not null collate utf8_bin,"
            L" table_schema varchar(64) not null collate utf8_bin,"
            L" table_name varchar(64) not null collate utf8_bin,"
            L" column_name varchar(64) not null collate utf8_bin,"
            L" ordinal_position bigint not null,"
            L" referenced_table_schema varchar(64) null collate utf8_bin,"
            L" referenced_table_name varchar(64) null collate utf8_bin,"
            L" referenced_column_name varchar(64) null collate utf8_bin,"
            L" primary key ( table_name, constraint_schema, constraint_name, ordinal_position )"
            L")",
            GetName(),
            (FdoString*) mKeyColumnUsageTable
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

        // Populate the temporary table from key_column_usage. Just need rows for this owner.
        sqlStmt = FdoStringP::Format( 
            L"insert into  %ls.%ls ( "
            L" constraint_schema,"
            L" constraint_name,"
            L" table_schema,"
            L" table_name,"
            L" column_name,"
            L" ordinal_position,"
            L" referenced_table_schema,"
            L" referenced_table_name,"
            L" referenced_column_name"
            L")"
            L" select "
            L" constraint_schema,"
            L" constraint_name,"
            L" table_schema,"
            L" table_name,"
            L" column_name,"
            L" ordinal_position,"
            L" referenced_table_schema,"
            L" referenced_table_name,"
            L" referenced_column_name"
            L" from information_schema.key_column_usage where table_catalog is null and table_schema = %ls",
            GetName(),
            (FdoString*) mKeyColumnUsageTable,
            (FdoString*) GetManager()->FormatSQLVal( GetName(), FdoSmPhColType_String )
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }

    return FdoStringP(L"\"") + GetName() + L"\".\"" + mKeyColumnUsageTable + L"\"";
}

FdoStringP FdoSmPhMySqlOwner::GetTableConstraintsTable()
{
    if ( mTableConstraintsTable == L"" ) {
        // Temporary table not created yet

        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

        // Generate unique name for temporary table
        mTableConstraintsTable = FdoStringP::Format( L"fdo_%ls_table_constraints%d", GetName(), NextTempTableNum() );

        // It shouldn't already exist but drop it just in case it does.
        DropTempTable( mTableConstraintsTable );

        // Create the temporary table from the table_constraints table.
        // Make all string fields case sensitive
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"create temporary table %ls.%ls ( "
            L" constraint_schema varchar(64) not null collate utf8_bin,"
            L" constraint_name varchar(64) not null collate utf8_bin,"
            L" table_schema varchar(64) not null collate utf8_bin,"
            L" table_name varchar(64) not null collate utf8_bin,"
            L" constraint_type varchar(64) not null collate utf8_bin,"
            L" primary key ( table_name, constraint_schema, constraint_name )"
            L")",
            GetName(),
            (FdoString*) mTableConstraintsTable
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

        // Populate the temporary table from table_constraints. Just need rows for this owner.
        sqlStmt = FdoStringP::Format( 
            L"insert into  %ls.%ls ( "
            L" constraint_schema,"
            L" constraint_name,"
            L" table_schema,"
            L" table_name,"
            L" constraint_type"
            L")"
            L" select "
            L" constraint_schema,"
            L" constraint_name,"
            L" table_schema,"
            L" table_name,"
            L" constraint_type"
            L" from information_schema.table_constraints where constraint_catalog is null and table_schema = %ls",
            GetName(),
            (FdoString*) mTableConstraintsTable,
            (FdoString*) GetManager()->FormatSQLVal( GetName(), FdoSmPhColType_String )
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }

    return FdoStringP(L"\"") + GetName() + L"\".\"" + mTableConstraintsTable + L"\"";
}

FdoStringP FdoSmPhMySqlOwner::GetTablesTable(bool createTemp)
{
    FdoStringP tablesTable;

    if ( createTemp && (mTablesTable == L"") ) {
        // Temporary table not created yet

        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

        // Generate unique name for temporary table
        mTablesTable = FdoStringP::Format( L"fdo_%ls_tables%d", GetName(), NextTempTableNum() );

        // It shouldn't already exist but drop it just in case it does.
        DropTempTable( mTablesTable );

        // Create the temporary table from the tables table.
        // Make all string fields case sensitive
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"create temporary table %ls.%ls ( "
            L" table_schema varchar(64) not null collate utf8_bin,"
            L" table_name varchar(64) not null collate utf8_bin,"
            L" table_type varchar(64) not null collate utf8_bin,"
            L" engine varchar(64) null collate utf8_bin,"
            L" auto_increment bigint null,"
            L" primary key ( table_name )"
            L")",
            GetName(),
            (FdoString*) mTablesTable
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

        // Populate the temporary table from information_schema.tables. Just need rows for this owner.
        sqlStmt = FdoStringP::Format( 
            L"insert into  %ls.%ls ( "
            L" table_schema,"
            L" table_name,"
            L" table_type,"
            L" engine,"
            L" auto_increment"
            L")"
            L" select "
            L" table_schema,"
            L" table_name,"
            L" table_type,"
            L" engine,"
            L" auto_increment"
            L" from information_schema.tables where table_catalog is null and table_schema = %ls",
            GetName(),
            (FdoString*) mTablesTable,
            (FdoString*) GetManager()->FormatSQLVal( GetName(), FdoSmPhColType_String )
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }

    if ( mTablesTable == L"" ) 
        return "information_schema.tables";
    else
        return FdoStringP(L"\"") + GetName() + L"\".\"" + mTablesTable + L"\"";
}

FdoStringP FdoSmPhMySqlOwner::GetColumnsTable(bool createTemp)
{
    FdoStringP tablesTable;

    if ( createTemp && (mColumnsTable == L"") ) {
        // Temporary table not created yet

        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

        // Generate unique name for temporary table
        mColumnsTable = FdoStringP::Format( L"fdo_%ls_columns%d", GetName(), NextTempTableNum() );

        // It shouldn't already exist but drop it just in case it does.
        DropTempTable( mColumnsTable );

        // Create the temporary table from the tables table.
        // Make all string fields case sensitive
        FdoStringP sqlStmt = FdoStringP::Format( 
            L"create temporary table %ls.%ls ( "
            L" table_schema varchar(64) not null collate utf8_bin,"
            L" table_name varchar(64) not null collate utf8_bin,"
            L" column_name varchar(64) not null collate utf8_bin,"
            L" ordinal_position bigint not null,"
            L" is_nullable varchar(3) not null collate utf8_bin,"
            L" data_type varchar(64) not null collate utf8_bin,"
            L" character_octet_length bigint null,"
            L" numeric_precision bigint null,"
            L" numeric_scale bigint null,"
            L" column_type longtext not null,"
            L" extra varchar(20) not null collate utf8_bin,"
            L" primary key ( table_name, ordinal_position )"
            L")",
            GetName(),
            (FdoString*) mColumnsTable
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );

        // Populate the temporary table from information_schema.columns. Just need rows for this owner.
        sqlStmt = FdoStringP::Format( 
            L"insert into  %ls.%ls ( "
            L" table_schema,"
            L" table_name,"
            L" column_name,"
            L" ordinal_position,"
            L" is_nullable,"
            L" data_type,"
            L" character_octet_length,"
            L" numeric_precision,"
            L" numeric_scale,"
            L" column_type,"
            L" extra"
            L")"
            L" select "
            L" table_schema,"
            L" table_name,"
            L" column_name,"
            L" ordinal_position,"
            L" is_nullable,"
            L" data_type,"
            L" character_octet_length,"
            L" numeric_precision,"
            L" numeric_scale,"
            L" column_type,"
            L" extra"
            L" from information_schema.columns where table_catalog is null and table_schema = %ls",
            GetName(),
            (FdoString*) mColumnsTable,
            (FdoString*) GetManager()->FormatSQLVal( GetName(), FdoSmPhColType_String )
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }

    if ( mColumnsTable == L"" ) 
        return "information_schema.columns";
    else
        return FdoStringP(L"\"") + GetName() + L"\".\"" + mColumnsTable + L"\"";
}

void FdoSmPhMySqlOwner::DropTempTables()
{
    DropKeyColumnUsageTable();
    DropTableConstraintsTable();
    DropTablesTable();
    DropColumnsTable();
}

void FdoSmPhMySqlOwner::DropKeyColumnUsageTable()
{
    DropTempTable( mKeyColumnUsageTable );

    mKeyColumnUsageTable = L"";
}

void FdoSmPhMySqlOwner::DropTableConstraintsTable()
{
    DropTempTable(  mTableConstraintsTable );

    mTableConstraintsTable = L"";
}

void FdoSmPhMySqlOwner::DropTablesTable()
{
    DropTempTable( mTablesTable );

    mTablesTable = L"";
}

void FdoSmPhMySqlOwner::DropColumnsTable()
{
    DropTempTable( mColumnsTable );

    mColumnsTable = L"";
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

FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhMySqlOwner::CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlDbObjectReader( FDO_SAFE_ADDREF(pOwner), join );
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

FdoPtr<FdoSmPhRdConstraintReader> FdoSmPhMySqlOwner::CreateConstraintReader( FdoSmPhRdTableJoinP join , FdoStringP constraintType) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlConstraintReader( FDO_SAFE_ADDREF(pOwner), join, constraintType );
}

FdoPtr<FdoSmPhRdFkeyReader> FdoSmPhMySqlOwner::CreateFkeyReader() const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlFkeyReader( pOwner->GetManager(), FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdIndexReader> FdoSmPhMySqlOwner::CreateIndexReader() const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlIndexReader( pOwner->GetManager(), FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhMySqlOwner::CreatePkeyReader() const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlPkeyReader( pOwner->GetManager(), FDO_SAFE_ADDREF(pOwner) );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhMySqlOwner::CreateColumnReader() const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlColumnReader( FDO_SAFE_ADDREF(pOwner), (FdoSmPhRdTableJoin*)NULL );
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhMySqlOwner::CreateColumnReader( FdoSmPhRdTableJoinP join ) const
{
    FdoSmPhMySqlOwner* pOwner = (FdoSmPhMySqlOwner*) this;

    return new FdoSmPhRdMySqlColumnReader( FDO_SAFE_ADDREF(pOwner), join );
}

bool FdoSmPhMySqlOwner::Add()
{
    FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    // TODO: Always latin1 ok for now since data is always utf8.
    // Need to revisit this issue at some point.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"create database %ls character set latin1 collate latin1_bin",
        (FdoString*) GetDbName()
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
        (FdoString*) GetDbName()
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

void FdoSmPhMySqlOwner::OnAfterCommit()
{
    // Notify base about commit event.
    FdoSmPhOwner::OnAfterCommit();

    // After physical schema modification,drop temporary tables so they will be regenerated
    // with new snapshot of information_schema.
    // This also prevents temporary tables from accumulating when successive ApplySchema commands
    // are executed.
    DropTempTables();
}

FdoInt32 FdoSmPhMySqlOwner::NextTempTableNum()
{
    // Generate unique number in threadsafe manner
    mMutex.Enter();
    mTempTableNum++;
    mMutex.Leave();

    return mTempTableNum;
}
    
void FdoSmPhMySqlOwner::DropTempTable( FdoStringP tableName)
{
    if ( tableName != L"" ) {
        FdoSmPhMySqlMgrP mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
        GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

        FdoStringP sqlStmt = FdoStringP::Format( 
            L"drop temporary table if exists %ls.%ls",
            GetName(),
            (FdoString*) tableName
            );

        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
    }
}
FdoString* FdoSmPhMySqlOwner::GetDbNameClause(bool isEqual)
{
	if (isEqual)
		return L"schemaname = database()";
	else
		return L"schemaname != database()";
}
