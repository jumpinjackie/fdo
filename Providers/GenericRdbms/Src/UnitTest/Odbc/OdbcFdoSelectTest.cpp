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
#include "OdbcFdoSelectTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoSelectTest, "FdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoSelectTest, "OdbcOracleFdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoSelectTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSelectTest, "FdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSelectTest, "OdbcMySqlFdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSelectTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSelectTest, "FdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSelectTest, "OdbcSqlServerFdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSelectTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSelectTest, "FdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSelectTest, "OdbcAccessFdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSelectTest, "OdbcAccessTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcExcelFdoSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoSelectTest, "FdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoSelectTest, "OdbcExcelFdoSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoSelectTest, "OdbcExcelTests");

bool OdbcSqlServerFdoSelectTest::DataSupportCreated = false;
#endif

bool OdbcMySqlFdoSelectTest::DataSupportCreated = false;
bool OdbcOracleFdoSelectTest::DataSupportCreated = false;

void OdbcOracleFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcOracle" );
}

void OdbcOracleFdoSelectTest::connect ()
{
    try
    {
		if (!OdbcOracleFdoSelectTest::DataSupportCreated)
		{
			OdbcOracleFdoSelectTest::DataSupportCreated = true;
			OdbcBaseSetup pOdbcSetup(DataBaseType_Oracle);
			FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_OraSetup, "");
			FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
			connection->SetConnectionString ( userConnectString);
			connection->Open();
			pOdbcSetup.CreateDataStore(connection.p, "");
			connection->Close();
		}

        mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }
}

void OdbcOracleFdoSelectTest::feature_query ()
{
	clock_t start, finish;
    FdoIFeatureReader *myReader = NULL;
    FdoISelect *selCmd = NULL;

	start = clock();
    if( mConnection != NULL )
    {
        try
        {
            selCmd = (FdoISelect*)mConnection->CreateCommand( FdoCommandType_Select );
            FdoStringP schemaName = FdoStringP(UnitTestUtil::GetEnviron("datastore")).Upper();
#ifdef _WIN32
            FdoStringP className = schemaName + L":ACDB3DPOLYLINE_LT";
#else
            // For Linux, Easysoft's default config does not report schema names.
            FdoStringP className = L"ACDB3DPOLYLINE_LT";
#endif
            selCmd->SetFeatureClassName(className);
            //FdoFilter* filterPtr = CreateFilter();
            //selCmd->SetFilter(filterPtr);

            //filterPtr->Release();
            myReader = selCmd->Execute();
            if( myReader != NULL  )
            {
                while ( myReader->ReadNext() )
                {
                    UnitTestUtil::ProcessFeature(myReader);

                    FdoClassDefinitionP classDef = myReader->GetClassDefinition();
                    FdoPropertiesP props = classDef->GetProperties();
                    FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = classDef->GetBaseProperties();

                    CPPUNIT_ASSERT( props->GetCount() == 19 );
                    CPPUNIT_ASSERT( baseProps->GetCount() == 0 );
                }
                myReader->Release();
            }
            selCmd->Release();
        }
        catch( FdoException *ex )
        {
            if( myReader )
                myReader->Release();

            selCmd->Release();
			CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
            throw;
        }
    }
	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}


void OdbcMySqlFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcMySql" );
}

void OdbcMySqlFdoSelectTest::connect ()
{
    try
    {
		if (!OdbcMySqlFdoSelectTest::DataSupportCreated)
		{
			OdbcMySqlFdoSelectTest::DataSupportCreated = true;
			FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
			connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_NoDatastore, "") );
			connection->Open();
			OdbcBaseSetup pSetup;
			pSetup.CreateDataStore(connection.p, "");
			pSetup.CreateAcadSchema(connection.p);
			connection->Close();
		}
		mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }
}

void OdbcMySqlFdoSelectTest::feature_query()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = L"acdb3dpolyline";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

#ifdef _WIN32
void OdbcSqlServerFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcSqlServer" );
}

void OdbcSqlServerFdoSelectTest::connect ()
{
    try
    {
		if (!OdbcSqlServerFdoSelectTest::DataSupportCreated)
		{
			OdbcSqlServerFdoSelectTest::DataSupportCreated = true;
			FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
			connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_NoDatastore, "") );
			connection->Open();
			OdbcBaseSetup pSetup(DataBaseType_SqlServer);
			pSetup.CreateDataStore(connection.p, "");
			pSetup.CreateAcadSchema(connection.p);
			connection->Close();
		}
        mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }

}

void OdbcSqlServerFdoSelectTest::feature_query()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = L"acdb3dpolyline";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcAccessFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoSelectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }

}

void OdbcAccessFdoSelectTest::Table1Test()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = GetSchemaName();
            fcn += L":";
            fcn += L"TABLE1";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
            FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcAccessFdoSelectTest::ComparisonFilterTable1Test()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = GetSchemaName();
            fcn += L":";
            fcn += L"TABLE1";
            selectCmd->SetFeatureClassName(fcn);

#if 0
            // create a comparison filter: Name == 'MyName'
            FdoPtr<FdoIdentifier>          dataPropID = FdoIdentifier::Create(L"NAME");
            FdoPtr<FdoDataValue>           dataValue  = FdoStringValue::Create(L"MyName");
            FdoPtr<FdoComparisonCondition> compCond   = FdoComparisonCondition::Create(
                dataPropID,
                FdoComparisonOperations_EqualTo,
                dataValue);

            // set the filter on the command
            selectCmd->SetFilter(compCond);
