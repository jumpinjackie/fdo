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
#include "SqlServerFdoSelectTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoSelectTest, "FdoSelectTest");

const FdoStringP SqlServerFdoSelectTest::mSpatialGeogSuffix(L"_spatial_geog",true);

void SqlServerFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoSelectTest::TestDateFilter()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoIInsert> insertCmd;
            FdoPtr<FdoIFeatureReader> featReader;
            FdoPtr<FdoPropertyValueCollection> propVals;
            FdoPtr<FdoPropertyValue> propVal;
            FdoPtr<FdoValueExpression> expr;

            FdoString* CLASSNAME = L"Acad:AcDbEntity";
            FdoString* DATEPROPNAME = L"datetime";
            FdoString* IDPROPNAME = L"FeatId";

            // Delete leftover data from previous tests:
            FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)mConnection->CreateCommand(FdoCommandType_Delete);
            deleteCmd->SetFeatureClassName(CLASSNAME);
            deleteCmd->SetFilter(L"layer = 'TESTDATES'");
            FdoInt32 numDeleted = deleteCmd->Execute();

            // Insert some data:
            insertCmd = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            insertCmd->SetFeatureClassName(CLASSNAME);
            propVals = insertCmd->GetPropertyValues();
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"TIMESTAMP '2004-05-06 00:00:00'");
            propVal = FdoPropertyValue::Create(L"datetime", expr);
            propVals->Add(propVal);
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"'TESTDATES'");
            propVal = FdoPropertyValue::Create(L"layer", expr);
            propVals->Add(propVal);
            featReader = insertCmd->Execute();
            featReader->ReadNext();
            FdoInt64 featid1 = featReader->GetInt64(IDPROPNAME);

            insertCmd = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            insertCmd->SetFeatureClassName(CLASSNAME);
            propVals = insertCmd->GetPropertyValues();
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"TIMESTAMP '2004-03-06 00:00:00'");
            propVal = FdoPropertyValue::Create(L"datetime", expr);
            propVals->Add(propVal);
            expr = (FdoValueExpression*)FdoValueExpression::Parse(L"'TESTDATES'");
            propVal = FdoPropertyValue::Create(L"layer", expr);
            propVals->Add(propVal);
            featReader = insertCmd->Execute();
            featReader->ReadNext();
            FdoInt64 featid2 = featReader->GetInt64(IDPROPNAME);

            // Select the data to verify the date filter worked:
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            selectCmd->SetFeatureClassName(CLASSNAME);
            FdoPtr<FdoIdentifier> dataPropID = FdoIdentifier::Create(DATEPROPNAME);
            FdoPtr<FdoDateTimeValue> dataValue = FdoDateTimeValue::Create(FdoDateTime(2004, 5, (FdoInt8)6));
            FdoPtr<FdoComparisonCondition> compCond   = FdoComparisonCondition::Create(
                       dataPropID,
                        FdoComparisonOperations_EqualTo,
                        dataValue);
            selectCmd->SetFilter(compCond);
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
            FdoInt64 idFound = -1;
            long countFound = 0;
            while (reader->ReadNext())
            {
                idFound = reader->GetInt64(IDPROPNAME);
                countFound++;
            }
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", countFound==1);
            CPPUNIT_ASSERT_MESSAGE("date filter gave wrong result(s)", idFound==featid1);
            reader->Close();
        }
        catch (FdoException* e)
        {
            TestCommonFail (e);
        }
    }
}

// In SQL Server 2008, geodetic polyons must have counter-clockwise exterior ring
// and clockwise interior rings. The SQLServerSpatial provider automatically reverses
// rings on spatial filter polygons to conform to this restriction.
//
// This test tries various polygons and multipolygons that violate the restriction,
// to ensure that the provider properly converts then to have the accepted ring
// orientations.

