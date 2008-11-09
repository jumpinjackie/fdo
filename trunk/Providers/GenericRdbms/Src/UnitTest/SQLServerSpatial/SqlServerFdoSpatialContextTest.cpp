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
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 0, 10, 10 );
                count++;
            }
            else if ( scName == L"SC_YLT" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 0, 100, 100 );
                count++;
            }
            else if ( scName == L"SC_XEQ_TOL" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 5, 0, 5.001, 10  );
                count++;
            }
            else if ( scName == L"SC_XEQ_1" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 5, 0, 6, 10 );
                count++;
            }
            else if ( scName == L"SC_XEQ_HT_POS" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 5, 1, 5.25, 1.25 );
                count++;
            }
            else if ( scName == L"SC_XEQ_HT_NEG" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 5, 0.3, 5.7, 1 );
                count++;
            }
            else if ( scName == L"SC_YEQ_TOL" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 3, 5, 3.001 );
                count++;
            }
            else if ( scName == L"SC_YEQ_1" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 25.3, 100, 26.3 );
                count++;
            }
            else if ( scName == L"SC_YEQ_WD_POS" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 5, 1.25, 5.1, 1.35 );
                count++;
            }
            else if ( scName == L"SC_YEQ_WD_NEG" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 4.8, 1.25, 5, 1.45 );
                count++;
            }
            else if ( scName == L"SC_XYEQ_TOL" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 0, 2.1, 2.1 );
                count++;
            }
            else if ( scName == L"SC_XYEQ_1" ) {
                VldAdjustedExtent( scName, reader->GetExtent(), 0, 5, 1, 6 );
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

void SqlServerFdoSpatialContextTest::VldAdjustedExtent( FdoStringP scName, FdoByteArray* ba, double minx, double miny, double maxx, double maxy )
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(ba);
    FdoPtr<FdoIEnvelope> env = geom->GetEnvelope();

    char message[1000];

    sprintf( message, "Wrong minx for %ls", (FdoString*) scName );
    CPPUNIT_ASSERT_MESSAGE( message, minx == env->GetMinX() );

    sprintf( message, "Wrong miny for %ls", (FdoString*) scName );
    CPPUNIT_ASSERT_MESSAGE( message, miny == env->GetMinY() );

    sprintf( message, "Wrong maxx for %ls", (FdoString*) scName );
    CPPUNIT_ASSERT_MESSAGE( message, maxx == env->GetMaxX() );

    sprintf( message, "Wrong maxy for %ls", (FdoString*) scName );
    CPPUNIT_ASSERT_MESSAGE( message, maxy == env->GetMaxY() );
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
