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

#include "Pch.h"
#include "SqlServerFdoApplySchemaTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "../../SQLServerSpatial/SchemaMgr/Ph/Owner.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoApplySchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoApplySchemaTest, "FdoApplySchemaTest");

void SqlServerFdoApplySchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
	mIsLowerDatastoreName = true;
}

void SqlServerFdoApplySchemaTest::CreateNLSSchema( FdoIConnection* connection, StaticConnection* staticConn )
{
    FdoApplySchemaTest::CreateNLSSchema( connection, staticConn );

    FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();
    const FdoSmLpSchemaCollection* lp = mgr->RefLogicalPhysicalSchemas();

    const FdoSmLpSchema* pSchema = lp->RefItem(
        FdoStringP::Format(
            L"%c%c%c", 
            UnitTestUtil::GetNlsChar(1),
            UnitTestUtil::GetNlsChar(2),
            UnitTestUtil::GetNlsChar(3)
        )
    );

    const FdoSmLpClassCollection* pClasses = pSchema->RefClasses();

    for ( int i = 0; i < pClasses->GetCount(); i++ ) {
        const FdoSmLpClassDefinition* pClass = pClasses->RefItem(i);
        const FdoSmPhDbObject* pTable = pClass->RefDbObject()->RefDbObject();

        CPPUNIT_ASSERT( (FdoStringP(L"dbo.") + pClass->GetName()).Lower() == pTable->GetName() );

        const FdoSmLpPropertyDefinitionCollection* pProps = pClass->RefProperties();

        for ( int j = 0; j < pProps->GetCount(); j++ ) {
            const FdoSmLpSimplePropertyDefinition* pProp = 
                (FdoSmLpSimplePropertyDefinition*)(pProps->RefItem(j));

            const FdoSmPhColumn* pColumn = pProp->RefColumn();
            if ( (!pProp->GetIsSystem()) && pColumn ) 
                CPPUNIT_ASSERT( FdoStringP(pProp->GetName()).Lower() == pColumn->GetName() );
        }
    }
}

void SqlServerFdoApplySchemaTest::DeletePhSystemSchemas( StaticConnection* staticConn )
{
    DeletePhSystemSchema( staticConn, L"INFORMATION_SCHEMA" );
    DeletePhSystemSchema( staticConn, L"guest" );
    DeletePhSystemSchema( staticConn, L"db_datareader" );
}

void SqlServerFdoApplySchemaTest::DeletePhSystemSchema( StaticConnection* staticConn, FdoStringP schemaName )
{
    bool succeeded = false;
    FdoSchemaManagerP sm = staticConn->CreateSchemaManager();
    FdoSmPhMgrP phMgr = sm->GetPhysicalSchema();

    FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_NO_META_SUFFIX);
    FdoSmPhSqsOwnerP owner = phMgr->FindOwner(datastore)->SmartCast<FdoSmPhSqsOwner>();

    FdoSmPhSqsSchemaP schema = owner->GetSchemas()->FindItem( schemaName );
    CPPUNIT_ASSERT( schema );

    try {
        schema->SetElementState( FdoSchemaElementState_Deleted );
        owner->Commit();
        succeeded = true;
    }
	catch ( FdoSchemaException* e )
	{
        FdoStringP expected = FdoStringP::Format( L"Cannot delete system schema '%ls' from datastore.", (FdoString*) schemaName );
        FdoString* pMessage = e->GetExceptionMessage();
#ifdef _WIN32
	    CPPUNIT_ASSERT( pMessage && wcscmp(pMessage, expected) == 0);
#else
        CPPUNIT_ASSERT( pMessage && wcscmp(pMessage, expected) == 0);
#endif
        FDO_SAFE_RELEASE(e);
	}

    if ( succeeded ) {
        FdoStringP msg = FdoStringP::Format( L"Deleting system schema '%ls' was supposed to fail", (FdoString*) schemaName );
		CPPUNIT_FAIL( (const char*) msg );
    }
}

void SqlServerFdoApplySchemaTest::CreatePhSystemSchemas( FdoIConnection* connection )
{
    CreatePhSystemSchema( connection, L"INFORMATION_SCHEMA" );
    CreatePhSystemSchema( connection, L"guest" );
    CreatePhSystemSchema( connection, L"db_datareader" );
}

