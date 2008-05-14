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
#include "SqlServerFdoUpdateTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "../../SQLServerSpatial/SchemaMgr/SchemaManager.h"
#include "../../SQLServerSpatial/SchemaMgr/Ph/Mgr.h"
#include <Sm/Ph/Rd/QueryReader.h>

#define NO_META_SUFFIX L"_no_meta2"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoUpdateTest, "FdoUpdateTest");

void SqlServerFdoUpdateTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
	m_hasGeom = true;
	m_hasAssoc = true;
}

void SqlServerFdoUpdateTest::UpdateNonDbo()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* conn = UnitTestUtil::NewStaticConnection();
    FdoStringP providerName = conn->GetServiceName();

    try {
        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", NO_META_SUFFIX);

        printf( "Opening Connection ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();
        FdoSmPhSqsMgrP sqsMgr = phMgr->SmartCast<FdoSmPhSqsMgr>();
        FdoStringP verStr = sqsMgr->GetDbVersion();

        // Create foreign key to test associations only if Sql Server version
        // later than Sql Server 2000. On 2000, foreign keys on non-dbo tables
        // cannot yet be retrieved.
        bool after2000 = (verStr.Left( L"." ).ToLong() > 8 );

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        printf( "Predeleting schema ...\n" );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        printf( "Creating schema ...\n" );

        // Create a datastore without MetaSchema
        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );

        FdoStringP tableName = FdoStringP::Format(
            L"%hs.table1",
            "guest"
        );

        CreateExternalTable( owner, tableName, true, m_hasGeom, after2000 );

        owner->Commit();

        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            NO_META_SUFFIX
        );

        CreateExternalData( connection, phMgr, tableName, m_hasGeom, after2000 );

        // Select and verify all data (post-update state).

        SelectNoMetaAll( connection, phMgr, tableName, m_hasGeom, after2000 );
        SelectNoMetaFilter( connection, phMgr, tableName, m_hasGeom, after2000 );
        SelectNoMetaProps( connection, phMgr, tableName, m_hasGeom );

        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
            {
                connection->Close ();
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
            {
                connection->Close ();
            }
        }
        catch ( ... )
        {
        }
        throw;
    }
}

FdoString* SqlServerFdoUpdateTest::NoMetaSuffix()
{
    static FdoString* noMetaSuffix = L"_no_met\x00e4\x00e4\x00e4\x00e4";

    return noMetaSuffix;
}

FdoString* SqlServerFdoUpdateTest::MultiIdSuffix()
{
    static FdoString* multiIdSuffix = L"_\x00e4\x00e4\x00e4\x00e4";

    return multiIdSuffix;
}

FdoStringP SqlServerFdoUpdateTest::TableIdGeomName()
{
    // Seconds can handle Latin1 supplemental characters.
    return FdoStringP::Format( L"t%cble_id_geom", 0xe4 );;
}

