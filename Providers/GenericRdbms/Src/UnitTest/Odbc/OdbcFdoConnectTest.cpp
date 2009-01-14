/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "OdbcFdoConnectTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoConnectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoConnectTest, "FdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoConnectTest, "OdbcMySqlFdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoConnectTest, "OdbcMySqlTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoConnectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoConnectTest, "FdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoConnectTest, "OdbcOracleFdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoConnectTest, "OdbcOracleTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoConnectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoConnectTest, "FdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoConnectTest, "OdbcAccessFdoConnectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoConnectTest, "OdbcAccessTests");
#endif

void OdbcMySqlFdoConnectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcMySql" );
}

void OdbcMySqlFdoConnectTest::ConnectByDsn()
{
    try
    {
		FdoPtr<FdoIConnection> connection;
		OdbcBaseSetup pOdbcSetup;
		try
		{
			FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
			// Create/open primary connection:
			connection = UnitTestUtil::GetProviderConnectionObject();
			connection->SetConnectionString ( userConnectString);
			connection->Open();
			pOdbcSetup.CreateDataStore(connection.p, L"");
			connection->Close();
		}
		catch(...){}

        // Now open the database with the given 
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        // Now check the schema.
        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        FdoInt32 numSchemas = schemas->GetCount();
        for (int i=0; i<numSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();
            FdoString* schemaDesc = schema->GetDescription();
	    wprintf(L"Schema '%ls' '%ls'\n", schemaName, schemaDesc);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            for (int j=0; j<numClasses; j++)
            {
                // Note the assumption here that it is a feature class (it is set up so
                // in the test data).
                //FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);
                FdoPtr<FdoClassDefinition> featureClass = classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                FdoString* classDesc = featureClass->GetDescription();
		wprintf(L"  Class '%ls' '%ls'\n", className, classDesc);
                FdoClassType classType = featureClass->GetClassType();
#if 0
                // Test the ClassCapabilities for this class:
                if (!HasGeometry(connection, featureClass->GetName()))
                    CPPUNIT_FAIL("FAILED - class is missing its Geometry property\n");
                FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClass->GetGeometryProperty();
                wprintf(L"Override geometry property '%ls' '%ls'\n", geomProp->GetName(), geomProp->GetDescription());
#endif
                FdoPtr<FdoDataPropertyDefinitionCollection> identityProps = featureClass->GetIdentityProperties();
                FdoPtr<FdoPropertyDefinitionCollection> classProps = featureClass->GetProperties();

                FdoInt32 numProps = classProps->GetCount();
                for (int k=0; k<numProps; k++)
                {
                    FdoPtr<FdoPropertyDefinition> propDef = (FdoPropertyDefinition*)classProps->GetItem(k);

                    FdoString* propName = propDef->GetName();
                    FdoString* propDesc = propDef->GetDescription();
        	    wprintf(L"    Property '%ls' '%ls'\n", propName, propDesc);
                    FdoPropertyType propType = propDef->GetPropertyType();

                    if (propType == FdoPropertyType_GeometricProperty)
                    {
                        FdoGeometricPropertyDefinition* geomPropDef = (FdoGeometricPropertyDefinition*)propDef.p;

                        FdoInt32 geomTypes = geomPropDef->GetGeometryTypes();
                        bool geomHasElevation = geomPropDef->GetHasElevation();
                        bool geomHasMeasure = geomPropDef->GetHasMeasure();
                        bool geomReadOnly = geomPropDef->GetReadOnly();
                    }

                    if (propType == FdoPropertyType_DataProperty)
                    {
                        FdoDataPropertyDefinition* dataPropDef = (FdoDataPropertyDefinition*)propDef.p;

                        FdoDataType dataDataType = dataPropDef->GetDataType();
                        FdoString* dataDefaultValue = dataPropDef->GetDefaultValue();
                        FdoInt32 dataLength = dataPropDef->GetLength();
                        bool dataNullable = dataPropDef->GetNullable();
                        FdoInt32 dataPrecision = dataPropDef->GetPrecision();
                        FdoInt32 dataScale = dataPropDef->GetScale();
                    }
                }
            }
        }

#if 0
        // Read the features too.  While this hard-codes the class name,
        // note that it is the only one reported earlier (e.g. other tables are
        // invisible with the configuration file in place).
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"Acdb:Polyline");
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }
        printf("   %i feature(s) read\n", numFeatures);
        reader->Close();
#endif
        // This is not currently written to a file for checking, but it is handy
        // for debugging.
        FdoIoMemoryStreamP schemaStream = FdoIoMemoryStream::Create();
        schemas->WriteXml(schemaStream);
		pOdbcSetup.DestroyDataStore(connection.p, L"");
		connection->Close();
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

void OdbcOracleFdoConnectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcOracle" );
}