#else
            // create a comparison filter: Geometry within rectangle (8,8) to (17,17)
            FdoPtr<FdoIdentifier> geomPropID = FdoIdentifier::Create(L"Geometry");
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIEnvelope> pEnvelope = gf->CreateEnvelopeXY(8, 8, 17, 17);
            FdoPtr<FdoIGeometry> pGeometry = gf->CreateGeometry(pEnvelope);
            FdoPtr<FdoByteArray> pByteArray = gf->GetFgf(pGeometry);
            FdoPtr<FdoGeometryValue> pValue = FdoGeometryValue::Create(pByteArray);
            FdoPtr<FdoSpatialCondition> pSpatialCond = FdoSpatialCondition::Create(
                geomPropID, 
                FdoSpatialOperations_Within, 
                pValue);

            // set the filter on the command
            selectCmd->SetFilter(pSpatialCond);
#endif

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcAccessFdoSelectTest::RestrictedPropertiesTable1Test()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP className = GetSchemaName();
            className += L":" L"TABLE1";
            selectCmd->SetFeatureClassName(className);

            // restrict the set of properties to the named property
            FdoPtr<FdoIdentifier> propID1 = FdoIdentifier::Create(L"NAME");
            FdoPtr<FdoIdentifier> propID2 = FdoIdentifier::Create(L"Geometry");
            FdoPtr<FdoIdentifierCollection> propIDs = selectCmd->GetPropertyNames();
            propIDs->Add(propID1);
            propIDs->Add(propID2);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

// A test that is hard-coded for a known table.
// Table2 is notable for:
//    -- AutoNumber property type
//    -- Yes/No (boolean) property type
//    -- ordinate columns in the order (Y,X,...Z)
//    -- ordinate columns not as last columns
//    -- ordinate columns interleaved with non-ordinate columns
void OdbcAccessFdoSelectTest::Table2Test()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP className = GetSchemaName();
            className += L":" L"TABLE2";
            selectCmd->SetFeatureClassName(className);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 
            CPPUNIT_ASSERT_MESSAGE("Wrong class type from reader", classDef->GetClassType()==FdoClassType_FeatureClass);
            FdoFeatureClass* featureClassDef = static_cast<FdoFeatureClass*>(classDef.p);
            FdoPtr<FdoGeometricPropertyDefinition> geomPropertyDef = featureClassDef->GetGeometryProperty();
            CPPUNIT_ASSERT_MESSAGE("feature class missing geometry property", geomPropertyDef!=NULL);
            FdoPtr<FdoPropertyDefinitionCollection> propertyDefs = featureClassDef->GetProperties();
            FdoPtr<FdoPropertyDefinition> propertyDef = propertyDefs->FindItem(geomPropertyDef->GetName());
            CPPUNIT_ASSERT_MESSAGE("feature class missing geometry property", propertyDef!=NULL);

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);

                // Test geometry explicitly, since we'll skip testing it if it's missing from the reader's class definition
                if (!reader->IsNull(L"Geometry"))
                {
                    FdoPtr<FdoByteArray> bytes = reader->GetGeometry(L"Geometry");
                }
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();

            // Try another selection with restricted properties.
            // This is avoid any internal special handling of the all-column case.

            selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            selectCmd->SetFeatureClassName(className);

            // restrict the set of properties to the named property
            FdoPtr<FdoIdentifier> propID1 = FdoIdentifier::Create(L"Geometry");
            FdoPtr<FdoIdentifierCollection> propIDs = selectCmd->GetPropertyNames();
            propIDs->Add(propID1);

            // execute the command
            reader = selectCmd->Execute();
            // read through all the features
            numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcAccessFdoSelectTest::TestDefect779194()
{
    if( mConnection != NULL )
    {
        try
        {
            // Make sure that GetPropertyCount succeeds when no identifiers are specified.

            FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
            FdoStringP className = GetSchemaName();
            className += L":" L"EMPLOYEES";
            selectAggrCmd->SetFeatureClassName(className);

            // Execute the command:
            FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();
            FdoInt32 propertyCount = dataReader->GetPropertyCount();
            CPPUNIT_ASSERT_MESSAGE("Expected some columns, got zero", propertyCount>0);
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcExcelFdoSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcExcel" );
}

void OdbcExcelFdoSelectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }

}

void OdbcExcelFdoSelectTest::AllTypesTest()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = GetSchemaName();
            fcn += L":";
            fcn += L"ALLTYPES";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
            FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}

void OdbcExcelFdoSelectTest::AllTypesConfigFileTest()
{
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");
        connection->SetConfiguration(fileStream);

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);

        // must set the feature class name
        FdoStringP fcn = L"Fdo";
        fcn += L":";
        fcn = L"ALLTYPES";
        selectCmd->SetFeatureClassName(fcn);

        // execute the command
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

        FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
        CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
        FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

        // read through all the features
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
        connection->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}

void OdbcExcelFdoSelectTest::AllTypesConfigFileTest_defect814052()
{
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        // Use the config file that does not define an identifer property.
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile2(), L"r");
        connection->SetConfiguration(fileStream);

        connection->SetConnectionString(GetConnectString());
        connection->Open();
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);

        FdoStringP fcn = L"Fdo";
        fcn += L":";
        fcn = L"ALLTYPES";
        selectCmd->SetFeatureClassName(fcn);

        // execute the command
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

        FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
        CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
        FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

        // read through all the features
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
        connection->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}

void OdbcExcelFdoSelectTest::CityTest()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = GetSchemaName();
            fcn += L":";
            fcn += L"CITY";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            FdoDataPropertiesP idProps = classDef->GetIdentityProperties();
            
            CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
            CPPUNIT_ASSERT_MESSAGE("Class should not have identity, otherwise need to try another class", idProps->GetCount() == 0);
            FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                if ( numFeatures < 5 ) 
                    printf( "%ls, %ls\n", reader->GetString(L"NAME"), reader->GetString(L"COUNTRY") );
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            UnitTestUtil::fail (e);
        }
    }
}
#endif