void SqlServerFdoUpdateTest::testDefect810181 ()
{
    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);

        // Run Update commands to set all of the geometries to their existing values.
        // This verifies that Update can handle all of the same geometry values as 
        // Insert, which previously populated the class.
        // While there is no verification possible without white-box testing, the
        // original defect (810181) involves geometries whose values lead to
        // single-valued spatial index entries (instead of the possible two SI values).
        // Manual inspection on SqlServer shows that at least some values in the Parcel
        // class involve these values.

		FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"Länd:Parcel");
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
        FdoPtr<FdoByteArray> ba;
        FdoInt64 featId = -1;
        FdoString * province = NULL;
        FdoString * pin = NULL;
        while ( myReader->ReadNext() )
		{
            featId = myReader->GetInt64(L"FeatId");
            province = myReader->GetString(L"Prövince");
            pin = myReader->GetString(L"PIN");
            bool isNull = myReader->IsNull(UnitTestUtil::GetNlsObjectName(L"Geometry"));
            if (!isNull)
            {
			    ba = myReader->GetGeometry(UnitTestUtil::GetNlsObjectName(L"Geometry"));
                FdoInt32 size = ba->GetCount();
                FdoIGeometry * geom = gf->CreateGeometryFromFgf(ba);
                FdoString * fgfText = geom->GetText();

                FdoPtr<FdoPropertyValue> propertyValue;

                FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
                FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
                updateCommand->SetFeatureClassName(L"Länd:Parcel");
		        FdoPtr<FdoFilter> primaryKeyFilter1 = FdoComparisonCondition::Create(
	                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")),
	                FdoComparisonOperations_EqualTo, 
	                FdoPtr<FdoDataValue>(FdoDataValue::Create(featId)));
		        FdoPtr<FdoFilter> primaryKeyFilter2 = FdoComparisonCondition::Create(
	                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Prövince")),
	                FdoComparisonOperations_EqualTo, 
	                FdoPtr<FdoDataValue>(FdoDataValue::Create(province)));
		        FdoPtr<FdoFilter> primaryKeyFilter3 = FdoComparisonCondition::Create(
	                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"PIN")),
	                FdoComparisonOperations_EqualTo, 
	                FdoPtr<FdoDataValue>(FdoDataValue::Create(pin)));
                FdoPtr<FdoFilter> filter1 = FdoFilter::Combine( primaryKeyFilter1, FdoBinaryLogicalOperations_And, primaryKeyFilter2);
                FdoPtr<FdoFilter> filterAll = FdoFilter::Combine( filter1, FdoBinaryLogicalOperations_And, primaryKeyFilter3);
                updateCommand->SetFilter(filterAll);

                FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();

                propertyValue = AddNewProperty( propertyValues, UnitTestUtil::GetNlsObjectName(L"Geometry"));
                FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(ba);
                propertyValue->SetValue(geometryValue);

                // Change the "Value" property so that we are sure that there was an effect upon
                // manual inspection.
                propertyValue = AddNewProperty( propertyValues, L"Value");
                FdoPtr<FdoDataValue> dataValue = FdoDataValue::Create((FdoInt32)55);
                propertyValue->SetValue(dataValue);

                FdoInt32 numRows = updateCommand->Execute();
                CPPUNIT_ASSERT( numRows == 1 );

                featureTransaction->Commit();
            }
        }
    }
    catch (FdoCommandException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);

        // Run Update commands to set all of the geometries to their existing values.
        // This verifies that Update can handle all of the same geometry values as 
        // Insert, which previously populated the class.
        // While there is no verification possible without white-box testing, the
        // original defect (810181) involves geometries whose values lead to
        // single-valued spatial index entries (instead of the possible two SI values).
        // Manual inspection on SqlServer shows that at least some values in the Parcel
        // class involve these values.

		FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
        FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
        FdoPtr<FdoByteArray> ba;
        FdoInt64 featId = -1;
        while ( myReader->ReadNext() )
		{
            featId = myReader->GetInt64(L"FeatId");
            bool isNull = myReader->IsNull(L"Geometry");
            if (!isNull)
            {
			    ba = myReader->GetGeometry(L"Geometry");
                FdoInt32 size = ba->GetCount();
                FdoIGeometry * geom = gf->CreateGeometryFromFgf(ba);
                FdoString * fgfText = geom->GetText();

                FdoPtr<FdoPropertyValue> propertyValue;

                FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
                FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
                updateCommand->SetFeatureClassName(L"Acad:AcDb3dPolyline");
		        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
	                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")),
	                FdoComparisonOperations_EqualTo, 
	                FdoPtr<FdoDataValue>(FdoDataValue::Create(featId)));
                updateCommand->SetFilter(filter);

                FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();

                propertyValue = AddNewProperty( propertyValues, L"Geometry");
                FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(ba);
                propertyValue->SetValue(geometryValue);

                FdoInt32 numRows = updateCommand->Execute();
                CPPUNIT_ASSERT( numRows == 1 );

                featureTransaction->Commit();
            }
        }
    }
    catch (FdoCommandException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

void SqlServerFdoUpdateTest::UpdateSingleIdFeatureClass()
{
    // This is really a spatial query test, for the point-on-boundary condition.
    // It tests defect 787392.  We override UpdateSingleIdFeatureClass to make sure
    // that the spatial query follows the insertion of data.  We expect that there will
    // be five features with Point geometry values, all at location (5, 10).

    // Call base class to set up test data.
    FdoUpdateTest::UpdateSingleIdFeatureClass();

    FdoPtr<FdoIConnection> connection;
    FdoPtr<FdoITransaction> featureTransaction;

    try
    {
        connection = UnitTestUtil::GetConnection(mSuffix, false);

        FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

        selectCommand->SetFeatureClassName( L"L\x00e4nd:Municipality" );

        // Use a spatial filter that has the Point on its edge.
        TestRectangleFilter(selectCommand, 0.0, 10.0, 20.0, 30.0, FdoSpatialOperations_Intersects, 5);
        TestRectangleFilter(selectCommand, 0.0, 10.0, 20.0, 30.0, FdoSpatialOperations_Inside, 0);

        // Use a spatial filter that has the Point slightly outside.
        TestRectangleFilter(selectCommand, 0.0, 10.001, 20.0, 30.0, FdoSpatialOperations_Intersects, 0);
        TestRectangleFilter(selectCommand, 0.0, 10.0001, 20.0, 30.0, FdoSpatialOperations_Inside, 0);

        // Use a spatial filter that has the Point slightly inside.
        TestRectangleFilter(selectCommand, 0.0, 9.999, 20.0, 30.0, FdoSpatialOperations_Intersects, 5);
        TestRectangleFilter(selectCommand, 0.0, 9.999, 20.0, 30.0, FdoSpatialOperations_Inside, 5);

        connection->Close ();
    }
    catch (FdoException *ex)
    {
        try {
            if( connection )
                connection->Close ();
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
        }
        catch ( ... )
        {
        }
        throw;
    }

}

void SqlServerFdoUpdateTest::UpdateGeogColumn()
{
	FdoPtr<FdoIConnection> connection;
    StaticConnection* conn = UnitTestUtil::NewStaticConnection();
    FdoStringP providerName = conn->GetServiceName();
    FdoStringP table_id_geom = TableIdGeomName();

    try {
        FdoStringP datastore = UnitTestUtil::GetEnviron("datastore", NoMetaSuffix());

        printf( "\nOpening Connection ...\n" );

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        printf( "Predeleting schema ...\n" );

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        printf( "Creating schema ...\n" );

        // Create a datastore without MetaSchema
        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );

        CreateGeogTable( owner, table_id_geom );
        owner->Commit();

        connection = UnitTestUtil::CreateConnection(
            false,
            false,
            NoMetaSuffix()
        );

        CreateGeogData( connection, phMgr, table_id_geom );

        // Select and verify all data (post-update state).
        SelectGeogAll( connection, phMgr, table_id_geom );
	    SelectGeogSpatial( connection, phMgr, table_id_geom );

        connection->Close ();

        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;
        conn = NULL;
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

void SqlServerFdoUpdateTest::CreateGeogTable( FdoSmPhOwnerP owner, FdoStringP tableName )
{
    FdoSmPhMgrP phMgr = owner->GetManager();
    FdoSmPhTableP table = owner->CreateTable( phMgr->GetDcDbObjectName(tableName) );
    
    FdoSmPhColumnP column = table->CreateColumnChar( phMgr->GetDcColumnName(L"key1"), false, 10 );
    table->AddPkeyCol( column->GetName() );
    column = table->CreateColumnGeom( phMgr->GetDcColumnName(L"geometry"), NULL, true, false );
    column->SetTypeName( "geography" );
}

void SqlServerFdoUpdateTest::CreateGeogData( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName )
{
    FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
    FdoPtr<FdoIInsert> insertCommand;
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;
    FdoPtr<FdoIFeatureReader> reader;

    // Do some inserts. Verifies that objects, for feature classes without featid,
    // can be created.
    
    FdoStringP qClassName = phMgr->GetDcDbObjectName(tableName).Replace(L".",L":");
    
    insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(qClassName);

    propertyValues = insertCommand->GetPropertyValues();
    propertyValue;

    dataValue = FdoDataValue::Create(L"KEY1_1");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1"));
    propertyValue->SetValue(dataValue);
    
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    FdoPtr<FdoIGeometry> geom = CreateGeogGeom(1);
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(geom);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();
    CPPUNIT_ASSERT( reader!= NULL );
    
    dataValue = FdoDataValue::Create(L"KEY1_2");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    geom = CreateGeogGeom(2);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();

    dataValue = FdoDataValue::Create(L"KEY1_3");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    geom = CreateGeogGeom(3);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();

    dataValue = FdoDataValue::Create(L"KEY1_4");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    geom = CreateGeogGeom(4);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();

    dataValue = FdoDataValue::Create(L"KEY1_5");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    geom = CreateGeogGeom(5);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();

    dataValue = FdoDataValue::Create(L"KEY1_6");
    propertyValue = FdoUpdateTest::AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"key1") );
    propertyValue->SetValue(dataValue);
    
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );
    geom = CreateGeogGeom(5);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    reader = insertCommand->Execute();

    featureTransaction->Commit();
    featureTransaction = NULL;

    // Try geometric property update
    
    FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_1") )
    );
    
    FdoPtr<FdoIUpdate> UpdateCommand = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
    UpdateCommand->SetFeatureClassName(qClassName);
    UpdateCommand->SetFilter( filter );
    propertyValues = UpdateCommand->GetPropertyValues();
    propertyValue = AddNewProperty( propertyValues, phMgr->GetDcColumnName(L"geometry") );

    geom = CreateGeogGeom(101);
    byteArray = gf->GetFgf(geom);
    geometryValue = FdoGeometryValue::Create(byteArray); 
    propertyValue->SetValue(geometryValue);

    UpdateCommand->Execute();
    UpdateCommand = NULL;
    
    // Verifies that features without featid can be deleted.
    
    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);
    filter = FdoComparisonCondition::Create(
        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(phMgr->GetDcColumnName(L"key1")) ),
        FdoComparisonOperations_EqualTo,
        FdoPtr<FdoDataValue>(FdoDataValue::Create(L"KEY1_6") )
    );
        
    deleteCommand->SetFeatureClassName(qClassName);
    deleteCommand->SetFilter( filter );
    deleteCommand->Execute();
    deleteCommand = NULL;
}