void OdbcOracleFdoConnectTest::ConfigFileTest()
{
	OdbcBaseSetup pOdbcSetup(DataBaseType_Oracle);
	try
	{
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_OraSetup);
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( userConnectString);
		connection->Open();
		pOdbcSetup.CreateDataStore(connection.p, L"");
		connection->Close();
	}
    catch (FdoException *ex)
	{
		ex->Release();
	}
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile2(), L"r");

        // Import the configuration file.
        connection->SetConfiguration(fileStream);

        // Now open the database with the given 
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        // Now analyse the schema with the mappings in place.
        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        FdoInt32 numSchemas = schemas->GetCount();
        for (int i=0; i<numSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();
            FdoString* schemaDesc = schema->GetDescription();
			wprintf(L"Override schema '%ls' '%ls'\n", schemaName, schemaDesc);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            for (int j=0; j<numClasses; j++)
            {
                // Note the assumption here that it is a feature class (it is set up so
                // in the test data).
                FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                FdoString* classDesc = featureClass->GetDescription();
    			wprintf(L"Override class '%ls' '%ls'\n", className, classDesc);
                FdoClassType classType = featureClass->GetClassType();
                // Test the ClassCapabilities for this class:
                if (!HasGeometry(connection, featureClass->GetName()))
                    CPPUNIT_FAIL("FAILED - class is missing its Geometry property\n");
                FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClass->GetGeometryProperty();
                wprintf(L"Override geometry property '%ls' '%ls'\n", geomProp->GetName(), geomProp->GetDescription());
                FdoPtr<FdoDataPropertyDefinitionCollection> identityProps = featureClass->GetIdentityProperties();
                FdoPtr<FdoPropertyDefinitionCollection> classProps = featureClass->GetProperties();

                FdoInt32 numProps = classProps->GetCount();
                for (int k=0; k<numProps; k++)
                {
                    FdoPtr<FdoPropertyDefinition> propDef = (FdoPropertyDefinition*)classProps->GetItem(k);

                    FdoString* propName = propDef->GetName();
                    FdoString* propDesc = propDef->GetDescription();
        			wprintf(L"Override property '%ls' '%ls'\n", propName, propDesc);
                    FdoPropertyType propType = propDef->GetPropertyType();

                    if (propType == FdoPropertyType_GeometricProperty)
                    {
                        FdoGeometricPropertyDefinition* geomPropDef = (FdoGeometricPropertyDefinition*)propDef.p;

                        FdoInt32 geomTypes = geomPropDef->GetGeometryTypes();
                        bool geomHasElevation = geomPropDef->GetHasElevation();
                        bool geomHasMeasure = geomPropDef->GetHasMeasure();
                        bool geomReadOnly = geomPropDef->GetReadOnly();
                    }

                    if (propType == FdoPropertyType_DataProperty)
                    {
                        FdoDataPropertyDefinition* dataPropDef = (FdoDataPropertyDefinition*)propDef.p;

                        FdoDataType dataDataType = dataPropDef->GetDataType();
                        FdoString* dataDefaultValue = dataPropDef->GetDefaultValue();
                        FdoInt32 dataLength = dataPropDef->GetLength();
                        bool dataNullable = dataPropDef->GetNullable();
                        FdoInt32 dataPrecision = dataPropDef->GetPrecision();
                        FdoInt32 dataScale = dataPropDef->GetScale();
                    }
                }
            }
        }

        // Read the features too.  Note that these hard-coded class names
        // "Polyline" and "Table1" are the only ones reported earlier
        // (e.g. other tables are invisible with the configuration file in place).
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"Acdb:Polyline");
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }
        printf("   %i feature(s) read from Polyline\n", numFeatures);
        reader->Close();

        selectCmd->SetFeatureClassName(L"Acdb:Table1");
        reader = selectCmd->Execute();
        numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }
        printf("   %i feature(s) read from Table1\n", numFeatures);
        reader->Close();

        // This is not currently written to a file for checking, but it is handy
        // for debugging.
        FdoIoMemoryStreamP schemaStream = FdoIoMemoryStream::Create();
        schemas->WriteXml(schemaStream);
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

void OdbcOracleFdoConnectTest::AllDatabaseTypesTest()
{
	OdbcBaseSetup pOdbcSetup(DataBaseType_Oracle);
	try
	{
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_OraSetup);
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( userConnectString);
		connection->Open();
		pOdbcSetup.CreateDataStore(connection.p, L"");
		connection->Close();
	}
    catch (FdoException *ex)
	{
		ex->Release();
	}
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        // Inspect tables that collectively contain has at least one column of all the database'
        // built-in column types that a client can create.  Multiple tables are used due to 
        // Oracle's restrictions on multiple LONG column types in one table.

        // First, check incremental class loading by doing a Select on each one.

        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ALLDBTYPES");
        FdoPtr<FdoIFeatureReader> readerIncr = selectCmd->Execute();
        FdoPtr<FdoClassDefinition> classDefIncr = readerIncr->GetClassDefinition();
        AssertAllDatabaseTypes(classDefIncr);

        selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ALLDBTYPES2");
        readerIncr = selectCmd->Execute();
        classDefIncr = readerIncr->GetClassDefinition();
        AssertAllDatabaseTypes(classDefIncr);

        selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"ALLDBTYPES3");
        readerIncr = selectCmd->Execute();
        classDefIncr = readerIncr->GetClassDefinition();
        AssertAllDatabaseTypes(classDefIncr);

        readerIncr->Close();

        // Now re-open the database (to get rid of any partial schema loading)
        // and get a full schema description.

        connection->Close();
        connection->Open();

        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        bool foundAllTypesClass = false;
        bool foundAllTypesClass2 = false;
        bool foundAllTypesClass3 = false;
        FdoInt32 numSchemas = schemas->GetCount();

        for (int i=0; i<numSchemas && !foundAllTypesClass; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            for (int j=0; j<numClasses; j++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(j);

                // analyze the feature class
                FdoString* className = classDef->GetName();
                if (wcscmp(className, L"ALLDBTYPES")==0 ||
                    wcscmp(className, L"ALLDBTYPES2")==0 ||
                    wcscmp(className, L"ALLDBTYPES3")==0)
                {
                    if (wcscmp(className, L"ALLDBTYPES")==0)    foundAllTypesClass = true;
                    if (wcscmp(className, L"ALLDBTYPES2")==0)   foundAllTypesClass2 = true;
                    if (wcscmp(className, L"ALLDBTYPES3")==0)   foundAllTypesClass3 = true;

                    AssertAllDatabaseTypes(classDef);

                    // Select from the table and check the reader's class 
                    // definition too.
                    FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
                    selectCmd->SetFeatureClassName(className);
                    FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
                    FdoPtr<FdoClassDefinition> classDef2 = reader->GetClassDefinition();
                    AssertAllDatabaseTypes(classDef2);
                }
            }
        }

        if (!foundAllTypesClass)
            CPPUNIT_FAIL("FAILED - did not find class ALLDBTYPES\n");
        if (!foundAllTypesClass2)
            CPPUNIT_FAIL("FAILED - did not find class ALLDBTYPES2\n");
        if (!foundAllTypesClass3)
            CPPUNIT_FAIL("FAILED - did not find class ALLDBTYPES3\n");

    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

