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
#include "PostGisFdoSpatialContextTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "../SchemaMgr/Ph/Rd/QueryReader.h"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisFdoSpatialContextTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisFdoSpatialContextTest, "FdoSpatialContextTest");

void PostGisFdoSpatialContextTest::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
}

void PostGisFdoSpatialContextTest::testAxes()
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

    try {
        // open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			false,
			true,
            L"",
            0,
            NULL,
            0
		);
        connection->Close();

        staticConn = UnitTestUtil::NewStaticConnection();
        staticConn->connect();

        FdoSchemaManagerP mgr = staticConn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", L"")
        );
        FdoSmPhOwnerP owner = phMgr->FindOwner(datastore);
        owner->SetCurrent();

        FdoStringP sqlString = L"select srid, srtext from public.spatial_ref_sys";
        FdoSmPhRowP row = new FdoSmPhRow( phMgr, L"spatial_ref_sys" );
        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"srid",
            row->CreateColumnInt64(L"srid",true)
        );

        field = new FdoSmPhField(
            row, 
            L"srtext",
            row->CreateColumnChar(L"srtext",true, 1000)
        );

        FdoSmPhRdQueryReaderP reader = phMgr->CreateQueryReader( row, sqlString );

        while ( reader->ReadNext() )
        {
            FdoInt64 srid = reader->GetInt64(L"", L"srid");
            FdoStringP csysName = reader->GetString(L"", L"srid");
            FdoStringP wkt = reader->GetString(L"", L"srtext");

            FdoSmPhCoordinateSystemP csys = new FdoSmPhCoordinateSystem(phMgr, csysName, L"", srid, wkt);
            FdoStringP fixedWkt = csys->GetWkt();

            if ( !wkt.Contains(L"AXIS") ) 
            {
                if ( wkt != fixedWkt )
                {
                    printf( "%ls\n", (FdoString*) wkt );
                    printf( "%ls\n", (FdoString*) fixedWkt );
                    CPPUNIT_FAIL("WKT should not have changed, see log");
                }
            }
            else if ( fixedWkt.Contains(L"AXIS") )
            {
                printf( "%ls\n", (FdoString*) wkt );
                printf( "%ls\n", (FdoString*) fixedWkt );
                CPPUNIT_FAIL("WKT still has axes, see log");
            }
            else
            {
                CPPUNIT_ASSERT( wkt != fixedWkt );

                FdoString* start1 = (FdoString*) wkt;
                FdoString* start2 = (FdoString*) fixedWkt;

                while ( (*start1) == (*start2) )
                {
                    if ( wcsncmp(start1, L",AXIS", 5) == 0)
                        break;

                    start1++;
                    start2++;
                }


                FdoStringP removed = FdoStringP(start1).Mid(0, wkt.GetLength() - fixedWkt.GetLength());

                if ( removed == L",AXIS[\"Easting\",UNKNOWN],AXIS[\"Northing\",UNKNOWN]" )
                {
                }
                else if ( removed == L",AXIS[\"X\",UNKNOWN],AXIS[\"Y\",UNKNOWN]" )
                {
                }
                else if ( removed == L",AXIS[\"none\",EAST],AXIS[\"none\",NORTH]" )
                {
                }
                else if ( removed == L",AXIS[\"none\",NORTH],AXIS[\"none\",EAST]" )
                {
                }
                else
                {
                    bool success = true;

                    FdoStringP XName = L"\"X\"";
                    FdoStringP YName = L"\"Y\"";
                    int len = 31;

                    if ( removed.Contains(L"\"x\"") )
                    {
                        XName = L"\"x\"";
                        YName = L"\"y\"";
                    }
                    if ( removed.Contains(L"Easting") )
                    {
                        XName = L"\"Easting\"";
                        YName = L"\"Northing\"";
                        len = 44;
                    }

                    if ( removed.Contains(L"E(X)") )
                    {
                        XName = L"\"E(X)\"";
                        YName = L"\"N(Y)\"";
                        len = 37;
                    }

                    if ( removed.Contains(L"M") )
                    {
                        XName = L"\"M\"";
                        YName = L"\"P\"";
                    }

                    if ( removed.GetLength() != len )
                        success = false;
                    else if ( !removed.Contains(L",AXIS") )
                        success = false;
                    else if ( !removed.Contains(XName) )
                        success = false;
                    else if ( !removed.Contains(YName) )
                        success = false;
                    else if ( !(removed.Contains(L"NORTH") || removed.Contains(L"SOUTH")) )
                        success = false;
                    else if ( !(removed.Contains(L"EAST") || removed.Contains(L"WEST")) )
                        success = false;
                    else if ( removed.Contains(L"NORTH") && removed.Contains(L"SOUTH") )
                        success = false;
                    else if ( removed.Contains(L"EAST") && removed.Contains(L"WEST") )
                        success = false;

                    if ( !success ) 
                    {
                        printf( "%ls\n", (FdoString*) wkt );
                        printf( "%ls\n", (FdoString*) fixedWkt );
                        printf( "Removed: \"%ls\"\n", (FdoString*) removed );
                        CPPUNIT_FAIL("wrong part of WKT removed, see log");
                    }
                }
            }
        
            double x1, y1, z1; 

            double x2 = x1 = 5.5;
            double y2 = y1 = 3.1;
            double z2 = z1 = 1.9;

            csys->TransformOut(x2, y2, z2);

            if ( z2 != z1 )
            {
                printf( "%ls\n", (FdoString*) fixedWkt );
                CPPUNIT_FAIL("Z not supposed to change, see log");
            }

            if ( srid == 31254 )
            {
                CPPUNIT_ASSERT_MESSAGE( "SRID 31254 wrong X", x2 == y1 ); 
                CPPUNIT_ASSERT_MESSAGE( "SRID 31254 wrong Y", y2 == x1 ); 
            }
            else if ( srid == 2227 )
            {
                CPPUNIT_ASSERT_MESSAGE( "SRID 2227 wrong X", x2 == x1 ); 
                CPPUNIT_ASSERT_MESSAGE( "SRID 2227 wrong Y", y2 == y1 ); 
            }
            else if ( srid == 2065 )
            {
                CPPUNIT_ASSERT_MESSAGE( "SRID 2065 wrong X", x2 == -y1 ); 
                CPPUNIT_ASSERT_MESSAGE( "SRID 2065 wrong Y", y2 == -x1 ); 
            }

            csys->TransformIn(x2, y2, z2);

            if ( (x2 != x1) || (y2 != y1) || (z2 != z1) )
            {
                printf( "%ls\n", (FdoString*) fixedWkt );
                CPPUNIT_FAIL("XYZ different on round trip, see log");
            }

        
        }
 
        reader = NULL;
        owner = NULL;
        phMgr = NULL;
        mgr = NULL;

        // Close connections.
        delete staticConn;

    }
    catch (FdoException *exp)
    {
        try
        {
            if ( staticConn ) delete staticConn;
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
        }
        catch ( ... )
        {
        }

        throw;
    }
}



/* TODO Port this test
void PostGisFdoSpatialContextTest::testNumericCoordinateSystemNames( )
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
*/