FdoIGeometry* SqlServerFdoUpdateTest::CreateGeogGeom( int idx )
{
    FdoIGeometry* ret = NULL;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    double       coordsBuffer[50];

    switch ( idx ) {
    case 1:
        {
            coordsBuffer[0] = -100;
            coordsBuffer[1] = 50;
            coordsBuffer[2] = -99;
            coordsBuffer[3] = 51;

            ret = gf->CreateLineString(FdoDimensionality_XY, 4, coordsBuffer);
        }
        break;

    case 2:
        {
            coordsBuffer[0] = -90;
            coordsBuffer[1] = 50;
            ret = gf->CreatePoint(FdoDimensionality_XY, coordsBuffer);
        }
        break;

    case 3:
        {
            coordsBuffer[0] = -90;
            coordsBuffer[1] = 60;
            coordsBuffer[2] = -88;
            coordsBuffer[3] = 60;
            coordsBuffer[4] = -89;
            coordsBuffer[5] = 61;
            coordsBuffer[6] = -90;
            coordsBuffer[7] = 60;

            FdoPtr<FdoILinearRing> ring = gf->CreateLinearRing(FdoDimensionality_XY, 8, coordsBuffer);
            ret = gf->CreatePolygon( ring, NULL );
        }
        break;

    case 4:
        {
            FdoPtr<FdoLineStringCollection> lineStrings = FdoLineStringCollection::Create();

            coordsBuffer[0] = -100;
            coordsBuffer[1] = 60;
            coordsBuffer[2] = -99;
            coordsBuffer[3] = 61;

            lineStrings->Add( gf->CreateLineString(FdoDimensionality_XY, 4, coordsBuffer) );

            coordsBuffer[0] = -100;
            coordsBuffer[1] = 61;
            coordsBuffer[2] = -99;
            coordsBuffer[3] = 60;

            lineStrings->Add( gf->CreateLineString(FdoDimensionality_XY, 4, coordsBuffer) );

            ret = gf->CreateMultiLineString(lineStrings);
        }
        break;

    case 5:
        {
            FdoPtr<FdoGeometryCollection> geoms = FdoGeometryCollection::Create();

            coordsBuffer[0] = -95;
            coordsBuffer[1] = 55;

            geoms->Add( gf->CreatePoint(FdoDimensionality_XY, coordsBuffer) );

            coordsBuffer[0] = -96;
            coordsBuffer[1] = 54;
            coordsBuffer[2] = -94;
            coordsBuffer[3] = 54;
            coordsBuffer[4] = -94;
            coordsBuffer[5] = 56;
            coordsBuffer[6] = -96;
            coordsBuffer[7] = 56;
            coordsBuffer[8] = -96;
            coordsBuffer[9] = 54;

            FdoPtr<FdoILinearRing> ring = gf->CreateLinearRing(FdoDimensionality_XY, 10, coordsBuffer);
            geoms->Add( gf->CreatePolygon( ring, NULL ) );

            ret = gf->CreateMultiGeometry( geoms );
        }
        break;

    case 101:
        {
            coordsBuffer[0] = -100;
            coordsBuffer[1] = 49;
            coordsBuffer[2] = -99;
            coordsBuffer[3] = 51;

            ret = gf->CreateLineString(FdoDimensionality_XY, 4, coordsBuffer);
        }
        break;
    }

    return ret;
}
void SqlServerFdoUpdateTest::SelectGeogAll( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        VldGeogRow( phMgr, rdr, props );
    }

    CPPUNIT_ASSERT( rowCount == 5 );

    rdr->Close();
}