struct OdbcOracleAllDatabaseTypesMapEntry
{
    FdoStringP      propName;
    FdoDataType     dataType;
    FdoInt32        length;             // -1 == don't care
    FdoInt32        precision;          // -1 == don't care
    FdoInt32        scale;              // -1 == don't care
};

// Any column types that are not in these lists will not appear in FDO
// class definitions (schema manager skips unsupported types).
// Multiple tables are used because Oracle has restrictions on how 
// many "long" columns may appear in one table.
static OdbcOracleAllDatabaseTypesMapEntry OdbcOracleAllDatabaseTypesMap_S[] =
{
    { L"DBTYPE_VARCHAR2",       FdoDataType_String,     10, -1, -1 },
    { L"DBTYPE_NUMBER",         FdoDataType_Double,     -1,  0,  0 },
    { L"DBTYPE_NUMBER_20_0",    FdoDataType_Decimal,    -1, 20,  0 },
    { L"DBTYPE_NUMBER_27_5",    FdoDataType_Decimal,    -1, 27,  5 },
    { L"DBTYPE_INTEGER",        FdoDataType_Decimal,    -1, 38,  0 },
    { L"DBTYPE_SMALLINT",       FdoDataType_Decimal,    -1, 38,  0 },
    { L"DBTYPE_DECIMAL",        FdoDataType_Decimal,    -1, 38,  0 },
    { L"DBTYPE_DECIMAL_26",     FdoDataType_Decimal,    -1, 26,  0 },
    { L"DBTYPE_NUMERIC",        FdoDataType_Decimal,    -1, 38,  0 },
    { L"DBTYPE_NUMERIC_25_3",   FdoDataType_Decimal,    -1, 25,  3 },
    { L"DBTYPE_FLOAT",          FdoDataType_Double,     -1,  0,  0 },
    { L"DBTYPE_REAL",           FdoDataType_Double,     -1,  0,  0 },
    { L"DBTYPE_VARCHAR",        FdoDataType_String,     10,  0,  0 },
    { L"DBTYPE_DATE",           FdoDataType_DateTime,    0,  0,  0 },
    { L"DBTYPE_CHAR",           FdoDataType_String,      1,  0,  0 },
    { L"DBTYPE_TIMESTAMP",      FdoDataType_String,      0,  0,  0 },
};
static OdbcOracleAllDatabaseTypesMapEntry OdbcOracleAllDatabaseTypesMap2_S[] =
{
    { L"DBTYPE_VARCHAR2",       FdoDataType_String,     10, -1, -1 },
    { L"DBTYPE_LONG",           FdoDataType_String, 1073741824,  0,  0 }
};
static OdbcOracleAllDatabaseTypesMapEntry OdbcOracleAllDatabaseTypesMap3_S[] =
{
    { L"DBTYPE_VARCHAR2",       FdoDataType_String,     10, -1, -1 },
    { L"DBTYPE_LONGVARCHAR",    FdoDataType_String, 1073741824,  0,  0 }
};

static FdoInt32 OdbcOracleAllDatabaseTypesMapSize_S = 
    sizeof(OdbcOracleAllDatabaseTypesMap_S) / sizeof(OdbcOracleAllDatabaseTypesMap_S[0]);
static FdoInt32 OdbcOracleAllDatabaseTypesMapSize2_S = 
    sizeof(OdbcOracleAllDatabaseTypesMap2_S) / sizeof(OdbcOracleAllDatabaseTypesMap2_S[0]);
static FdoInt32 OdbcOracleAllDatabaseTypesMapSize3_S = 
    sizeof(OdbcOracleAllDatabaseTypesMap3_S) / sizeof(OdbcOracleAllDatabaseTypesMap3_S[0]);

void OdbcOracleFdoConnectTest::AssertAllDatabaseTypes(FdoClassDefinition * classDef)
{
    FdoInt32 numPropertiesMatchedToMap = 0;

    FdoString * className = classDef->GetName();

    OdbcOracleAllDatabaseTypesMapEntry * typeMap = NULL;
    FdoInt32 typeMapSize = 0;
    if (wcscmp(className, L"ALLDBTYPES")==0)
    {
        typeMap = OdbcOracleAllDatabaseTypesMap_S;
        typeMapSize = OdbcOracleAllDatabaseTypesMapSize_S;
    }
    else if (wcscmp(className, L"ALLDBTYPES2")==0)
    {
        typeMap = OdbcOracleAllDatabaseTypesMap2_S;
        typeMapSize = OdbcOracleAllDatabaseTypesMapSize2_S;
    }
    else if (wcscmp(className, L"ALLDBTYPES3")==0)
    {
        typeMap = OdbcOracleAllDatabaseTypesMap3_S;
        typeMapSize = OdbcOracleAllDatabaseTypesMapSize3_S;
    }
    else
    {
        CPPUNIT_FAIL("FAILED - no type map found\n");
    }

    printf("Checking column type conversion for class '%ls'...\n", className);

    FdoPtr<FdoPropertyDefinitionCollection> classProps = classDef->GetProperties();

    FdoInt32 numProps = classProps->GetCount();
    for (FdoInt32 i=0; i<numProps; i++)
    {
        FdoPtr<FdoPropertyDefinition> propDef = classProps->GetItem(i);

        FdoString* propName = propDef->GetName();
        FdoPropertyType propType = propDef->GetPropertyType();

        if (FdoPropertyType_DataProperty != propType)
            CPPUNIT_FAIL("FAILED - unexpectedly encountered a property of type other than Data\n");

        FdoDataPropertyDefinition* dataPropDef = (FdoDataPropertyDefinition*)propDef.p;

        FdoDataType dataDataType = dataPropDef->GetDataType();
        FdoInt32 dataLength = dataPropDef->GetLength();
        FdoInt32 dataPrecision = dataPropDef->GetPrecision();
        FdoInt32 dataScale = dataPropDef->GetScale();

        printf("    Property '%ls' length=%d precision=%d scale=%d\n", 
            propName, dataLength, dataPrecision, dataScale);

        bool foundTypeMapEntry = false;
        for (FdoInt32 j=0;
            j < typeMapSize && !foundTypeMapEntry;
            j++)
        {
            OdbcOracleAllDatabaseTypesMapEntry * mapEntry = &typeMap[j];
            if (wcscmp(propName, mapEntry->propName)==0)
            {
                foundTypeMapEntry = true;
                numPropertiesMatchedToMap++;
                if (mapEntry->length != -1)
                    CPPUNIT_ASSERT( dataLength == mapEntry->length );
                if (mapEntry->precision != -1)
                    CPPUNIT_ASSERT( dataPrecision == mapEntry->precision );
                if (mapEntry->scale != -1)
                    CPPUNIT_ASSERT( dataScale == mapEntry->scale );
            }
        }
        if (!foundTypeMapEntry)
            CPPUNIT_FAIL("FAILED - mismatch while checking properties in class ALLDBTYPES\n");
    }
    if (numPropertiesMatchedToMap != typeMapSize)
        CPPUNIT_FAIL("FAILED - mismatch while checking all properties in class ALLDBTYPES\n");
}


