 //
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

#include "Pch.h"
#include "FdoSpatialContextTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "XmlFormatter.h"
#include "../SchemaMgr/Ph/Owner.h"

#define DB_NAME_SUFFIX L"_spatialcontext"

FdoSpatialContextTest::FdoSpatialContextTest(void)
{
}

FdoSpatialContextTest::~FdoSpatialContextTest(void)
{
}

void FdoSpatialContextTest::setUp ()
{
    set_provider();
}

void FdoSpatialContextTest::testMeta ()
{
	try {
        DoTest( true );
	}
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
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

void FdoSpatialContextTest::testNoMeta ()
{
	try {
        DoTest( false );
	}
	catch ( FdoException* e ) 
	{
		UnitTestUtil::FailOnException( e );
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

void FdoSpatialContextTest::DoTest( bool hasMetaSchema )
{
    FdoPtr<FdoIConnection> connection;
    StaticConnection* staticConn = NULL;

    try {
        if ( !hasMetaSchema ) {
            staticConn = UnitTestUtil::NewStaticConnection();
            staticConn->connect();
            UnitTestUtil::CreateDBNoMeta( 
                staticConn->CreateSchemaManager(),
                UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)
            );
        }


        // delete, re-create and open the datastore
		printf( "Initializing Connection ... \n" );
		connection = UnitTestUtil::CreateConnection(
			hasMetaSchema,
			hasMetaSchema,
            DB_NAME_SUFFIX,
            0,
            NULL,
            0
		);

		printf( "Creating Spatial Contexts ... \n" );

        UnitTestUtil::CreateSpatialContext( connection, L"Bermuda", L"Bermuda 1957", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"Rectangular", L"", 0, 0, 1000, 1000 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"Bermuda Duplicate", L"Bermuda 1957", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"Luxembourg Delete", L"Luxembourg 1930", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"South", L"Australian Antarctic", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"sc_2", L"Puerto Rico", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"South Delete", L"Australian Antarctic", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"sc_1", L"NAD27", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"South Duplicate", L"Australian Antarctic", 0, 0, 10, 10 ); 
        UnitTestUtil::CreateSpatialContext( connection, L"Liberia by WKT", L"", 0, 0, 10, 10, L"GEOGCS[\"Liberia 1964\", DATUM[\"Liberia 1964\", ELLIPSOID[\"Clarke 1880 (RGS)\", 6378249.145, 293.465]], PRIMEM[\"Greenwich\", 0], UNIT[\"Degree\", 0.0174532925199433]]" ); 
        UnitTestUtil::CreateSpatialContext( connection, L"Qatar wrong name", L"Bermuda 1957", 0, 0, 10, 10, L"GEOGCS[\"Qatar 1974\", DATUM[\"Qatar 1974\", ELLIPSOID[\"International 1924\", 6378388, 297]], PRIMEM[\"Greenwich\", 0], UNIT[\"Degree\", 0.0174532925199433]]" ); 

        connection->Close();
        connection->Open();

		printf( "Deleting Spatial Contexts ... \n" );

        UnitTestUtil::DeleteSpatialContext( connection, L"Luxembourg Delete" );
        UnitTestUtil::DeleteSpatialContext( connection, L"South Delete" );

        connection->Close();
        connection->Open();

        CreateSchema( connection, hasMetaSchema );

		printf( "Writing output ... \n" );

        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX);

        FdoIoMemoryStreamP stream1 = FdoIoMemoryStream::Create();

        FdoXmlSpatialContextFlagsP flags = FdoXmlSpatialContextFlags::Create(
            L"fdo.osgeo.org/schemas/feature",
            FdoXmlFlags::ErrorLevel_Normal,
            true,
            FdoXmlSpatialContextFlags::ConflictOption_Add,
            true
        );

        UnitTestUtil::ExportDb( 
            connection, 
            stream1, 
            flags,
            false, 
            FdoStringP(L"Fdo") + datastore
        );

   		UnitTestUtil::Config2SortedFile( stream1, UnitTestUtil::GetOutputFileName( GenFileName(1, hasMetaSchema, false) ) );

        UnitTestUtil::CheckOutput( 
            GenFileName( 1, hasMetaSchema, true ),
            UnitTestUtil::GetOutputFileName( GenFileName(1, hasMetaSchema, false) )
        );

        InsertFeatures( connection );

        stream1 = FdoIoMemoryStream::Create();

        UnitTestUtil::ExportDb( 
            connection, 
            stream1, 
            flags,
            false, 
            FdoStringP(L"Fdo") + datastore,
            L"AutoGen",
            true
        );

   		UnitTestUtil::Config2SortedFile( stream1, UnitTestUtil::GetOutputFileName( GenFileName(2, hasMetaSchema, false) ) );

        UnitTestUtil::CheckOutput( 
            GenFileName( 2, hasMetaSchema, true ),
            UnitTestUtil::GetOutputFileName( GenFileName(2, hasMetaSchema, false) )
        );

        if ( !hasMetaSchema ) {
            CreateTables( staticConn, datastore );

            connection->Close();
            connection->Open();

            stream1 = FdoIoMemoryStream::Create();

            UnitTestUtil::ExportDb( 
                connection, 
                stream1, 
                flags,
                false, 
                FdoStringP(L"Fdo") + datastore,
                L"AutoGen",
                true
            );

   		    UnitTestUtil::Config2SortedFile( stream1, UnitTestUtil::GetOutputFileName( GenFileName(3, hasMetaSchema, false) ) );

            UnitTestUtil::CheckOutput( 
                GenFileName( 3, hasMetaSchema, true ),
                UnitTestUtil::GetOutputFileName( GenFileName(3, hasMetaSchema, false) )
            );


            try {
                UnitTestUtil::CreateSpatialContext( connection, L"California", L"CA-I", 0, 0, 10, 10 ); 
                CPPUNIT_FAIL( "Setting unsupported coordinate system by name supposed to fail" );
            }
            catch (FdoException* e ) {
#ifdef _WIN32
                FdoStringP expectedMessage = L" Error creating spatial context California, coordinate system CA-I is not in current datastore. ";
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' );
                if (pMessage) pMessage++;
                CPPUNIT_ASSERT( pMessage && expectedMessage.ICompare(pMessage) == 0 );
#endif
                FDO_SAFE_RELEASE(e);
            }

            try {
                UnitTestUtil::CreateSpatialContext( connection, L"California", L"", 0, 0, 10, 10, L"PROJCS[\"CA-I\",GEOGCS[\"LL27\",DATUM[\"NAD27\",SPHEROID[\"CLRK66\",6378206.400,294.97869821]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Lambert_Conformal_Conic_2SP\"],PARAMETER[\"false_easting\",2000000.000],PARAMETER[\"false_northing\",0.000],PARAMETER[\"central_meridian\",-122.00000000000000],PARAMETER[\"latitude_of_origin\",39.33333333333333],PARAMETER[\"standard_parallel_1\",41.66666666666666],PARAMETER[\"standard_parallel_2\",40.00000000000000],UNIT[\"Foot_US\",0.30480060960122]]" ); 
                CPPUNIT_FAIL( "Setting unsupported coordinate system by wkt supposed to fail" );
            }
            catch (FdoException* e ) {
#ifdef _WIN32
                FdoStringP expectedMessage = L" Error creating spatial context California, coordinate system catalog does not contain entry for WKT 'PROJCS[\"CA-I\",GEOGCS[\"LL27\",DATUM[\"NAD27\",SPHEROID[\"CLRK66\",6378206.400,294.97869821]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Lambert_Conformal_Conic_2SP\"],PARAMETER[\"false_easting\",2000000.000],PARAMETER[\"false_northing\",0.000],PARAMETER[\"central_meridian\",-122.00000000000000],PARAMETER[\"latitude_of_origin\",39.33333333333333],PARAMETER[\"standard_parallel_1\",41.66666666666666],PARAMETER[\"standard_parallel_2\",40.00000000000000],UNIT[\"Foot_US\",0.30480060960122]]' ";
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' );
                if (pMessage) pMessage++;
                CPPUNIT_ASSERT( pMessage && expectedMessage.ICompare(pMessage) == 0 );
#endif
                FDO_SAFE_RELEASE(e);
            }
        }
        else {
            UnitTestUtil::CreateSpatialContext( connection, L"California1", L"CA-I", 0, 0, 10, 10 ); 
            UnitTestUtil::CreateSpatialContext( connection, L"California2", L"CA_I", 0, 0, 10, 10, L"PROJCS[\"CA-I\",GEOGCS[\"LL27\",DATUM[\"NAD27\",SPHEROID[\"CLRK66\",6378206.400,294.97869821]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Lambert_Conformal_Conic_2SP\"],PARAMETER[\"false_easting\",2000000.000],PARAMETER[\"false_northing\",0.000],PARAMETER[\"central_meridian\",-122.00000000000000],PARAMETER[\"latitude_of_origin\",39.33333333333333],PARAMETER[\"standard_parallel_1\",41.66666666666666],PARAMETER[\"standard_parallel_2\",40.00000000000000],UNIT[\"Foot_US\",0.30480060960122]]" ); 

            stream1 = FdoIoMemoryStream::Create();
            UnitTestUtil::ExportDb( 
                connection, 
                stream1, 
                flags,
                false, 
                FdoStringP(L"Fdo") + datastore,
                L"AutoGen",
                true
            );

            UnitTestUtil::Config2SortedFile( stream1, UnitTestUtil::GetOutputFileName( GenFileName(3, hasMetaSchema, false) ) );

            UnitTestUtil::CheckOutput( 
                GenFileName( 3, hasMetaSchema, true ),
                UnitTestUtil::GetOutputFileName( GenFileName(3, hasMetaSchema, false) )
            );

        }

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

void FdoSpatialContextTest::CreateSchema( FdoIConnection* connection, bool hasMetaSchema )
{
    FdoPtr<FdoFeatureSchema> schema;

    if ( hasMetaSchema ) {
    	/* A schema with dictionary */
        schema = FdoFeatureSchema::Create( L"ScTest", L"" );
    }
    else {
	    FdoPtr<FdoIDescribeSchema> cmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
	    FdoFeatureSchemasP schemas = cmd->Execute();

        CPPUNIT_ASSERT( schemas->GetCount() > 0 );

        schema = schemas->FindItem(L"dbo");
        if ( !schema ) 
            schema = schemas->GetItem(0);
    }

	FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

	/* An abstract base class */

	FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"ClassB1", L"" );
	FdoClassesP(schema->GetClasses())->Add( pClass );

	FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"Prop1", L"" );
	pProp->SetDataType( FdoDataType_Int64 );
	pProp->SetNullable(false);
    pProp->SetIsAutoGenerated(true);
	FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	// Test geometry property

	FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry1", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation(L"Bermuda");
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	pClass->SetGeometryProperty( pGeomProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry2", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation(L"Rectangular");
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry3", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation(L"South");
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry4", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation(L"Bermuda Duplicate");
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry5", L"" );
	pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
    pGeomProp->SetSpatialContextAssociation(L"sc_1");
    FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );

	pCmd->SetFeatureSchema( schema );
	pCmd->Execute();
}

void FdoSpatialContextTest::InsertFeatures( FdoIConnection* connection )
{
    FdoPtr<FdoIInsert> insertCommand;
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;
    FdoPtr<FdoIFeatureReader> reader;


    insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(L"ClassB1");

    propertyValues = insertCommand->GetPropertyValues();
    propertyValue;

    double       coordsBuffer[4];
    int          segCount = 2;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    coordsBuffer[0] = 2.1;
    coordsBuffer[1] = 3.1;
    coordsBuffer[2] = 4.1;
    coordsBuffer[3] = 5.1;

    FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(line1);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue = FdoPropertyValue::Create(L"Geometry1", geometryValue);
    propertyValues->Add(propertyValue);

    coordsBuffer[0] += 0.1;
    coordsBuffer[1] += 0.1;
    coordsBuffer[2] += 0.1;
    coordsBuffer[3] += 0.1;

    line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    byteArray = gf->GetFgf(line1);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue = FdoPropertyValue::Create(L"Geometry2", geometryValue);
    propertyValues->Add(propertyValue);

    coordsBuffer[0] += 0.1;
    coordsBuffer[1] += 0.1;
    coordsBuffer[2] += 0.1;
    coordsBuffer[3] += 0.1;

    line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    byteArray = gf->GetFgf(line1);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue = FdoPropertyValue::Create(L"Geometry3", geometryValue);
    propertyValues->Add(propertyValue);

    coordsBuffer[0] += 0.1;
    coordsBuffer[1] += 0.1;
    coordsBuffer[2] += 0.1;
    coordsBuffer[3] += 0.1;

    line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    byteArray = gf->GetFgf(line1);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue = FdoPropertyValue::Create(L"Geometry4", geometryValue);
    propertyValues->Add(propertyValue);

    coordsBuffer[0] += 0.1;
    coordsBuffer[1] += 0.1;
    coordsBuffer[2] += 0.1;
    coordsBuffer[3] += 0.1;

    line1 = gf->CreateLineString(FdoDimensionality_XY, segCount*2, coordsBuffer);
    byteArray = gf->GetFgf(line1);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue = FdoPropertyValue::Create(L"Geometry5", geometryValue);
    propertyValues->Add(propertyValue);

    reader = insertCommand->Execute();
}

void FdoSpatialContextTest::CreateTables( StaticConnection* staticConn, FdoStringP datastore )
{
    FdoSchemaManagerP sm = staticConn->CreateSchemaManager();
    FdoSmPhMgrP phMgr = sm->GetPhysicalSchema();
    FdoSmPhOwnerP owner = phMgr->FindOwner( datastore );

    FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table1" ));
    FdoSmPhColumnP column = table->CreateColumnInt32( L"id", false );
    table->AddPkeyCol( column->GetName() );

    FdoSmPhScInfoP scinfo = UnitTestUtil::CreateScInfo( 4903, 0, 0, 10, 10, 0.001, 0.001 );
    column = table->CreateColumnGeom( L"geometry1", scinfo, true, false );
    
    scinfo = UnitTestUtil::CreateScInfo( 4903, 0, 0, 20, 20, 0.001, 0.001 );
    column = table->CreateColumnGeom( L"geometry2", scinfo, true, false );
   
    table = owner->CreateTable( phMgr->GetDcDbObjectName(L"table2" ));
    column = table->CreateColumnInt32( L"id", false );
    table->AddPkeyCol( column->GetName() );

    scinfo = UnitTestUtil::CreateScInfo( 4722, 0, 0, 10, 10, 0.001, 0.001 );
    column = table->CreateColumnGeom( L"geometry1", scinfo, true, false );
   
    scinfo = UnitTestUtil::CreateScInfo( 4216, 0, 0, 10, 10, 0.001, 0.001 );
    column = table->CreateColumnGeom( L"geometry2", scinfo, true, false );

    scinfo = UnitTestUtil::CreateScInfo( 4722, 0, 0, 10, 10, 0.001, 0.001 );
    column = table->CreateColumnGeom( L"geometry3", scinfo, true, false );

    owner->Commit();
}

FdoStringP FdoSpatialContextTest::GenFileName( int fileNum, bool hasMetaSchema, bool isMaster )
{
    return FdoStringP::Format( L"spatial_context_test%d%c%ls.xml", fileNum, hasMetaSchema ? 'm' : 'n', isMaster ? L"_master" : L"" );
}