void SqlServerFdoUpdateTest::SelectGeogSpatial( FdoPtr<FdoIConnection> connection, FdoSmPhMgrP phMgr, FdoStringP tableName )
{
    FdoPtr<FdoISelect> selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    // Try select with filter


    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double ordsXYExt[10];
    ordsXYExt[0] = -97; ordsXYExt[1] = 53; 
    ordsXYExt[2] = -93; ordsXYExt[3] = 53; 
    ordsXYExt[4] = -93; ordsXYExt[5] = 57; 
    ordsXYExt[6] = -97; ordsXYExt[7] = 57; 
    ordsXYExt[8] = -97; ordsXYExt[9] = 53; 

    FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(extRing, NULL );
    FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create(FdoPtr<FdoByteArray>(gf->GetFgf(poly)));
    FdoPtr<FdoSpatialCondition> spatialFilter = FdoPtr<FdoSpatialCondition>(FdoSpatialCondition::Create(phMgr->GetDcColumnName(L"geometry"),
                                                                      FdoSpatialOperations_Intersects,
                                                                      geomValue));

    selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);

    selectCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    selectCommand->SetFilter(spatialFilter);

    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    FdoPtr<FdoIdentifierCollection> props = selectCommand->GetPropertyNames();

    int rowCount = 0;

    while ( rdr->ReadNext() )

    {
        rowCount++;
        CPPUNIT_ASSERT( wcscmp( FixStringVal(rdr->GetString(phMgr->GetDcColumnName(L"key1"))), L"KEY1_5" ) == 0 );
        VldGeogRow( phMgr, rdr, props );
    }

    CPPUNIT_ASSERT( rowCount == 1 );

    rdr->Close();

    FdoPtr<FdoISelectAggregates> extentsCommand = (FdoISelectAggregates *) connection->CreateCommand(FdoCommandType_SelectAggregates);
    extentsCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    props = extentsCommand->GetPropertyNames();

    FdoPtr<FdoComputedIdentifier> prop = (FdoComputedIdentifier*) FdoExpression::Parse( L"SpatialExtents(geometry) as extents" );
    props->Add(prop);

    FdoPtr<FdoIDataReader> extRdr = extentsCommand->Execute();

    CPPUNIT_ASSERT(extRdr->ReadNext());

    CPPUNIT_ASSERT( !extRdr->IsNull(L"extents") );

    gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> sGeom = extRdr->GetGeometry( L"extents" );
    CPPUNIT_ASSERT( sGeom );

    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf( sGeom );
    FdoPtr<FdoIEnvelope> env = geom->GetEnvelope();
    CPPUNIT_ASSERT( env->GetMinX() == -100 );
    CPPUNIT_ASSERT( env->GetMinY() == 49 );
    CPPUNIT_ASSERT( env->GetMaxX() == -88 );
    CPPUNIT_ASSERT( env->GetMaxY() == 61 );

    CPPUNIT_ASSERT(!extRdr->ReadNext());

    FdoPtr<FdoIdentifier> idProp = FdoIdentifier::Create( L"key1" );
    props->Add( idProp );
    
    FdoPtr<FdoIdentifierCollection> grouping = extentsCommand->GetGrouping();
    grouping->Add( idProp );
    bool failure = false;

    try {
        extRdr = extentsCommand->Execute();
    }
    catch ( FdoException* ex ) 
    {
        FDO_SAFE_RELEASE(ex);
        failure = true;
    }

    CPPUNIT_ASSERT( failure );

    extentsCommand = (FdoISelectAggregates *) connection->CreateCommand(FdoCommandType_SelectAggregates);
    extentsCommand->SetFeatureClassName( phMgr->GetDcDbObjectName(tableName).Replace(L".",L":") );
    props = extentsCommand->GetPropertyNames();

    props->Add(prop);
    prop = (FdoComputedIdentifier*) FdoExpression::Parse( L"Count(geometry) as total" );
    props->Add(prop);

    extRdr = extentsCommand->Execute();

    CPPUNIT_ASSERT(extRdr->ReadNext());

    CPPUNIT_ASSERT( !extRdr->IsNull(L"extents") );

    gf = FdoFgfGeometryFactory::GetInstance();
    sGeom = extRdr->GetGeometry( L"extents" );
    CPPUNIT_ASSERT( sGeom );

    geom = gf->CreateGeometryFromFgf( sGeom );
    env = geom->GetEnvelope();
    CPPUNIT_ASSERT( env->GetMinX() == -100 );
    CPPUNIT_ASSERT( env->GetMinY() == 49 );
    CPPUNIT_ASSERT( env->GetMaxX() == -88 );
    CPPUNIT_ASSERT( env->GetMaxY() == 61 );

    CPPUNIT_ASSERT( !extRdr->IsNull(L"total") );
    CPPUNIT_ASSERT( extRdr->GetInt64(L"total") == 5 );
    CPPUNIT_ASSERT( !extRdr->ReadNext() );
}
 