#ifdef _WIN32
void OdbcAccessFdoConnectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoConnectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
		mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection= NULL;
        TestCommonFail (ex);
    }
}

// tests static CreateConnection method
void OdbcAccessFdoConnectTest::StaticCreateTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // call it again to verify we can create two connections
        FdoPtr<FdoIConnection> connection2 = UnitTestUtil::GetProviderConnectionObject();
        if (connection2 == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

// tests setting the connection string directly
void OdbcAccessFdoConnectTest::StringTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

		connection->SetConnectionString(GetConnectString());
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

// Test importing and exporting the configuration file.
void OdbcAccessFdoConnectTest::ConfigFileTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");

        // Import the configuration file.
        connection->SetConfiguration(fileStream);

        // Now open the database with the given 
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        // Now analyse the schema with the mappings in place.
        // This is a modified version of DescribeSchemaTest::SchemaTest().
        FdoPtr<FdoIDescribeSchema> describeSchemaCmd =
            (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        if (schemas == NULL)
            CPPUNIT_FAIL("FAILED - DescribeSchema returned NULL collection\n");

        FdoInt32 numSchemas = schemas->GetCount();
        for (int i=0; i<numSchemas; i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();
            FdoString* schemaDesc = schema->GetDescription();
			wprintf(L"Override schema '%ls' '%ls'\n", schemaName, schemaDesc);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            for (int j=0; j<numClasses; j++)
            {
                // Note the assumption here that it is a feature class (it is set up so
                // in the test data).
                FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                FdoString* classDesc = featureClass->GetDescription();
    			wprintf(L"Override class '%ls' '%ls'\n", className, classDesc);
                FdoClassType classType = featureClass->GetClassType();
                // Test the ClassCapabilities for this class:
                if (!HasGeometry(connection, featureClass->GetName()))
                    CPPUNIT_FAIL("FAILED - class is missing its Geometry property\n");
                FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClass->GetGeometryProperty();
                wprintf(L"Override geometry property '%ls' '%ls'\n", geomProp->GetName(), geomProp->GetDescription());
                FdoPtr<FdoDataPropertyDefinitionCollection> identityProps = featureClass->GetIdentityProperties();
                FdoPtr<FdoPropertyDefinitionCollection> classProps = featureClass->GetProperties();

                FdoInt32 numProps = classProps->GetCount();
                for (int k=0; k<numProps; k++)
                {
                    FdoPtr<FdoPropertyDefinition> propDef = (FdoPropertyDefinition*)classProps->GetItem(k);

                    FdoString* propName = propDef->GetName();
                    FdoString* propDesc = propDef->GetDescription();
        			wprintf(L"Override property '%ls' '%ls'\n", propName, propDesc);
                    FdoPropertyType propType = propDef->GetPropertyType();

                    if (propType == FdoPropertyType_GeometricProperty)
                    {
                        FdoGeometricPropertyDefinition* geomPropDef = (FdoGeometricPropertyDefinition*)propDef.p;

                        FdoInt32 geomTypes = geomPropDef->GetGeometryTypes();
                        bool geomHasElevation = geomPropDef->GetHasElevation();
                        bool geomHasMeasure = geomPropDef->GetHasMeasure();
                        bool geomReadOnly = geomPropDef->GetReadOnly();
                    }

                    if (propType == FdoPropertyType_DataProperty)
                    {
                        FdoDataPropertyDefinition* dataPropDef = (FdoDataPropertyDefinition*)propDef.p;

                        FdoDataType dataDataType = dataPropDef->GetDataType();
                        FdoString* dataDefaultValue = dataPropDef->GetDefaultValue();
                        FdoInt32 dataLength = dataPropDef->GetLength();
                        bool dataNullable = dataPropDef->GetNullable();
                        FdoInt32 dataPrecision = dataPropDef->GetPrecision();
                        FdoInt32 dataScale = dataPropDef->GetScale();
                    }
                }
            }
        }

        // Read the features too.  While this hard-codes the class name,
        // note that it is the only one reported earlier (e.g. Table1 is
        // invisible with the configuration file in place).
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(L"hydro:Sounding");
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessFeature(reader);
        }
        printf("   %i feature(s) read\n", numFeatures);
        reader->Close();

        // This is not currently written to a file for checking, but it is handy
        // for debugging.
        FdoIoMemoryStreamP schemaStream = FdoIoMemoryStream::Create();
        schemas->WriteXml(schemaStream);
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }

    try
    {
        // Use a variation of the configuration file that omits geometry.  This tests
        // the fix for defect 680408.
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile2(), L"r");

        connection->SetConfiguration(fileStream);

        connection->SetConnectionString(UnitTestUtil::GetConnectionString());
        connection->Open();

        if (HasGeometry(connection, L"Sounding"))
            CPPUNIT_FAIL("FAILED - hydro:Sounding has a Geometry property; none was expected with this override\n");
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}



