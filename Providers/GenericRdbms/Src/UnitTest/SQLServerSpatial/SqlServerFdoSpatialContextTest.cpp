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
#include "SqlServerFdoSpatialContextTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoSpatialContextTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoSpatialContextTest, "FdoSpatialContextTest");

void SqlServerFdoSpatialContextTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoSpatialContextTest::testAdjustExtents( )
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

    try {
        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        UnitTestUtil::CreateDBNoMeta( 
            staticConn->CreateSchemaManager(),
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );


        // open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_SUFFIX,
            0,
            NULL,
            0
		);

		printf( "Creating Spatial Contexts ... \n" );

        // xmax < xmin
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XLT", L"CA-I", 10, 0, 0, 10 ); 

        // ymax < ymin
        UnitTestUtil::CreateSpatialContext( connection, L"SC_YLT", L"CA-I", 0, 100, 100, 0 ); 

        // xmax = xmin

        // Use tolerance as width
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XEQ_TOL", L"CA-I", 5, 0, 5, 10 ); 

        // Use 1 as width
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XEQ_1", L"CA-I", 5, 0, 5, 10, L"", 0.0 ); 

        // Use height as width
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XEQ_HT_POS", L"CA-I", 5, 1, 5, 1.25, L"", 0.0 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XEQ_HT_NEG", L"CA-I", 5, 1, 5, 0.3, L"", 0.0 ); 

        // ymax = ymin

        // Use tolerance as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_YEQ_TOL", L"CA-I", 0, 3, 5, 3 ); 

        // Use 1 as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_YEQ_1", L"CA-I", 0, 25.3, 100, 25.3, L"", 0.0 ); 

        // Use width as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_YEQ_WD_POS", L"CA-I", 5, 1.25, 5.1, 1.25, L"", 0.0 ); 

        // Use width as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_YEQ_WD_NEG", L"CA-I", 5, 1.25, 4.8, 1.25, L"", 0.0 ); 

        // Width and Height 0

        // Use tolerance as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XYEQ_TOL", L"CA-I", 0, 0, 0, 0, L"", 2.1 ); 

        // Use 1 as height
        UnitTestUtil::CreateSpatialContext( connection, L"SC_XYEQ_1", L"CA-I", 0, 5, 0, 5, L"", 0.0 ); 

        connection->Close();
        connection->Open();

        FdoPtr<FdoIGetSpatialContexts> cmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
        cmd->SetActiveOnly(false);

        FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();
        int count = 0;

        while ( reader->ReadNext() ) {
            FdoStringP scName = reader->GetName();
            if ( scName == L"SC_XLT" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 0, 10, 10 );
                count++;
            }
            else if ( scName == L"SC_YLT" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 0, 100, 100 );
                count++;
            }
            else if ( scName == L"SC_XEQ_TOL" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 5, 0, 5.001, 10  );
                count++;
            }
            else if ( scName == L"SC_XEQ_1" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 5, 0, 6, 10 );
                count++;
            }
            else if ( scName == L"SC_XEQ_HT_POS" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 5, 1, 5.25, 1.25 );
                count++;
            }
            else if ( scName == L"SC_XEQ_HT_NEG" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 5, 0.3, 5.7, 1 );
                count++;
            }
            else if ( scName == L"SC_YEQ_TOL" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 3, 5, 3.001 );
                count++;
            }
            else if ( scName == L"SC_YEQ_1" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 25.3, 100, 26.3 );
                count++;
            }
            else if ( scName == L"SC_YEQ_WD_POS" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 5, 1.25, 5.1, 1.35 );
                count++;
            }
            else if ( scName == L"SC_YEQ_WD_NEG" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 4.8, 1.25, 5, 1.45 );
                count++;
            }
            else if ( scName == L"SC_XYEQ_TOL" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 0, 2.1, 2.1 );
                count++;
            }
            else if ( scName == L"SC_XYEQ_1" ) {
                TestCommonMiscUtil::VldExtent( scName, (FdoPtr<FdoByteArray>)reader->GetExtent(), 0, 5, 1, 6 );
                count++;
            }
        }

        CPPUNIT_ASSERT( count == 12 );

        delete staticConn;
        connection->Close();
    }
    catch ( ... )
    {
        try {
            if ( staticConn ) delete staticConn;
            if ( connection ) connection->Close();
        }
        catch ( ... ) {
        }

        throw;
    }
}