void SqlServerFdoUpdateTest::VldGeogRow( 
    FdoSmPhMgrP phMgr, 
    FdoPtr<FdoIFeatureReader> rdr, 
    FdoPtr<FdoIdentifierCollection> props,
    bool byExtents
)
{
    FdoStringP key1val = FixStringVal( rdr->GetString(phMgr->GetDcColumnName(L"key1")) );
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoStringP propName = byExtents ? FdoStringP(L"extents") : phMgr->GetDcColumnName(L"geometry");

    if ( wcscmp(key1val, L"KEY1_1" ) == 0 )
        VldGeogGeom( rdr, propName, 101, byExtents );
    if ( wcscmp(key1val, L"KEY1_2" ) == 0 )
        VldGeogGeom( rdr, propName, 2, byExtents );
    if ( wcscmp(key1val, L"KEY1_3" ) == 0 )
        VldGeogGeom( rdr, propName, 3, byExtents );
    if ( wcscmp(key1val, L"KEY1_4" ) == 0 )
        VldGeogGeom( rdr, propName, 4, byExtents );
    if ( wcscmp(key1val, L"KEY1_5" ) == 0 )
        VldGeogGeom( rdr, propName, 5, byExtents );
}

void SqlServerFdoUpdateTest::VldGeogGeom( 
    FdoPtr<FdoIFeatureReader> rdr, 
    FdoStringP propName,
    int idx,
    bool byExtents
)
{
    CPPUNIT_ASSERT( !rdr->IsNull(propName) );

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoByteArray> sGeom = rdr->GetGeometry( propName );
    CPPUNIT_ASSERT( sGeom );

    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf( sGeom );
    FdoPtr<FdoIGeometry> expectedGeom = CreateGeogGeom( idx );

    if ( byExtents ) {
        FdoPtr<FdoIEnvelope> geomExt = geom->GetEnvelope();
        FdoPtr<FdoIEnvelope> expectedExt = expectedGeom->GetEnvelope();
        CPPUNIT_ASSERT( geomExt->GetMinX() == expectedExt->GetMinX() );
        CPPUNIT_ASSERT( geomExt->GetMinY() == expectedExt->GetMinY() );
        CPPUNIT_ASSERT( geomExt->GetMaxX() == expectedExt->GetMaxX() );
        CPPUNIT_ASSERT( geomExt->GetMaxY() == expectedExt->GetMaxY() );
    }
    else {
        CPPUNIT_ASSERT( wcscmp(geom->GetText(), expectedGeom->GetText()) == 0 );
    }
}

