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