// tests opening connection
void OdbcAccessFdoConnectTest::OpenTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

#ifdef _WIN32
        // try opening the connection
        connection->SetConnectionString(L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\"");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;ConnectionString=;GenerateDefaultGeometryProperty=;");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;GenerateDefaultGeometryProperty=;ConnectionString=;");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;GenerateDefaultGeometryProperty=;ConnectionString=");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;ConnectionString=\"\";GenerateDefaultGeometryProperty=;");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;GenerateDefaultGeometryProperty=;ConnectionString=\"\";");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=;Password=;GenerateDefaultGeometryProperty=;ConnectionString=\"\"");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=;UserId=;Password=;ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\"");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\";DataSourceName=;UserId=;Password=;");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=;ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\";UserId=;Password=;");
        connection->Open();
        connection->Close();

        // try opening the connection
        connection->SetConnectionString(L"DataSourceName=; ConnectionString = \"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\" ; UserId=;Password=;");
        connection->Open();
        if (!HasGeometry(connection, L"TABLE1"))
            CPPUNIT_FAIL("FAILED - TABLE1 is missing its Geometry property\n");
        if (!HasGeometry(connection, L"Table4"))
            CPPUNIT_FAIL("FAILED - Table4 is missing its Geometry property\n");
        connection->Close();

        // Try the connection with default geometry property generation specified as "true".
        connection->SetConnectionString(L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\";GenerateDefaultGeometryProperty=true");
        connection->Open();
        if (!HasGeometry(connection, L"TABLE1"))
            CPPUNIT_FAIL("FAILED - TABLE1 is missing its Geometry property\n");
        if (!HasGeometry(connection, L"Table4"))
            CPPUNIT_FAIL("FAILED - Table4 is missing its Geometry property\n");
        connection->Close();

        // Try the connection with default geometry property generation specified as "false".
        connection->SetConnectionString(L"ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\";GenerateDefaultGeometryProperty=false");
        connection->Open();
        if (HasGeometry(connection, L"TABLE1"))
            CPPUNIT_FAIL("FAILED - TABLE1 should not have a Geometry property when GenerateDefaultGeometryProperty=false\n");
        if (!HasGeometry(connection, L"Table4"))    // Table4 really has a column called "Geometry".
            CPPUNIT_FAIL("FAILED - Table4 is missing its Geometry property\n");
        connection->Close();

#endif // _WIN32
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}



