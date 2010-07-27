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
#include "MySqlFdoApplySchemaTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoApplySchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoApplySchemaTest, "FdoApplySchemaTest");

void MySqlFdoApplySchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
	mIsLowerDatastoreName = true;
}

FdoRdbmsOvPhysicalSchemaMapping* MySqlFdoApplySchemaTest::CreateOverrideDefaults( FdoIConnection* connection, int passNum )
{ 
	FdoRdbmsOvPhysicalSchemaMapping* pOverrides = FdoApplySchemaTest::CreateOverrideDefaults( connection, passNum );

	FdoMySQLOvPhysicalSchemaMapping* pMySqlOverrides = static_cast<FdoMySQLOvPhysicalSchemaMapping*>(pOverrides);
	pMySqlOverrides->SetDataDirectory( L"c:\\\\" );  //TODO: remove doubled backslashes once MySQL Provider properly handles literal strings containing backslashes.
	pMySqlOverrides->SetIndexDirectory( L"c:\\\\" );  //TODO: remove doubled backslashes once MySQL Provider properly handles literal strings containing backslashes.
	pMySqlOverrides->SetStorageEngine( MySQLOvStorageEngineType_MyISAM );

    return pOverrides;
}

void MySqlFdoApplySchemaTest::CreateRdbmsSpecificElements(FdoIConnection* connection, FdoString* wDatastore)
{
    // nothing here yet
}

FdoRdbmsOvClassDefinition* MySqlFdoApplySchemaTest::CreateOvClassDefinition(FdoString *name)
{
    if (name == NULL)
        return FdoMySQLOvClassDefinition::Create();
    else
    {
        FdoPtr<FdoMySQLOvClassDefinition> classOv = FdoMySQLOvClassDefinition::Create(name);

        if (0==wcscmp(name, L"OvClassA"))
        {
            classOv->SetAutoIncrementPropertyName(L"FeatId");
            classOv->SetAutoIncrementSeed(100);
        }

        return FDO_SAFE_ADDREF(classOv.p);
    }
}

FdoRdbmsOvTable* MySqlFdoApplySchemaTest::CreateOvTable(FdoString* name)
{
    FdoMySQLOvTable* table = FdoMySQLOvTable::Create(name);

    // Set RDBMS-specific overrides, for some of the tables:
    if (0==wcscmp(name, L"ov_table_c"))
    {
// Assume the client and server are running on the same machine.
#ifdef _WIN32
        table->SetDataDirectory(L"d:\\\\");  //NOTE: need to double-up the back-slashes to please MySQL
        table->SetIndexDirectory(L"d:\\\\");  //NOTE: need to double-up the back-slashes to please MySQL
#else
        char mysqlDir[200];
        sprintf( mysqlDir, "/opt/mysql_data/%ls/", (FdoString*)  mDatastore );
        mkdir( mysqlDir, ACCESSPERMS | S_IWGRP | S_IWOTH );
        // mkdir does not set group and other write permissions so need
        // the following chmod.
        chmod( mysqlDir, ACCESSPERMS );
        table->SetDataDirectory(FdoStringP(L"/opt/mysql_data/") + mDatastore + L"/");
        table->SetIndexDirectory(FdoStringP(L"/opt/mysql_data/") + mDatastore + L"/");
#endif
        //TODO: most (all?) tables in the test have geometry columns, which only work on MyISAM engine,
        // so overriding the engine will cause errors during table creation:
        //table->SetStorageEngine(MySQLOvStorageEngineType_InnoDB);
    }

    if (0==wcscmp(name, L"oneforeign"))
    {
        table->SetDatabase( UnitTestUtil::GetEnviron("datastore", DB_NAME_OVERRIDE_SUFFIX) );
    }

    return table;
}

void MySqlFdoApplySchemaTest::VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass )
{
    // The following verifies that the AcDbEntity class capabilities are valid.
    FdoClassCapabilitiesP cc = pClass->GetCapabilities();
    CPPUNIT_ASSERT( cc != NULL );
    CPPUNIT_ASSERT( !(cc->SupportsLocking()) );
    CPPUNIT_ASSERT( !(cc->SupportsLongTransactions()) );

    FdoInt32 lockTypeCount;
    FdoLockType* lockTypes = cc->GetLockTypes( lockTypeCount );

    CPPUNIT_ASSERT( lockTypeCount == 0 );

    FdoPtr<FdoPropertyDefinitionCollection> props = pClass->GetProperties();
    for ( FdoInt32 i = 0; i < props->GetCount(); i++ )
    {
        FdoPtr<FdoPropertyDefinition> prop = props->GetItem(i);
        if (prop->GetPropertyType() == FdoPropertyType_GeometricProperty)
        {
            CPPUNIT_ASSERT( cc->GetPolygonVertexOrderRule(prop->GetName()) == FdoPolygonVertexOrderRule_CCW );
            CPPUNIT_ASSERT( !cc->GetPolygonVertexOrderStrictness(prop->GetName()) );
        }
    }
}

FdoStringP MySqlFdoApplySchemaTest::LogicalPhysicalBend( FdoString* inFile )
{
    FdoStringP outFile = UnitTestUtil::GetOutputFileName( inFile );

    _logicalPhysicalBend( inFile, outFile, L"MySql" );

    return outFile;
}

FdoStringP MySqlFdoApplySchemaTest::LogicalPhysicalFormat( FdoString* inFile )
{
    FdoStringP outFile = FdoStringP(inFile).Replace( L".", L"_LPhF." );

    _logicalPhysicalFormat( inFile, outFile, "MySql" );

    return outFile;
}

FdoStringP MySqlFdoApplySchemaTest::SchemaTestErrFile( int fileNum, bool isMaster ) 
{
    if ( isMaster ) {
        if ( (fileNum == 3) || (fileNum == 6) || (fileNum == 9) ) 
            return L"";
		return FdoStringP::Format( L"apply_schema_err%d%ls%ls.txt", fileNum,
			(fileNum == 2 || fileNum == 5 || fileNum == 8) ? L"M" : L"", L"_master");
    }
    return UnitTestUtil::GetOutputFileName( FdoStringP::Format( L"apply_schema_err%d%ls.txt",fileNum,
        (fileNum == 2 || fileNum == 3 || fileNum == 5 || fileNum == 6 || fileNum == 8 || fileNum == 9) ? L"M" : L"") );
}

FdoStringP MySqlFdoApplySchemaTest::GetParcelFirstName()
{
	return L"first_name";
}

FdoStringP MySqlFdoApplySchemaTest::GetParcelLastName()
{
	return L"last_name";
}