void SqlServerFdoSelectTest::TestSpatialGeog()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* conn = UnitTestUtil::NewStaticConnection();
    FdoStringP providerName = conn->GetServiceName();

    try {
        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", mSpatialGeogSuffix);

        printf( "\nOpening Connection ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        UnitTestUtil::CreateDBNoMeta( mgr, datastore );

        printf( "Creating Spatial Contexts and Feature Schema ...\n" );

        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            mSpatialGeogSuffix
        );

        SpatialGeogSchema( connection );

        printf( "Creating Features ...\n" );

        SpatialGeogData( connection );

        printf( "Performing Selects ...\n" );

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

        // Try various filter polygons
        // Simple clockwise polygon

        double ordsXYExt[10];
        ordsXYExt[0] = -0.005; ordsXYExt[1] = -0.005; 
        ordsXYExt[2] = -0.005; ordsXYExt[3] = 0.005; 
        ordsXYExt[4] = 0.005;  ordsXYExt[5] = 0.005; 
        ordsXYExt[6] = 0.005;  ordsXYExt[7] = -0.005; 
        ordsXYExt[8] = -0.005; ordsXYExt[9] = -0.005; 

        FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
        FdoPtr<FdoIGeometry> spatialArea = gf->CreatePolygon(extRing, NULL );

        SpatialGeogSelect( connection, spatialArea );

        // MultiPolygon - some clockwise, some counterclockwise
    
        FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();

        for ( int i = 0; i < 3; i++ ) 
        {
            switch ( i )
            {
            case 0:
                ordsXYExt[0] = -0.009; ordsXYExt[1] = -0.009; 
                ordsXYExt[2] = -0.009; ordsXYExt[3] = -0.007; 
                ordsXYExt[4] = -0.007; ordsXYExt[5] = -0.007; 
                ordsXYExt[6] = -0.007; ordsXYExt[7] = -0.009; 
                ordsXYExt[8] = -0.009; ordsXYExt[9] = -0.009; 
                break;
            case 1:
                ordsXYExt[0] = -0.009; ordsXYExt[1] = 0.007; 
                ordsXYExt[2] = -0.007; ordsXYExt[3] = 0.007; 
                ordsXYExt[4] = -0.007; ordsXYExt[5] = 0.009; 
                ordsXYExt[6] = -0.009; ordsXYExt[7] = 0.009; 
                ordsXYExt[8] = -0.009; ordsXYExt[9] = 0.007; 
                break;
            case 2:
                ordsXYExt[0] = 0.007; ordsXYExt[1] = 0.007; 
                ordsXYExt[2] = 0.007; ordsXYExt[3] = 0.009; 
                ordsXYExt[4] = 0.009; ordsXYExt[5] = 0.009; 
                ordsXYExt[6] = 0.009; ordsXYExt[7] = 0.007; 
                ordsXYExt[8] = 0.007; ordsXYExt[9] = 0.007; 
                break;
            }
            extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
            FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(extRing, NULL);
            polygons->Add(polygon);
        }

        spatialArea = gf->CreateMultiPolygon(polygons);

        SpatialGeogSelect( connection, spatialArea );

        // Clockwise polygon with Islands (some clockwise, some counterclockwise)

        ordsXYExt[0] = -0.005; ordsXYExt[1] = -0.005; 
        ordsXYExt[2] = -0.005; ordsXYExt[3] = 0.005; 
        ordsXYExt[4] = 0.005;  ordsXYExt[5] = 0.005 ; 
        ordsXYExt[6] = 0.005;  ordsXYExt[7] = -0.005; 
        ordsXYExt[8] = -0.005; ordsXYExt[9] = -0.005; 

        extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);

        FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create();

        for ( int i = 0; i < 3; i++ ) 
        {
            switch ( i )
            {
            case 0:
                ordsXYExt[0] = -0.004; ordsXYExt[1] = -0.004; 
                ordsXYExt[2] = -0.004; ordsXYExt[3] = -0.002; 
                ordsXYExt[4] = -0.002; ordsXYExt[5] = -0.002; 
                ordsXYExt[6] = -0.002; ordsXYExt[7] = -0.004; 
                ordsXYExt[8] = -0.004; ordsXYExt[9] = -0.004; 
                break;
            case 1:
                ordsXYExt[0] = -0.004; ordsXYExt[1] = 0.002; 
                ordsXYExt[2] = -0.002; ordsXYExt[3] = 0.002; 
                ordsXYExt[4] = -0.002; ordsXYExt[5] = 0.004; 
                ordsXYExt[6] = -0.004; ordsXYExt[7] = 0.004; 
                ordsXYExt[8] = -0.004; ordsXYExt[9] = 0.002; 
                break;
            case 2:
                ordsXYExt[0] = 0.002; ordsXYExt[1] = 0.002; 
                ordsXYExt[2] = 0.002; ordsXYExt[3] = 0.004; 
                ordsXYExt[4] = 0.004; ordsXYExt[5] = 0.004; 
                ordsXYExt[6] = 0.004; ordsXYExt[7] = 0.002; 
                ordsXYExt[8] = 0.002; ordsXYExt[9] = 0.002; 
                break;
            }
            FdoPtr<FdoILinearRing>  intRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
            intRings->Add(intRing);
        }

        spatialArea = gf->CreatePolygon(extRing, intRings );

        SpatialGeogSelect( connection, spatialArea );

        // MultiPolygon with Islands. 1st polygon has CW exterior

        polygons = FdoPolygonCollection::Create();
        polygons->Add( (FdoIPolygon*) spatialArea.p );

        ordsXYExt[0] = 0.007; ordsXYExt[1] = 0.007; 
        ordsXYExt[2] = 0.007; ordsXYExt[3] = 0.009; 
        ordsXYExt[4] = 0.009; ordsXYExt[5] = 0.007; 
        ordsXYExt[6] = 0.007; ordsXYExt[7] = 0.007; 

        extRing = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYExt);
        FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(extRing, NULL );
        polygons->Add(polygon);

        spatialArea = gf->CreateMultiPolygon(polygons);

        SpatialGeogSelect( connection, spatialArea );

        // CounterClockwise polygon with Islands (some clockwise, some counterclockwise)

        ordsXYExt[0] = -0.005; ordsXYExt[1] = -0.005; 
        ordsXYExt[2] = 0.005;  ordsXYExt[3] = -0.005; 
        ordsXYExt[4] = 0.005;  ordsXYExt[5] = 0.005 ; 
        ordsXYExt[6] = -0.005; ordsXYExt[7] = 0.005; 
        ordsXYExt[8] = -0.005; ordsXYExt[9] = -0.005; 

        extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);

        spatialArea = gf->CreatePolygon(extRing, intRings );

        SpatialGeogSelect( connection, spatialArea );

        // MultiPolygon with Islands. 1st polygon has CCW exterior

        polygons = FdoPolygonCollection::Create();
        polygons->Add( (FdoIPolygon*) spatialArea.p );

        ordsXYExt[0] = 0.007; ordsXYExt[1] = 0.007; 
        ordsXYExt[2] = 0.007; ordsXYExt[3] = 0.009; 
        ordsXYExt[4] = 0.009; ordsXYExt[5] = 0.007; 
        ordsXYExt[6] = 0.007; ordsXYExt[7] = 0.007; 

        extRing = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYExt);
        polygon = gf->CreatePolygon(extRing, NULL );
        polygons->Add(polygon);

        spatialArea = gf->CreateMultiPolygon(polygons);

        SpatialGeogSelect( connection, spatialArea );

        connection->Close ();

        conn->disconnect();
        delete conn;
        conn = NULL;

        printf( "Done.\n" );
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
            if (conn != NULL)
            {
                conn->disconnect();
                delete conn;
            }
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException(ex);
    }
    catch (...)
    {
        try {
            if( connection )
                connection->Close ();
            if (conn != NULL)
            {
                conn->disconnect();
                delete conn;
            }
        }
        catch ( ... )
        {
        }
        throw;
    }
}

