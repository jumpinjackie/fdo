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
#include "FdoCapabilityTest.h"
#include "UnitTestUtil.h"

FdoCapabilityTest::FdoCapabilityTest(void)
{
}

FdoCapabilityTest::~FdoCapabilityTest(void)
{
}

void FdoCapabilityTest::setUp ()
{
    set_provider();
}


void FdoCapabilityTest::connection ()
{
    FdoIConnection* connection = NULL;
    try
    {
        connection = UnitTestUtil::GetConnection("", true);

	    const wchar_t **strings;
	    const wchar_t **strings2;
	    int count;
	    int count2;
        int size;
	    int *types;
        bool lockArray[20];
        int i;

        for ( i = 0; i < 20; i++ )
            lockArray[i] = false;

	    FdoIConnectionCapabilities *connectionCapabilities = connection->GetConnectionCapabilities();
        FdoSpatialContextExtentType *contextExtentType = connectionCapabilities->GetSpatialContextTypes(size);
        FdoThreadCapability threadCapability = connectionCapabilities->GetThreadCapability();
        bool supportsLocking = connectionCapabilities->SupportsLocking();
        bool supportsLongTransactions = connectionCapabilities->SupportsLongTransactions();
        bool supportsSQL = connectionCapabilities->SupportsSQL();
        bool supportsTimeout = connectionCapabilities->SupportsTimeout();
        bool supportsTransactions = connectionCapabilities->SupportsTransactions();

        FdoLockType* lockTypes = connectionCapabilities->GetLockTypes( size );

        CPPUNIT_ASSERT( size == 5 );

        for ( i = 0; i < size; i++ ) {
            switch ( lockTypes[i] ) {
            case FdoLockType_Transaction:
                lockArray[0] = true;
                break;
            case FdoLockType_Shared:
                lockArray[1] = true;
                break;
            case FdoLockType_AllLongTransactionExclusive:
                lockArray[2] = true;
                break;
            case FdoLockType_Exclusive:
                lockArray[3] = true;
                break;
            case FdoLockType_LongTransactionExclusive:
                lockArray[4] = true;
                break;
            default:
			    CPPUNIT_FAIL( "Unexpected lock type" );
                break;
            }
        }

        for ( i = 0; i < size; i++ )
            CPPUNIT_ASSERT( lockArray[i] );

        FdoICommandCapabilities *commandCapabilities = connection->GetCommandCapabilities();
        FdoInt32 *commands = commandCapabilities->GetCommands(size);

        if (commandCapabilities == NULL)
            CPPUNIT_FAIL("FAILED - GetCommandCapabilities returned NULL\n");
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
			commands[9] == FdoCommandType_CreateDataStore &&
			commands[10] == FdoCommandType_DestroyDataStore &&
			commands[11] == FdoCommandType_ListDataStores	
		);

        bool supportsParameters = commandCapabilities->SupportsParameters();
        bool supportsTimeout2 = commandCapabilities->SupportsTimeout();

	    FdoISchemaCapabilities *schemaCapabilities = connection->GetSchemaCapabilities();
        FdoClassType *classTypes = schemaCapabilities->GetClassTypes(size);
        FdoDataType *dataTypes = schemaCapabilities->GetDataTypes(size);
        bool supportsInheritance = schemaCapabilities->SupportsInheritance();
        bool supportsMultipleSchemas = schemaCapabilities->SupportsMultipleSchemas();
        bool supportsObjectProperties = schemaCapabilities->SupportsObjectProperties();
        bool supportsAssociations = schemaCapabilities->SupportsAssociationProperties();

        FdoIFilterCapabilities* filterCapabilities = connection->GetFilterCapabilities();
        FdoConditionType *conditionTypes = filterCapabilities->GetConditionTypes(size);
        FdoDistanceOperations *distanceOperations = filterCapabilities->GetDistanceOperations(size);
        FdoSpatialOperations *spatialOperations = filterCapabilities->GetSpatialOperations(size);
        bool supportsGeodesicDistance = filterCapabilities->SupportsGeodesicDistance();
        bool nonLiteralGeometricOperations = filterCapabilities->SupportsNonLiteralGeometricOperations();
        
        FdoIExpressionCapabilities* expressionCapabilities = connection->GetExpressionCapabilities();
        FdoExpressionType *expressionTypes = expressionCapabilities->GetExpressionTypes(size);
        FdoFunctionDefinitionCollection*    functions = expressionCapabilities->GetFunctions();
        if( functions )
        {
            printf("\nSupported functions: \n");
            for(int i=0; i<functions->GetCount(); i++ )
            {
                FdoPtr<FdoFunctionDefinition> func = functions->GetItem( i );
                printf("Func: %ls: %ls\n", func->GetName(), func->GetDescription() );
                FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = func->GetArguments();
                if( args )
                {
                    for( int j=0; j<args->GetCount(); j++ )
                    {
                        FdoPtr<FdoArgumentDefinition>arg = args->GetItem( j );
                        printf("\tArg%d= %ls: %ls\n", j, arg->GetName(), arg->GetDescription() );
                    }
                }
            }
        }
	    FdoIConnectionInfo* connectionInfo = connection->GetConnectionInfo();
        FdoString* featureDataObjectsVersion = connectionInfo->GetFeatureDataObjectsVersion();
        FdoString* providerDescription = connectionInfo->GetProviderDescription();
        FdoString* providerName = connectionInfo->GetProviderName();
        FdoString* providerVersion = connectionInfo->GetProviderVersion();

	    FdoIConnectionPropertyDictionary *dictionary = connectionInfo->GetConnectionProperties();
	    FdoClassType *classTypes2 = schemaCapabilities->GetClassTypes(count);

	    FdoClassType ret = classTypes[1];

	    types = commandCapabilities->GetCommands(count);
	    //FdoClassType *classTypes = commandCapabilities->GetClassTypes(count);

	    strings = dictionary->GetPropertyNames(count);
        FdoString *propertyDefault = dictionary->GetPropertyDefault(L"Username");
        bool propertyRequired = dictionary->IsPropertyRequired(L"Username");
        bool propertyEnumerable = dictionary->IsPropertyEnumerable(L"Username");
        bool propertyProtected = dictionary->IsPropertyProtected(L"Username");
        propertyProtected = dictionary->IsPropertyProtected(L"Password");
        const wchar_t *passwordLocalized = dictionary->GetLocalizedName(L"Password");
	    strings = dictionary->GetPropertyNames(count);
	    strings2 = dictionary->EnumeratePropertyValues(L"Username", count2);
	    const wchar_t *value = dictionary->GetProperty(L"Username");

#ifdef _WIN32
        dictionary->SetProperty(L"Username", (FdoStringP)UnitTestUtil::GetEnviron("username"));
	    const wchar_t *password = dictionary->GetProperty(L"Password");
	    dictionary->SetProperty(L"Password", (FdoStringP)UnitTestUtil::GetEnviron("password"));
	    dictionary->SetProperty(L"DataStore", (FdoStringP)UnitTestUtil::GetEnviron("datastore"));
	    dictionary->SetProperty(L"Service", (FdoStringP)UnitTestUtil::GetEnviron("service"));
#else
	    wchar_t temp[128];
	    mbstowcs(temp, UnitTestUtil::GetEnviron("username"), sizeof(temp));
        dictionary->SetProperty(L"Username", temp);
	    const wchar_t *password = dictionary->GetProperty(L"Password");
	    mbstowcs(temp, UnitTestUtil::GetEnviron("password"), sizeof(temp));
	    dictionary->SetProperty(L"Password", temp);
	    mbstowcs(temp, UnitTestUtil::GetEnviron("datastore"), sizeof(temp));
	    dictionary->SetProperty(L"DataStore", temp);
	    mbstowcs(temp, UnitTestUtil::GetEnviron("service"), sizeof(temp));
	    dictionary->SetProperty(L"Service", temp);

#endif

	    const wchar_t *str = connection->GetConnectionString();
        connection->Open ();

	    dictionary->Release();
	    connectionCapabilities->Release();
	    commandCapabilities->Release();
	    schemaCapabilities->Release();
	    connectionInfo->Release();
	    filterCapabilities->Release();
	    expressionCapabilities->Release();
        functions->Release();

	    connection->Close ();
	    connection->Release();
    }
    catch (FdoException *ex)
    {
        if (connection)
        {
    	    connection->Close ();
	        connection->Release();    
        }
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }

}
void FdoCapabilityTest::datastore()
{
    FdoIConnection* connection = NULL;
    try
    {
        connection = UnitTestUtil::GetProviderConnectionObject();
//        connection->Close();

        int count;

	    FdoIConnectionInfo* connectionInfo = connection->GetConnectionInfo();
	    FdoIConnectionPropertyDictionary *dictionary = connectionInfo->GetConnectionProperties();
        connectionInfo->Release();

#ifdef _WIN32
        dictionary->SetProperty(L"Username", (FdoStringP)UnitTestUtil::GetEnviron("username"));
	    const wchar_t *password = dictionary->GetProperty(L"Password");
	    dictionary->SetProperty(L"Password", (FdoStringP)UnitTestUtil::GetEnviron("password"));
	    dictionary->SetProperty(L"Service", (FdoStringP)UnitTestUtil::GetEnviron("service"));
#else
	    wchar_t temp[128];
	    mbstowcs(temp, UnitTestUtil::GetEnviron("username"), sizeof(temp));
        dictionary->SetProperty(L"Username", temp);
	    const wchar_t *password = dictionary->GetProperty(L"Password");
	    mbstowcs(temp, UnitTestUtil::GetEnviron("password"), sizeof(temp));
	    dictionary->SetProperty(L"Password", temp);
	    mbstowcs(temp, UnitTestUtil::GetEnviron("service"), sizeof(temp));
	    dictionary->SetProperty(L"Service", temp);

#endif
        connection->Open ();
        FdoString ** datastores = dictionary->EnumeratePropertyValues(L"DataStore", count);
        for (int i=0; i<count; i++)
        {
            printf("%d. %ls \n", i, datastores[i]);
        }
        dictionary->Release();
		connection->Close ();
		connection->Release();

    }
    catch (FdoException *ex)
    {
        if (connection)
        {
    	    connection->Close ();
	        connection->Release();    
        }
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }

}