void SqlServerFdoUpdateTest::TestRectangleFilter(
    FdoISelect * selectCommand,
    double xmin,
    double ymin,
    double xmax,
    double ymax,
    FdoSpatialOperations spatialOperation,
    FdoInt32 numRowsExpected)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope> filterEnv = gf->CreateEnvelopeXY(xmin, ymin, xmax, ymax);
    FdoPtr<FdoIGeometry> filterGeom = gf->CreateGeometry(filterEnv);
    FdoPtr<FdoByteArray> ba = gf->GetFgf(filterGeom);
    FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create(ba);
    FdoPtr<FdoSpatialCondition> pSpatialFilter =
        FdoSpatialCondition::Create(
        L"Geometry",
        spatialOperation,
        geomValue);

	selectCommand->SetFilter(pSpatialFilter);
    FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();
    int rowCount = 0;
    while ( rdr->ReadNext() )
        rowCount++;
    CPPUNIT_ASSERT( rowCount == numRowsExpected );
    rdr->Close();
}


FdoStringP SqlServerFdoUpdateTest::FixStringVal( FdoString* val )
{
    int i;


    for ( i = (wcslen(val) - 1); i > 0; i-- ) {
        if ( val[i] != ' ' ) 
            break;
    }

    FdoStringP ret = FdoStringP(val).Mid(0, i + 1 );
  
    return ret;
}