void SqlServerFdoSelectTest::spatial_query_defect813611()
{

}


void SqlServerFdoSelectTest::SpatialGeogSchema( FdoIConnection* connection )
{
    UnitTestUtil::CreateSpatialContext( connection, L"projSC", L"CA-I", 0, 0, 0, 0 );
    UnitTestUtil::CreateSpatialContext( connection, L"geogSC", L"WGS 84", 0, 0, 0, 0 );

    FdoPtr<FdoIApplySchema>  cmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);

    FdoFeatureSchemaP schema = FdoFeatureSchema::Create( L"Schema1", L"" );
    FdoClassesP classes = schema->GetClasses();

    FdoFeatureClassP featClass = FdoFeatureClass::Create( L"FeatClass1", L"" );
    featClass->SetIsAbstract(false);
    classes->Add(featClass);
    FdoPropertiesP props = featClass->GetProperties();
    FdoDataPropertiesP idProps = featClass->GetIdentityProperties();

    FdoDataPropertyP dataProp = FdoDataPropertyDefinition::Create( L"Name", L"" );
    dataProp->SetDataType( FdoDataType_String );
    dataProp->SetLength(20);
    dataProp->SetNullable(false);
    props->Add( dataProp );
    idProps->Add( dataProp );

    FdoGeometricPropertyP geomProp = FdoGeometricPropertyDefinition::Create( L"Geom", L"" );
    geomProp->SetSpatialContextAssociation(L"projSC");
    props->Add( geomProp );

    geomProp = FdoGeometricPropertyDefinition::Create( L"Geog", L"" );
    geomProp->SetSpatialContextAssociation(L"geogSC");
    props->Add( geomProp );
    cmd->SetFeatureSchema( schema );
    cmd->Execute();

}