// tests connection capabilities
void OdbcAccessFdoConnectTest::CapabilitiesTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // check the connection capabilities
        FdoPtr<FdoIConnectionCapabilities> capConnection = connection->GetConnectionCapabilities();
        if (capConnection == NULL)
            CPPUNIT_FAIL("FAILED - GetConnectionCapabilities returned NULL\n");
        CPPUNIT_ASSERT_MESSAGE("Incorrect ThreadCapability.", capConnection->GetThreadCapability() == FdoThreadCapability_PerConnectionThreaded);
        FdoInt32 numSceTypes = 0;
        FdoSpatialContextExtentType * sceTypes = capConnection->GetSpatialContextTypes(numSceTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of spatial context extent types.", numSceTypes == 1);
        CPPUNIT_ASSERT_MESSAGE("Incorrect spatial context extent type.", sceTypes[0] == FdoSpatialContextExtentType_Static);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsLocking capability.", capConnection->SupportsLocking() == false);
        FdoInt32 numLockTypes = 0;
        FdoLockType * lockTypes = capConnection->GetLockTypes(numLockTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of spatial context extent types.", numLockTypes == 0 && lockTypes == NULL);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsTimeout capability.", capConnection->SupportsTimeout() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsTransactions capability.", capConnection->SupportsTransactions() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsLongTransactions capability.", capConnection->SupportsLongTransactions() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsSQL capability.", capConnection->SupportsSQL() == true);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsConfiguration capability.", capConnection->SupportsConfiguration() == true);

        // check the geometry capabilities
        FdoPtr<FdoIGeometryCapabilities> capGeometries = connection->GetGeometryCapabilities();
        if (capGeometries == NULL)
            CPPUNIT_FAIL("FAILED - GetGeometryCapabilities returned NULL\n");
        FdoInt32 capGeomDimensions = capGeometries->GetDimensionalities();
        CPPUNIT_ASSERT_MESSAGE("Incorrect dimensionalities.", capGeomDimensions == (FdoDimensionality_XY | FdoDimensionality_Z));
        FdoInt32 capGeomTypeCount;
        FdoGeometryType* capGeomTypes = capGeometries->GetGeometryTypes(capGeomTypeCount);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of geometry types.", capGeomTypeCount == 1);
        CPPUNIT_ASSERT_MESSAGE("Incorrect geometry type.", capGeomTypes[0] == FdoGeometryType_Point);
        FdoInt32 capGeomComponentTypeCount;
        FdoGeometryComponentType* capGeomComponentTypes = capGeometries->GetGeometryComponentTypes(capGeomComponentTypeCount);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of geometry component types.", capGeomComponentTypeCount == 0 && capGeomComponentTypes == NULL);

        // check the schema capabilities
        FdoPtr<FdoISchemaCapabilities> capSchema = connection->GetSchemaCapabilities();
        if (capSchema == NULL)
            CPPUNIT_FAIL("FAILED - GetSchemaCapabilities returned NULL\n");
        FdoInt32 numClassTypes = 0;
        FdoClassType * classTypes = capSchema->GetClassTypes(numClassTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of class types.", numClassTypes == 2);
        CPPUNIT_ASSERT_MESSAGE("Incorrect class types.",
            classTypes[0] == FdoClassType_Class && classTypes[1] == FdoClassType_FeatureClass);
        FdoInt32 numDataTypes = 0;
        FdoDataType * dataTypes = capSchema->GetDataTypes(numDataTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of data types.", numDataTypes == 10);
        CPPUNIT_ASSERT_MESSAGE("Incorrect data types.",
            dataTypes[0] == FdoDataType_Boolean &&
            dataTypes[1] == FdoDataType_Byte &&
            dataTypes[2] == FdoDataType_DateTime &&
            dataTypes[3] == FdoDataType_Decimal &&
            dataTypes[4] == FdoDataType_Double &&
            dataTypes[5] == FdoDataType_Int16 &&
            dataTypes[6] == FdoDataType_Int32 &&
            dataTypes[7] == FdoDataType_Int64 &&
            dataTypes[8] == FdoDataType_Single &&
            dataTypes[9] == FdoDataType_String);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsInheritance capability.", capSchema->SupportsInheritance() == true);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsMultipleSchemas capability.", capSchema->SupportsMultipleSchemas() == true);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsObjectProperties capability.", capSchema->SupportsObjectProperties() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsSchemaOverrides capability.", capSchema->SupportsSchemaOverrides() == true);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsAssociationProperties capability.", capSchema->SupportsAssociationProperties() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsNetworkModel capability.", capSchema->SupportsNetworkModel() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsAutoIdGeneration capability.", capSchema->SupportsAutoIdGeneration() == true);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsDataStoreScopeUniqueIdGeneration capability.", capSchema->SupportsDataStoreScopeUniqueIdGeneration() == false);
        dataTypes = capSchema->GetSupportedAutoGeneratedTypes(numDataTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of auto-generated types.", numDataTypes == 3);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsSchemaModification capability.", capSchema->SupportsSchemaModification() == false);

        // check the command capabilities
        FdoPtr<FdoICommandCapabilities> capCommand = connection->GetCommandCapabilities();
        if (capCommand == NULL)
            CPPUNIT_FAIL("FAILED - GetCommandCapabilities returned NULL\n");
        FdoInt32 numCommands = 0;
        FdoInt32 * commands = capCommand->GetCommands(numCommands);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of commands.", numCommands == 11);
        CPPUNIT_ASSERT_MESSAGE("Incorrect commands.",
            commands[0] == FdoCommandType_Select &&
            commands[1] == FdoCommandType_SelectAggregates &&
            commands[2] == FdoCommandType_DescribeSchema &&
            commands[3] == FdoCommandType_DescribeSchemaMapping &&
            commands[4] == FdoCommandType_SQLCommand &&
            commands[5] == FdoCommandType_Insert &&
            commands[6] == FdoCommandType_Delete &&
            commands[7] == FdoCommandType_Update &&
            commands[8] == FdoCommandType_GetSpatialContexts &&
            commands[9] == FdoCommandType_GetClassNames &&
            commands[10] == FdoCommandType_GetSchemaNames);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsParameters capability.", capCommand->SupportsParameters() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsTimeout capability.", capCommand->SupportsTimeout() == false);

        // check the filter capabilities
        FdoPtr<FdoIFilterCapabilities> capFilter= connection->GetFilterCapabilities();
        if (capFilter == NULL)
            CPPUNIT_FAIL("FAILED - GetFilterCapabilities returned NULL\n");
        FdoInt32 numConditionTypes = 0;
        FdoConditionType * conditionTypes = capFilter->GetConditionTypes(numConditionTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of ConditionTypes.", numConditionTypes == 5);
        CPPUNIT_ASSERT_MESSAGE("Incorrect ConditionTypes.",
            conditionTypes[0] == FdoConditionType_Comparison &&
            conditionTypes[1] == FdoConditionType_Like &&
            conditionTypes[2] == FdoConditionType_In &&
            conditionTypes[3] == FdoConditionType_Null &&
            conditionTypes[4] == FdoConditionType_Spatial);
        FdoInt32 numSpatialOperations = 0;
        FdoSpatialOperations * spatialOperations = capFilter->GetSpatialOperations(numSpatialOperations);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of SpatialOperations.", numSpatialOperations == 4);
        CPPUNIT_ASSERT_MESSAGE("Incorrect SpatialOperations.",
            spatialOperations[0] == FdoSpatialOperations_Intersects &&
            spatialOperations[1] == FdoSpatialOperations_Within &&
            spatialOperations[2] == FdoSpatialOperations_EnvelopeIntersects &&
            spatialOperations[3] == FdoSpatialOperations_Inside);
        FdoInt32 numDistanceOperations = 0;
        FdoDistanceOperations * distancelOperations = capFilter->GetDistanceOperations(numDistanceOperations);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of DistanceOperations.", numDistanceOperations == 0 && distancelOperations == NULL);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsGeodesicDistance capability.", capFilter->SupportsGeodesicDistance() == false);
        CPPUNIT_ASSERT_MESSAGE("Wrong SupportsNonLiteralGeometricOperations capability.", capFilter->SupportsNonLiteralGeometricOperations() == false);

        // check the expression capabilities
        FdoPtr<FdoIExpressionCapabilities> capExpression= connection->GetExpressionCapabilities();
        if (capExpression == NULL)
            CPPUNIT_FAIL("FAILED - GetExpressionCapabilities returned NULL\n");
        FdoInt32 numExpressionTypes = 0;
        FdoExpressionType * expressionTypes = capExpression->GetExpressionTypes(numExpressionTypes);
        CPPUNIT_ASSERT_MESSAGE("Incorrect number of ExpressionTypes.", numExpressionTypes == 2);
        CPPUNIT_ASSERT_MESSAGE("Incorrect ExpressionTypes.",
            expressionTypes[0] == FdoExpressionType_Basic &&
            expressionTypes[1] == FdoExpressionType_Function);
        FdoPtr<FdoFunctionDefinitionCollection> functions = capExpression->GetFunctions();
        CPPUNIT_ASSERT_MESSAGE("NULL function list.", functions != NULL);
        CPPUNIT_ASSERT_MESSAGE("Wrong function list size.", functions->GetCount() == 56);
        FdoPtr<FdoFunctionDefinition> function;
        function = functions->FindItem(FDO_FUNCTION_CEIL);
        CPPUNIT_ASSERT_MESSAGE("Missing function Ceil", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_FLOOR);
        CPPUNIT_ASSERT_MESSAGE("Missing function Floor", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_AVG);
        CPPUNIT_ASSERT_MESSAGE("Missing function Avg", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_COUNT);
        CPPUNIT_ASSERT_MESSAGE("Missing function Count", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_MAX);
        CPPUNIT_ASSERT_MESSAGE("Missing function Max", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_MIN);
        CPPUNIT_ASSERT_MESSAGE("Missing function Min", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_SUM);
        CPPUNIT_ASSERT_MESSAGE("Missing function Sum", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_LOWER);
        CPPUNIT_ASSERT_MESSAGE("Missing function Lower", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_UPPER);
        CPPUNIT_ASSERT_MESSAGE("Missing function Upper", function != NULL);
        function = functions->FindItem(FDO_FUNCTION_CONCAT);
        CPPUNIT_ASSERT_MESSAGE("Missing function Concat", function != NULL);
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}


