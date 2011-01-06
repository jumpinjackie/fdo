// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include "SpatialContextTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\SpatialContextTest.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/SpatialContextTest.sqlite";
#endif
#include "FdoCommonFile.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SpatialContextTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SpatialContextTest, "SpatialContextTest");

SpatialContextTest::SpatialContextTest(void)
{
    
}

SpatialContextTest::~SpatialContextTest(void)
{
    
}


void SpatialContextTest::setUp ()
{

}

void SpatialContextTest::tearDown ()
{
}

void SpatialContextTest::TestFdo309 ()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        FdoPtr<FdoIGetSpatialContexts> gscCmd;
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        connection = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, false );

        FdoFeatureSchemaP pTestSchema = InitSchema( connection );

        FdoPtr<FdoIApplySchema>pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
        pCmd->SetFeatureSchema( pTestSchema );
	    pCmd->Execute();
        pTestSchema = NULL;
        // case where we have a class but no row in SC
        {
            gscCmd = (FdoIGetSpatialContexts *)connection->CreateCommand( FdoCommandType_GetSpatialContexts );
            // Get a SC reader
            FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();
            // Iterate ...
            while ( reader->ReadNext() )
            {
			    FdoPtr<FdoByteArray> ext = reader->GetExtent();
		    }
        }
        // let's add an empty row and test again
        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(connection->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"insert into spatial_ref_sys (srid,auth_srid,sr_xytol) values (0,0,0.005);");
        sqlCmd->ExecuteNonQuery();
        {
            gscCmd = (FdoIGetSpatialContexts *)connection->CreateCommand( FdoCommandType_GetSpatialContexts );
            // Get a SC reader
            FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();
            // Iterate ...
            while ( reader->ReadNext() )
            {
			    FdoPtr<FdoByteArray> ext = reader->GetExtent();
		    }
        }
        // create a new SC and set this SC to all classes
	    FdoPtr<FdoICreateSpatialContext> pCreateSC = (FdoICreateSpatialContext*) connection->CreateCommand(FdoCommandType_CreateSpatialContext);
	    pCreateSC->SetCoordinateSystemWkt(L"GEOGCS[\"LL84\",DATUM[\"WGS84\",SPHEROID[\"WGS84\",6378137.000,298.25722293]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.01745329251994]]");
	    pCreateSC->SetDescription(L"World Coordinate System, Degrees, what else do you need to know?" );
	    pCreateSC->SetName( L"LL84" );
        pCreateSC->SetXYTolerance(0.004);
	    pCreateSC->Execute();
        sqlCmd->SetSQLStatement(L"update geometry_columns set srid=1;");
        sqlCmd->ExecuteNonQuery();

        connection->Close();

        // add a valid SC when reopen the file
        connection = UnitTestUtil::OpenConnection( SC_TEST_FILE, false );

		gscCmd = (FdoIGetSpatialContexts *)connection->CreateCommand( FdoCommandType_GetSpatialContexts );
        // Get a SC reader
        FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();
        // Iterate ...
        while ( reader->ReadNext() )
        {
			FdoPtr<FdoByteArray> ext = reader->GetExtent();
		}

    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
		
	printf( "Done\n" );
}

FdoFeatureSchemaP SpatialContextTest::InitSchema ( FdoIConnection* connection )
{
    FdoFeatureSchemaP pTestSchema = FdoFeatureSchema::Create( L"Default", L"Test spatial contexts" );

    FdoFeatureClassP   pfeatureclass = InitFeatureClass( L"FeatClass1" );
    FdoClassesP(pTestSchema->GetClasses())->Add( pfeatureclass );
    FdoClassP pclass = InitClass( L"Class1" );
    FdoClassesP(pTestSchema->GetClasses())->Add( pclass );

    return pTestSchema;
}

FdoFeatureClassP SpatialContextTest::InitFeatureClass( FdoStringP className ) 
{
    FdoFeatureClassP pfeatureclass = FdoFeatureClass::Create( className, L"FeatureClass Desc" );

    // Add identity property
    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"FeatId Prop" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );
    FdoPtr<FdoDataPropertyDefinitionCollection>(pfeatureclass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"Name" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(32);
    pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pProp );

	FdoGeometricPropertyP pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	pGeomProp->SetHasElevation(true);
    FdoPtr<FdoPropertyDefinitionCollection>(pfeatureclass->GetProperties())->Add( pGeomProp );
	pfeatureclass->SetGeometryProperty( pGeomProp );

    return pfeatureclass;
}

FdoClassP SpatialContextTest::InitClass( FdoStringP className ) 
{
    FdoClassP pclass = FdoClass::Create( className, L"Class Desc" );

    // Add identity property
    FdoDataPropertyP pProp = FdoDataPropertyDefinition::Create( L"id", L"id Prop" );
    pProp->SetDataType( FdoDataType_Int32 );
    pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );
    FdoPtr<FdoDataPropertyDefinitionCollection>(pclass->GetIdentityProperties())->Add( pProp );

    pProp = FdoDataPropertyDefinition::Create( L"Name", L"Name" );
    pProp->SetDataType( FdoDataType_String );
    pProp->SetLength(32);
    pProp->SetNullable(false);
    FdoPtr<FdoPropertyDefinitionCollection>(pclass->GetProperties())->Add( pProp );

    return pclass;
}

