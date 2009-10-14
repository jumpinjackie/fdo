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
#include "FdoConnectTest.h"
#include "UnitTestUtil.h"

// Check whether a class definition has a property named "Geometry".
// The property does NOT have to actually be geometric.
FdoConnectTest::FdoConnectTest(void)
{
}

FdoConnectTest::~FdoConnectTest(void)
{
    if( mConnection != NULL )
        mConnection->Close();
    mConnection = NULL;
}

void FdoConnectTest::setUp ()
{
    set_provider();
}

void FdoConnectTest::tearDown ()
{
    if( mConnection != NULL )
        mConnection->Close();
    mConnection = NULL;
}

bool FdoConnectTest::HasGeometry(FdoIConnection * connection,FdoString * className)
{
    bool hasGeometry = false;
    FdoPtr<FdoIDescribeSchema> describeSchemaCmd = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

    FdoInt32 numSchemas = schemas->GetCount();
    for (int i=0; !hasGeometry && i<numSchemas; i++)
    {
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->FindItem(className);
        if ( classDef == NULL )
            CPPUNIT_FAIL("FAILED - expected class not found\n");
        FdoPtr<FdoPropertyDefinitionCollection> classProps = classDef->GetProperties();
        FdoPtr<FdoPropertyDefinition> propDef = classProps->FindItem(L"Geometry");
        if (propDef != NULL)
            hasGeometry = true;

        // Try executing a Select too, to confirm that it really works.
        // This verifies the fix for defect 666456.
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(className);
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        classDef = reader->GetClassDefinition();
        classProps = classDef->GetProperties();
        propDef = classProps->FindItem(L"Geometry");
        if (propDef == NULL && hasGeometry)
            CPPUNIT_FAIL("FAILED - schema and Select command conflict with Geometry property\n");
    }

    return hasGeometry;
}



void FdoConnectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetConnection(L"", true);
    }
    catch (FdoException *ex)
    {
        if (mConnection)
            mConnection= NULL;
        TestCommonFail (ex);
    }

}

void FdoConnectTest::ReadAcDbPolyline( int connNumber, FdoIFeatureReader *myReader )
{
    try
    {
        if( connNumber == 1 )
        {
            if( ! myReader->IsNull(L"color") )
                //printf("\t(First Connection) Color: %ls\n", myReader->GetString(L"color") );
                myReader->GetString(L"color");
            if( ! myReader->IsNull(L"layer") )
                //printf("\t(First Connection) Layer: %ls\n", myReader->GetString(L"layer") );
                myReader->GetString(L"layer");
        }
        else
        {
            if( ! myReader->IsNull(L"PIN") )
                //printf("\t(Second Connection) PIN: %ls\n", myReader->GetString(L"PIN") );
                myReader->GetString(L"PIN");
        }
    }
    catch( FdoException *ex)
    {
        printf("(%s)FDO exception: %ls \n", (connNumber==1)?"First":"Second", ex->GetExceptionMessage() );
        TestCommonFail(ex);
    }
}

void FdoConnectTest::query ()
{
    connect();
    FdoPtr<FdoIFeatureReader> myFirstReader;
    FdoPtr<FdoIFeatureReader> mySecondReader;
    FdoPtr<FdoComparisonCondition> filter = FdoPtr<FdoComparisonCondition>(FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                        FdoComparisonOperations_LessThan,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create((int)20))) );
    if( mConnection != NULL )
    {
         try
        {
            FdoPtr<FdoISelect> firstSelCmd = (FdoISelect*)mConnection->CreateCommand(  FdoCommandType_Select );
            firstSelCmd->SetFilter(filter);
            firstSelCmd->SetFeatureClassName(L"Acad:AcDb3dPolyline");
            myFirstReader = firstSelCmd->Execute( );
        }
        catch (FdoException *ex )
        {
			TestCommonFail(ex);
        }
        try
        {
            FdoPtr<FdoISelect> selCmd = (FdoISelect*)mConnection->CreateCommand(  FdoCommandType_Select );
            selCmd->SetFeatureClassName(L"L\x00e4nd:Parcel");
            mySecondReader = selCmd->Execute( );
            if( mySecondReader != NULL  && myFirstReader != NULL )
            {
                while ( mySecondReader->ReadNext() && myFirstReader->ReadNext() )
                {
                    ReadAcDbPolyline( 2, mySecondReader );
                    ReadAcDbPolyline( 1,myFirstReader );
                }
            }
        }
        catch( FdoException *ex )
        {
			TestCommonFail(ex);
        }
    }
}

void FdoConnectTest::connectWithInvalidString()
{
	FdoPtr<FdoIConnection> oConn;
	try
	{
		oConn = UnitTestUtil::GetProviderConnectionObject();
		oConn->SetConnectionString(L"UserName=testUserNoUse;Password=xabc");
		oConn->Open();
		
		oConn->Close();
		CPPUNIT_FAIL ("Open succeded with a invalid connect string");
    }
    catch (FdoException *ex)
    {
		// valid exception
		ex->Release();
    }
	catch (...)
	{
		CPPUNIT_FAIL("Unknown exception");
	}
}

void FdoConnectTest::connectWithExtraneousSpaces()
{
    wchar_t *wConnection = UnitTestUtil::GetConnectionString(Connection_WithDatastore, L"", true);

    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
    connection->SetConnectionString (wConnection);
    FdoPtr<FdoIConnectionInfo> connInfo = connection->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> connDict = connInfo->GetConnectionProperties();
	if( connection->Open() != FdoConnectionState_Open )
    {
        connection->Close();
        throw "connectWithExtraneousSpaces failed";
    }
}

void FdoConnectTest::connectFailAndSucceed()
{
    // Create datastore if it doesn't exist
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(L"", true);
    connection->Close();
    connection = NULL;

    // Open connection to datastore but omit password.
    connection = UnitTestUtil::GetProviderConnectionObject();
    FdoPtr<FdoIConnectionInfo> connInfo = connection->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> dict = connInfo->GetConnectionProperties();

	dict->SetProperty( L"Service", UnitTestUtil::GetEnviron("service") );
	dict->SetProperty( L"Username", UnitTestUtil::GetEnviron("username") );
	dict->SetProperty( L"DataStore", UnitTestUtil::GetEnviron("datastore", L"") );

    bool failed = false;

    try {
        connection->Open();
    }
    catch ( ... ) {
        // Connect without password should fail
        failed = true;
    }

    CPPUNIT_ASSERT( failed );

    // Set password and connection again; should succeed.
    dict->SetProperty( L"Password", UnitTestUtil::GetEnviron("password") );
    connection->Open();

    CPPUNIT_ASSERT( connection->GetConnectionState() == FdoConnectionState_Open );

    connection->Close();
}