void SqlServerFdoSelectTest::SpatialGeogData( FdoIConnection* connection )
{
    FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand;
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;
    FdoPtr<FdoIFeatureReader> reader;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(L"Schema1:FeatClass1");
    propertyValues = insertCommand->GetPropertyValues();

    for ( int y = -10; y < 10; y++ ) 
    {
        for ( int x = -10; x < 10; x++ )
        {

            dataValue = FdoDataValue::Create( FdoStringP::Format(L"%d_%d", x, y) );
            propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Name" );
            propertyValue->SetValue(dataValue);
    
            double coordinateBuffer[4];
            
            coordinateBuffer[0] = (double) x / 1000 + 0.0002;
            coordinateBuffer[1] = (double) y / 1000 + 0.0002;
            coordinateBuffer[2] = coordinateBuffer[0] + 0.0005;
            coordinateBuffer[3] = coordinateBuffer[1];

            FdoPtr<FdoIGeometry> geom = gf->CreateLineString(
                FdoDimensionality_XY,
                4, 
                coordinateBuffer
            );
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(geom);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray); 
           
            propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Geom" );
            propertyValue->SetValue(geometryValue);

            propertyValue = TestCommonMiscUtil::AddNewProperty( propertyValues, L"Geog" );
            propertyValue->SetValue(geometryValue);

            reader = insertCommand->Execute();
        }
    }

    featureTransaction->Commit();
}

void SqlServerFdoSelectTest::SpatialGeogSelect( FdoIConnection* connection, FdoIGeometry* spatialArea )
{
    FdoStringsP projNames = SpatialGeogSelectNames( connection, spatialArea, L"Geom" );
    FdoStringsP geogNames = SpatialGeogSelectNames( connection, spatialArea, L"Geog" );

    CPPUNIT_ASSERT( projNames->GetCount() == geogNames->GetCount() );

    for ( int i = 0; i < projNames->GetCount(); i++ ) 
    {
        CPPUNIT_ASSERT( geogNames->IndexOf(projNames->GetString(i)) >= 0 );
    }
}

FdoStringsP SqlServerFdoSelectTest::SpatialGeogSelectNames( FdoIConnection* connection, FdoIGeometry* spatialArea, FdoStringP propName )
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoStringsP names = FdoStringCollection::Create();

    FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(spatialArea)));
    FdoPtr<FdoSpatialCondition> spatialFilter = FdoSpatialCondition::Create(propName,
                                                                      FdoSpatialOperations_Intersects,
                                                                      geomValue);

    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( L"Schema1:FeatClass1" );
    selectCommand->SetFilter(spatialFilter);

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    while ( rdr->ReadNext() )
    {
        names->Add( rdr->GetString(L"Name") );
    }

    return names;
}