// tests connection info, including setting of connection string properties
void OdbcAccessFdoConnectTest::InfoTest()
{
    try
    {
        // call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // get the connection info
        FdoPtr<FdoIConnectionInfo> connInfo = connection->GetConnectionInfo();
        if (connInfo == NULL)
            CPPUNIT_FAIL("FAILED - GetConnectionInfo returned NULL\n");

        // get the connection properties
        FdoPtr<FdoIConnectionPropertyDictionary> connProps = connInfo->GetConnectionProperties();
        if (connProps == NULL)
            CPPUNIT_FAIL("FAILED - GetConnectionProperties returned NULL\n");

        // ------------------------------------------------
        // test basic access
        // ------------------------------------------------

        // get the names of all properties
        FdoInt32 numProps = 0;
        FdoString** propNames = NULL;
            
        propNames = connProps->GetPropertyNames(numProps);
        if (propNames == NULL || numProps != 5)
            CPPUNIT_FAIL("FAILED - GetConnectionProperties returned count != 5\n");

        numProps = 0;
        propNames = NULL;

        propNames = connProps->GetPropertyNames(numProps);
        if (propNames == NULL || numProps != 5)
            CPPUNIT_FAIL("FAILED - GetConnectionProperties returned count != 5\n");

        // access the data in each property
        for (int i=0; i<numProps; i++)
        {
            FdoString* propName = propNames[i];

            FdoString* propLocal   = connProps->GetLocalizedName(propName);
            FdoString* propValue   = connProps->GetProperty(propName);
            FdoString* propDefault = connProps->GetPropertyDefault(propName);

            bool propRequired      = connProps->IsPropertyRequired(propName);
            bool propProtected     = connProps->IsPropertyProtected(propName);
            bool propEnumerable    = connProps->IsPropertyEnumerable(propName);

            FdoInt32 numValues = 0;
            FdoString** propValues = connProps->EnumeratePropertyValues(propName, numValues);
            for (int j=0; j<numValues; j++)
            {
                FdoString* value = propValues[j];
				printf("Connection property enumeration #%d = '%ls'\n", j, value);
            }

            if (wcscmp(propName, L"DataSourceName") == 0)
            {
                CPPUNIT_ASSERT_MESSAGE("DataSourceName connection property should be enumerable", propEnumerable == true );
                CPPUNIT_ASSERT_MESSAGE("DataSourceName should have > 0 values", numValues > 0 );
                CPPUNIT_ASSERT_MESSAGE("DataSourceName should have > 0 values", propValues != NULL );
            }
        }

        // ------------------------------------------------
        // test basic setting of all properties
        // ------------------------------------------------

        // set all required properties to their defaults
        for (int i=0; i<numProps; i++)
        {
            FdoString* propName = propNames[i];
            if (connProps->IsPropertyRequired(propName))
                connProps->SetProperty(propName, connProps->GetPropertyDefault(propName));
        }

        // set non-required properties to their defaults
        for (int i=0; i<numProps; i++)
        {
            FdoString* propName = propNames[i];
            if (!connProps->IsPropertyRequired(propName))
                connProps->SetProperty(propName, connProps->GetPropertyDefault(propName));
        }

        // ------------------------------------------------
        // test updating of connection string
        // ------------------------------------------------

        // set custom values
        FdoStringP strDSN                   = L"MsTest";
        FdoStringP strUID                   = L"GregB";
        FdoStringP strPWD                   = L"test";
        FdoStringP strConnectionString      = L"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb";

        connProps->SetProperty(L"DataSourceName", strDSN);
        connProps->SetProperty(L"UserId", strUID);
        connProps->SetProperty(L"Password", strPWD);
        connProps->SetProperty(L"ConnectionString", strConnectionString);

        // get the updated connection string
        FdoStringP connStr1 = connection->GetConnectionString();

        // form the expected connection string
        wchar_t* tokenDSN = UnitTestUtil::MakeString(L"DataSourceName", L"=", strDSN);
        wchar_t* tokenUID = UnitTestUtil::MakeString(L"UserId", L"=", strUID);
        wchar_t* tokenPWD = UnitTestUtil::MakeString(L"Password", L"=", strPWD);
        wchar_t* tokenConnectStr = UnitTestUtil::MakeString(L"ConnectionString", L"=", L"\"", strConnectionString, L"\"");
        wchar_t* tokenGDGP = UnitTestUtil::MakeString(L"GenerateDefaultGeometryProperty", L"=", L"true");
        wchar_t* connStr2 = UnitTestUtil::MakeString(tokenUID, L";", tokenPWD, L";", tokenDSN, L";", tokenConnectStr, L";", tokenGDGP, L";");

        // compare them
        int result = wcscmp(connStr1, connStr2);
        if (result != 0)
            CPPUNIT_FAIL("FAILED - ConnectionInfo did not update connection string as expected\n");

        // cleanup
        delete [] tokenDSN;
        tokenDSN = NULL;
        delete [] tokenUID;
        tokenUID = NULL;
        delete [] tokenPWD;
        tokenPWD = NULL;
        delete [] tokenConnectStr;
        tokenConnectStr = NULL;
        delete [] connStr2;
        connStr2 = NULL;

        // ------------------------------------------------
        // test updating of connection info properties
        // ------------------------------------------------

        connection->SetConnectionString(L"DataSourceName=MsTest;UserId=Tester;Password=Test2;ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\"");

        strDSN = connProps->GetProperty(L"DataSourceName");
        strUID = connProps->GetProperty(L"UserId");
        strPWD = connProps->GetProperty(L"Password");
        strConnectionString = connProps->GetProperty(L"ConnectionString");

        // These next two tests are coded out until Defect 742563 is fixed.  The generic RDBMS code -- it never
        // updates connection property dictionary as a result of a connection string assignment.

        if (wcscmp(strDSN, L"MsTest") != 0 ||
            wcscmp(strUID, L"Tester") != 0 ||
            wcscmp(strPWD, L"Test2")  != 0 ||
            wcscmp(strConnectionString, L"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb")  != 0)
        {
            CPPUNIT_FAIL("FAILED - connection string did not update ConnectionInfo as expected\n");
        }

        // ------------------------------------------------
        // test setting valid string first and then get connection info
        // ------------------------------------------------

        FdoPtr<FdoIConnection> connection2 = UnitTestUtil::GetProviderConnectionObject();
        if (connection2 == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        connection2->SetConnectionString(L"DataSourceName=MsTest;UserId=Tester2;Password=Test3;ConnectionString=\"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb\"");
        FdoPtr<FdoIConnectionInfo> connInfo2 = connection2->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> connProps2 = connInfo2->GetConnectionProperties();

        strDSN = connProps2->GetProperty(L"DataSourceName");
        strUID = connProps2->GetProperty(L"UserId");
        strPWD = connProps2->GetProperty(L"Password");
        strConnectionString = connProps->GetProperty(L"ConnectionString");

        if (wcscmp(strDSN, L"MsTest") != 0 ||
            wcscmp(strUID, L"Tester2") != 0 ||
            wcscmp(strPWD, L"Test3")  != 0 ||
            wcscmp(strConnectionString, L"Driver={Microsoft Access Driver (*.mdb)};DBQ=MSTest.mdb")  != 0)
        {
            CPPUNIT_FAIL("FAILED - ConnectionInfo not initialized from connection string as expected\n");
        }

        // ------------------------------------------------
        // test handling of invalid operations
        // ------------------------------------------------

        // try accessing a non-existing property
        bool hitException = false;
        try
        {
            FdoString* value = connProps->GetProperty(L"DummyProperty");
        }
        catch (FdoException *e)
        {
            hitException = true;
            e->Release();
        }
        if (!hitException)
            CPPUNIT_FAIL("FAILED - ConnectionInfo allowed accessing an invalid property\n");

        // try setting a property while the connection is open
        connection->SetConnectionString(UnitTestUtil::GetConnectionString());
        connection->Open();

        hitException = false;
        try
        {
            connProps->SetProperty(L"UserId", L"Test4");
        }
        catch (FdoException*e)
        {
            hitException = true;
            e->Release();
        }
        if (!hitException)
            CPPUNIT_FAIL("FAILED - ConnectionInfo allowed setting a property on an open connection\n");

        connection->Close();

        // now we should be able to set it again
        connProps->SetProperty(L"UserId", L"Test4");
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

void OdbcAccessFdoConnectTest::ConnectWithParmTest()
{
    try
    {
    	// call the static method
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        // get the connection info
        FdoPtr<FdoIConnectionInfo> connInfo = connection->GetConnectionInfo();
        if (connInfo == NULL)
            CPPUNIT_FAIL("FAILED - GetConnectionInfo returned NULL\n");

        // get the connection properties
        FdoPtr<FdoIConnectionPropertyDictionary> connProps = connInfo->GetConnectionProperties();
        if (connProps == NULL)
            CPPUNIT_FAIL("FAILED - GetConnectionProperties returned NULL\n");

        // ------------------------------------------------
        // test basic access
        // ------------------------------------------------

        // get the names of all properties
        FdoInt32 numProps = 0;
        FdoString** propNames = NULL;
            
        propNames = connProps->GetPropertyNames(numProps);
        if (propNames == NULL || numProps != 5)
            CPPUNIT_FAIL("FAILED - GetConnectionProperties returned count != 4\n");

		connProps->SetProperty(L"DataSourceName",L"MsTest");
		connProps->SetProperty(L"UserId",L"");
		connProps->SetProperty(L"Password",L"");
		connProps->SetProperty(L"ConnectionString",L"\"\"");

		connection->Open();
    }
    catch (FdoException *ex)
    {
		TestCommonFail (ex);
    }
}

#endif