void SqlServerFdoSpatialContextTest::testNumericCoordinateSystemNames( )
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

    try {
        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();
        UnitTestUtil::CreateDBNoMeta( 
            staticConn->CreateSchemaManager(),
            UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
        );

        // open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			false,
            DB_NAME_SUFFIX,
            0,
            NULL,
            0
		);

		printf( "Creating Spatial Contexts ... \n" );
        UnitTestUtil::CreateSpatialContext( connection, L"NumCordSysName", L"11", 10, 0, 0, 10 ); 

        connection->Close();
        connection->Open();

        // Check if the coordinate system is created.
		printf( "Cross-checking Spatial Contexts ... \n" );
        FdoPtr<FdoIGetSpatialContexts> cmd = (FdoIGetSpatialContexts*) connection->CreateCommand( FdoCommandType_GetSpatialContexts );
        cmd->SetActiveOnly(false);

        FdoPtr<FdoISpatialContextReader> reader = cmd->Execute();
        bool found = false;

        while ( reader->ReadNext() )
        {
            FdoStringP scName = reader->GetName();
            FdoStringP scCoordSysName = reader->GetCoordinateSystem();
            if ( ( scName == L"NumCordSysName" ) && ( scCoordSysName == L"11" ) )
                found = true;
        }

        // Process cross-check result.
        if ( !found )
            throw FdoException::Create(L"Expected spatial context not found");
        else
		    printf( " ... Expected spatial context found \n" );

        // Close connections.
        delete staticConn;
        connection->Close();

    }
    catch (FdoException *exp)
    {
        try
        {
            if ( staticConn ) delete staticConn;
            if ( connection ) connection->Close();
        }
        catch ( ... )
        {
        }
		UnitTestUtil::FailOnException( exp );
    }
    catch ( ... )
    {
        try
        {
            if ( staticConn ) delete staticConn;
            if ( connection ) connection->Close();
        }
        catch ( ... )
        {
        }

        throw;
    }
}

void SqlServerFdoSpatialContextTest::testDottedTables()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf(" >>> ... creating test database \n");
        if (UnitTestUtil::DatastoreExists(L"_dottedtable"))
            UnitTestUtil::DropDb(L"_dottedtable");

        UnitTestUtil::CreateDB(false, false, L"_dottedtable", 0, false, false);

        // Connect and create the test schema.
        connection = UnitTestUtil::GetConnection(L"_dottedtable");

        // Create our user if it doesn't exist and map it to this database
        printf(" >>> ... creating table \n");

        FdoStringP sql = L"CREATE TABLE dbo.[dotted.table] (\n";
        sql += L"    ID int IDENTITY(1, 1) NOT NULL,\n";
        sql += L"    geom geography NULL\n";
        sql += L");";

        UnitTestUtil::Sql2Db(sql, connection);

        // Insert a geography instance to trigger SRID sampling on this table
        sql = L"INSERT INTO dbo.[dotted.table](geom) VALUES (geography::Point(0, 0, 4326))";
        UnitTestUtil::Sql2Db(sql, connection);

        FdoPtr<FdoIGetSpatialContexts> getSc = static_cast<FdoIGetSpatialContexts*>(connection->CreateCommand(FdoCommandType_GetSpatialContexts));
        FdoPtr<FdoISpatialContextReader> scReader = getSc->Execute();
        FdoInt32 scCount = 0;
        while (scReader->ReadNext())
        {
            scCount++;
        }

        CPPUNIT_ASSERT(scCount == 1);

        //Somewhat bleeding (testing) concerns here, but we need to check that the dotted table can be
        //described and queried from
        FdoPtr<FdoIDescribeSchema> describe = static_cast<FdoIDescribeSchema*>(connection->CreateCommand(FdoCommandType_DescribeSchema));
        describe->SetSchemaName(L"dbo");
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();
        CPPUNIT_ASSERT(schemas->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> theSchema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = theSchema->GetClasses();
        CPPUNIT_ASSERT(classes->GetCount() == 1);

        FdoPtr<FdoClassDefinition> daKlass = classes->GetItem(0);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(connection->CreateCommand(FdoCommandType_Select));
        FdoString* className = daKlass->GetName();
        selectCmd->SetFeatureClassName(className);

        FdoPtr<FdoIFeatureReader> fr = selectCmd->Execute();
        FdoInt32 count = 0;
        while (fr->ReadNext())
        {
            count++;
        }

        CPPUNIT_ASSERT(count == 1);
        
        //Now try qualified
        FdoStringP qClassName = L"dbo:";
        qClassName += className;
        selectCmd->SetFeatureClassName(qClassName);

        fr = selectCmd->Execute();
        count = 0;
        while (fr->ReadNext())
        {
            count++;
        }

        CPPUNIT_ASSERT(count == 1);

        //Finally with explicit property list
        FdoPtr<FdoIdentifierCollection> idents = selectCmd->GetPropertyNames();
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"ID");
        FdoPtr<FdoIdentifier> geom = FdoIdentifier::Create(L"geom");
        idents->Add(id);
        idents->Add(geom);

        fr = selectCmd->Execute();
        count = 0;
        while (fr->ReadNext())
        {
            count++;
        }

        CPPUNIT_ASSERT(count == 1);

        connection->Close();
    }
    catch (FdoException *exp)
    {
        printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
        if (connection)
            connection->Close();
        TestCommonFail(exp);
    }
    catch (...)
    {
        if (connection) connection->Close();
        throw;
    }
}