void SqlServerFdoApplySchemaTest::CreatePhSystemSchema( FdoIConnection* connection, FdoStringP schemaName )
{
    bool succeeded = false;
	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( schemaName, L"" );
    pCmd->SetFeatureSchema( pSchema );

    try {
        pCmd->Execute();    
    }
	catch ( FdoSchemaException* e )
	{
        FdoStringP expected = FdoStringP::Format( L"Cannot add schema %ls to database; it already exists ", (FdoString*) schemaName );
        FdoPtr<FdoException> cause = e->GetRootCause();
        CPPUNIT_ASSERT( cause );
#ifdef _DEBUG
        FdoString* pMessage = wcschr( cause->GetExceptionMessage(), ')' );
        if (pMessage) pMessage += 2;
#else
        FdoString* pMessage = cause->GetExceptionMessage();
#endif
#ifdef _WIN32
	    CPPUNIT_ASSERT( pMessage && wcscmp(pMessage, expected) == 0);
#else
        CPPUNIT_ASSERT( pMessage && wcscmp(pMessage, expected) == 0);
#endif
        FDO_SAFE_RELEASE(e);
	}

    if ( succeeded ) {
        FdoStringP msg = FdoStringP::Format( L"Creating system schema '%ls' was supposed to fail", (FdoString*) schemaName );
		CPPUNIT_FAIL( (const char*) msg );
    }
}

FdoRdbmsOvPhysicalSchemaMapping* SqlServerFdoApplySchemaTest::CreateOverrideDefaults( FdoIConnection* connection, int passNum )
{ 
	FdoRdbmsOvPhysicalSchemaMapping* pOverrides = FdoApplySchemaTest::CreateOverrideDefaults( connection, passNum );

	FdoSqlServerOvPhysicalSchemaMapping* pSqsOverrides = static_cast<FdoSqlServerOvPhysicalSchemaMapping*>(pOverrides);
	pSqsOverrides->SetTableFilegroup( L"TestFileGroup1" );
	pSqsOverrides->SetIndexFilegroup( L"TestFileGroup1" );
	pSqsOverrides->SetTextInRow(SqlServerOvTextInRowOption_InRow);

    return pOverrides;
}

void SqlServerFdoApplySchemaTest::CreateRdbmsSpecificElements(FdoIConnection* connection, FdoString* wDatastore)
{
    // Create a filegroup containing one file, for testing purposes:
    FdoPtr<FdoISQLCommand> sqlCommand = (FdoISQLCommand*)connection->CreateCommand(FdoCommandType_SQLCommand);

    const char* mbSqlServerRootPath = UnitTestUtil::GetEnv("sqlserver_data_root", "C:\\Program Files\\Microsoft SQL Server\\MSSQL\\data");

	sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILEGROUP TestFileGroup1", wDatastore));
    sqlCommand->ExecuteNonQuery();
	try
	{
    	sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILE (NAME=TestFile1, FILENAME='%hs\\%ls_file1.ndf', SIZE=2MB, MAXSIZE=100MB, FILEGROWTH=5MB) TO FILEGROUP TestFileGroup1", wDatastore, mbSqlServerRootPath, wDatastore));
		sqlCommand->ExecuteNonQuery();
	}
	catch (FdoException* ex)
	{
        FDO_SAFE_RELEASE(ex);
		sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILE (NAME=TestFile1, FILENAME='C:\\Program Files\\Microsoft SQL Server\\MSSQL.1\\MSSQL\\data\\%ls_file1.ndf', SIZE=2MB, MAXSIZE=100MB, FILEGROWTH=5MB) TO FILEGROUP TestFileGroup1", wDatastore, wDatastore));
		sqlCommand->ExecuteNonQuery();
	}

	sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILEGROUP TestFileGroup2", wDatastore));
    sqlCommand->ExecuteNonQuery();
	try
	{
    	sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILE (NAME=TestFile2, FILENAME='%hs\\%ls_file2.ndf', SIZE=2MB, MAXSIZE=100MB, FILEGROWTH=5MB) TO FILEGROUP TestFileGroup2", wDatastore, mbSqlServerRootPath, wDatastore));
		sqlCommand->ExecuteNonQuery();
	}
	catch (FdoException* ex)
	{
        FDO_SAFE_RELEASE(ex);
		sqlCommand->SetSQLStatement(FdoStringP::Format(L"ALTER DATABASE \"%ls\" ADD FILE (NAME=TestFile2, FILENAME='C:\\Program Files\\Microsoft SQL Server\\MSSQL.1\\MSSQL\\data\\%ls_file2.ndf', SIZE=2MB, MAXSIZE=100MB, FILEGROWTH=5MB) TO FILEGROUP TestFileGroup2", wDatastore, wDatastore));
		sqlCommand->ExecuteNonQuery();
    }
}

FdoRdbmsOvClassDefinition* SqlServerFdoApplySchemaTest::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoSqlServerOvClassDefinition::Create();
    else
    {
        FdoPtr<FdoSqlServerOvClassDefinition> classOv = FdoSqlServerOvClassDefinition::Create(name);

        if (0==wcscmp(name, L"OvClassA"))
        {
            classOv->SetIdentityPropertyName(L"FeatId");
            classOv->SetIdentitySeed(100);
            classOv->SetIdentityIncrement(5);
        }

        return FDO_SAFE_ADDREF(classOv.p);
    }
}

FdoRdbmsOvTable* SqlServerFdoApplySchemaTest::CreateOvTable(FdoString* name)
{
    FdoSqlServerOvTable* table = FdoSqlServerOvTable::Create(name);

    // Set RDBMS-specific overrides, for some of the tables:
    if (0==wcscmp(name, L"ov_table_c"))
    {
        table->SetTableFilegroup(L"TestFileGroup2");
        table->SetTextFilegroup (L"TestFileGroup2");
        table->SetIndexFilegroup(L"TestFileGroup2");
        table->SetTextInRow(SqlServerOvTextInRowOption_InRow);
    }

    if (0==wcscmp(name, L"oneforeign"))
    {
        table->SetOwner( UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX) );
    }

    return table;
}

FdoRdbmsOvColumn* SqlServerFdoApplySchemaTest::CreateOvColumn(FdoString* name)
{
    FdoPtr<FdoSqlServerOvColumn> ovCol = FdoSqlServerOvColumn::Create(name);

    if (0==wcscmp(name, L"ov_col_D"))
    {
        ovCol->SetFormula(L"substring(\"ov_col_B\",1,1)");
    }

    return FDO_SAFE_ADDREF(ovCol.p);
}

bool SqlServerFdoApplySchemaTest::CanApplyWithoutMetaSchema()
{
    return true;
}

bool SqlServerFdoApplySchemaTest::CanCreateSchemaWithoutMetaSchema()
{
    return true;
}

bool SqlServerFdoApplySchemaTest::CanAddNotNullCol()
{
    // SqlServer does not allow not null columns to be added to existing tables,
    // even if they are empty.
    return false;
}

bool SqlServerFdoApplySchemaTest::CanDropCol()
{
    // Workaround for SQL Server 2008 problem where "alter table ... drop column ..."
    // stops the server.
    return true;
}



FdoStringP SqlServerFdoApplySchemaTest::LogicalPhysicalBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile );

    _logicalPhysicalBend( inFile, outFile, L"SQLServerSpatial" );

    return outFile;
}

FdoStringP SqlServerFdoApplySchemaTest::LogicalPhysicalFormat( FdoString* inFile )
{
    FdoStringP outFile = FdoStringP(inFile).Replace( L".", L"_LPhF." );

    _logicalPhysicalFormat( inFile, outFile, L"SQLServerSpatial" );

    return outFile;
}

FdoStringP SqlServerFdoApplySchemaTest::SchemaTestErrFile( int fileNum, bool isMaster ) 
{
    if ( isMaster ) {
        if ( (fileNum == 2) || (fileNum == 5) || (fileNum == 8) || (fileNum == 9) ) 
            return L"";
		return FdoStringP::Format( L"apply_schema_err%d%ls%ls.txt", fileNum,
			((fileNum == 3) || (fileNum == 6)) ? L"S" : L"", L"_master");
    }
    return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_err%d%ls.txt", fileNum,
        ((fileNum == 3) || (fileNum == 6)) ? L"S" : L"" ) );
}

FdoStringP SqlServerFdoApplySchemaTest::SchemaOvErrFile( int fileNum, bool isMaster )
{
	if (isMaster)
		return FdoStringP::Format( L"apply_schema_overrides_err%dSP%ls.txt", fileNum, L"_master");
	else
		return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_overrides_err%dSP.txt", fileNum) );
}

FdoStringP SqlServerFdoApplySchemaTest::GetValueColumnName()
{
	return L"Value";
}

FdoStringP SqlServerFdoApplySchemaTest::GetDefaultSchemaName()
{
	return L"dbo";
}

