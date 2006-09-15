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

	    //const wchar_t **strings;
	    //const wchar_t **strings2;
	    //int count;
	    //int count2;
        int size;
	    //int *types;
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

        printf("SIZE = %d\n", size);
        CPPUNIT_ASSERT( size == 0 );

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

        //for ( i = 0; i < size; i++ )
        //    CPPUNIT_ASSERT( lockArray[i] );

        FdoICommandCapabilities *commandCapabilities = connection->GetCommandCapabilities();
        FdoInt32 *commands = commandCapabilities->GetCommands(size);

        if (commandCapabilities == NULL)
            CPPUNIT_FAIL("FAILED - GetCommandCapabilities returned NULL\n");
  //      CPPUNIT_ASSERT_MESSAGE("Incorrect commands.",
  //          commands[0] == FdoCommandType_Select &&
  //          commands[1] == FdoCommandType_SelectAggregates &&
  //          commands[2] == FdoCommandType_DescribeSchema &&
  //          commands[3] == FdoCommandType_DescribeSchemaMapping &&
  //          commands[4] == FdoCommandType_SQLCommand &&
  //          commands[5] == FdoCommandType_Insert &&
  //          commands[6] == FdoCommandType_Delete &&
  //          commands[7] == FdoCommandType_Update &&
  //          commands[8] == FdoCommandType_GetSpatialContexts &&
		//	commands[9] == FdoCommandType_CreateDataStore &&
		//	commands[10] == FdoCommandType_DestroyDataStore &&
		//	commands[11] == FdoCommandType_ListDataStores	
		//);

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

        m_numSupportedFunctions = 0;
        if( functions )
        {
            // Initialize the list of expected functions and their signature.
            InitSupportedFunctions();
            m_currFctObject    = NULL;

            // Document the list of supported functions and their signatures. At the same
            // time cross-check against the expected function list.
            printf("\nSupported functions: \n");
            for(int i=0; i<functions->GetCount(); i++ )
            {
                FdoPtr<FdoFunctionDefinition> func = functions->GetItem( i );
                printf("Func: %ls: %ls\n", func->GetName(), func->GetDescription() );
                CheckFunction( func->GetName(), func->IsAggregate() );
                FdoPtr<FdoReadOnlySignatureDefinitionCollection> signatures = func->GetSignatures();
                if( signatures )
                {
                    for( int k=0; k<signatures->GetCount(); k++ )
                    {
                        FdoPtr<FdoSignatureDefinition> signature = signatures->GetItem(k);
                        printf("\tSignature%d= return type = %ls \n", k, GetDataType(signature->GetReturnType()));
                        FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = signature->GetArguments();
                        if( args )
                        {
                            for( int j=0; j<args->GetCount(); j++ )
                            {
                                FdoPtr<FdoArgumentDefinition>arg = args->GetItem( j );
                                printf("\t\tArg%d= %ls: %ls\n", j, arg->GetName(), arg->GetDescription() );
                                CheckSignature( signature->GetReturnType(), arg->GetName(), arg->GetDataType() );
                            }
                        }
                    }
                }
            }
            CheckExpectation();
        }

//	    FdoIConnectionInfo* connectionInfo = connection->GetConnectionInfo();
//        FdoString* featureDataObjectsVersion = connectionInfo->GetFeatureDataObjectsVersion();
//        FdoString* providerDescription = connectionInfo->GetProviderDescription();
//        FdoString* providerName = connectionInfo->GetProviderName();
//        FdoString* providerVersion = connectionInfo->GetProviderVersion();
//
//	    FdoIConnectionPropertyDictionary *dictionary = connectionInfo->GetConnectionProperties();
//	    FdoClassType *classTypes2 = schemaCapabilities->GetClassTypes(count);
//
//	    FdoClassType ret = classTypes[1];
//
//	    types = commandCapabilities->GetCommands(count);
//	    //FdoClassType *classTypes = commandCapabilities->GetClassTypes(count);
//
//	    strings = dictionary->GetPropertyNames(count);
//        FdoString *propertyDefault = dictionary->GetPropertyDefault(L"Username");
//        bool propertyRequired = dictionary->IsPropertyRequired(L"Username");
//        bool propertyEnumerable = dictionary->IsPropertyEnumerable(L"Username");
//        bool propertyProtected = dictionary->IsPropertyProtected(L"Username");
//        propertyProtected = dictionary->IsPropertyProtected(L"Password");
//        const wchar_t *passwordLocalized = dictionary->GetLocalizedName(L"Password");
//	    strings = dictionary->GetPropertyNames(count);
//	    strings2 = dictionary->EnumeratePropertyValues(L"Username", count2);
//	    const wchar_t *value = dictionary->GetProperty(L"Username");
//
//#ifdef _WIN32
//        dictionary->SetProperty(L"Username", (FdoStringP)UnitTestUtil::GetEnviron("username"));
//	    const wchar_t *password = dictionary->GetProperty(L"Password");
//	    dictionary->SetProperty(L"Password", (FdoStringP)UnitTestUtil::GetEnviron("password"));
//	    dictionary->SetProperty(L"DataStore", (FdoStringP)UnitTestUtil::GetEnviron("datastore"));
//	    dictionary->SetProperty(L"Service", (FdoStringP)UnitTestUtil::GetEnviron("service"));
//#else
//	    wchar_t temp[128];
//	    mbstowcs(temp, UnitTestUtil::GetEnviron("username"), sizeof(temp));
//        dictionary->SetProperty(L"Username", temp);
//	    const wchar_t *password = dictionary->GetProperty(L"Password");
//	    mbstowcs(temp, UnitTestUtil::GetEnviron("password"), sizeof(temp));
//	    dictionary->SetProperty(L"Password", temp);
//	    mbstowcs(temp, UnitTestUtil::GetEnviron("datastore"), sizeof(temp));
//	    dictionary->SetProperty(L"DataStore", temp);
//	    mbstowcs(temp, UnitTestUtil::GetEnviron("service"), sizeof(temp));
//	    dictionary->SetProperty(L"Service", temp);
//
//#endif
//
//	    const wchar_t *str = connection->GetConnectionString();
//        connection->Open ();
//
//	    dictionary->Release();
//	    connectionCapabilities->Release();
//	    commandCapabilities->Release();
//	    schemaCapabilities->Release();
//	    connectionInfo->Release();
//	    filterCapabilities->Release();
//	    expressionCapabilities->Release();
//        functions->Release();
//
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

void FdoCapabilityTest::CheckExpectation()
{
    int i, j, k;
    bool issuesFound = false;
    FdoCapabilityTest::fctSignature *currSignature = NULL;
    FdoCapabilityTest::argObject *currArgument = NULL;

    for( i=0; i<m_numSupportedFunctions; i++ )
    {
        m_currFctObject = m_supportedFunctions[i];
        if( !m_currFctObject->found )
        {
            printf("\tFunction %ls expected, not found\n", m_currFctObject->name );
            issuesFound = true;
            break;
        }

        // The function was found, but what about the signatures?
        for( j=0; j<m_currFctObject->signatureCount; j++ )
        {
            currSignature = m_currFctObject->signature[j];
            if( !currSignature->found )
            {
                printf("\tSignature for function %ls expected, not found: ", m_currFctObject->name );
                printf("Return type: %ls ", GetDataType( currSignature->returnDataType ));
                for (int l=0; l<currSignature->argCount; l++)
                {
                    currArgument = currSignature->arguments[l];
                    printf("Arg%d: name=%ls, type=%ls; ", l, currArgument->name, GetDataType( currArgument->dataType ) );
                }
                printf("\n");
                issuesFound = true;
                continue;
            }

            // OK, the signature was found, but also all the arguments?
            for( k=0; k<currSignature->argCount; k++ )
            {
                currArgument = currSignature->arguments[k];
                if( !currArgument->found )
                {
                    printf("\tIncomplete signature for function %ls expected: ", m_currFctObject->name );
                    printf("Return type: %ls ", GetDataType( currSignature->returnDataType ));
                    printf("Arg%d: name=%ls, type=%ls; ", k, currArgument->name, GetDataType( currArgument->dataType ) );
                    printf("\n");
                    issuesFound = true;
                    break;
                }
            }
        }
    }

    if( issuesFound )
      throw FdoException::Create(L"Unexpected issues in function definitions");

}

void FdoCapabilityTest::CheckFunction(FdoString *name, bool isAggregate)
{
    int i;
    bool found;

    m_currFctObject = NULL;

    for( i=0, found=false; i<m_numSupportedFunctions && !found; i++ )
    {
        if( (wcscmp( m_supportedFunctions[i]->name, name ) == 0) &&
            (m_supportedFunctions[i]->isAggregate == isAggregate) )
        {
            m_supportedFunctions[i]->found = true;
            m_currFctObject = m_supportedFunctions[i];
            found = true;
        }
    }
}

void FdoCapabilityTest::CheckSignature (FdoDataType fctReturnType, FdoString *argName, FdoDataType argDataType)
{
    int i, j;
    bool afound, sfound;
    FdoCapabilityTest::fctSignature *currSignature = NULL;
    FdoCapabilityTest::argObject *currArgument = NULL;

    if( m_currFctObject == NULL )
        return;

    for( i=0, sfound=false; i<m_currFctObject->signatureCount && !sfound; i++ )
    {
        currSignature = m_currFctObject->signature[i];
        if( currSignature->returnDataType == fctReturnType )
        {
            for( j=0, afound=false; j<currSignature->argCount && !afound; j++ )
            {
                currArgument = currSignature->arguments[j];
                if( (wcscmp( currArgument->name, argName ) == 0) && (currArgument->dataType == argDataType) )
                {
                    currArgument->found = true;
                    afound = true;
                }
            }
            if( afound )
            {
                currSignature->found = true;
                sfound = true;
            }
        }
    }
}

FdoString *FdoCapabilityTest::GetDataType(FdoDataType dataType)
{
    switch( dataType )
    {
        case FdoDataType_Boolean:  return L"FdoDataType_Boolean";  break;
        case FdoDataType_Byte:     return L"FdoDataType_Byte";     break;
        case FdoDataType_DateTime: return L"FdoDataType_DateTime"; break;
        case FdoDataType_Decimal:  return L"FdoDataType_Decimal";  break;
        case FdoDataType_Double:   return L"FdoDataType_Double";   break;
        case FdoDataType_Int16:    return L"FdoDataType_Int16";    break;
        case FdoDataType_Int32:    return L"FdoDataType_Int32";    break;
        case FdoDataType_Int64:    return L"FdoDataType_Int64";    break;
        case FdoDataType_Single:   return L"FdoDataType_Single";   break;
        case FdoDataType_String:   return L"FdoDataType_String";   break;
        case FdoDataType_BLOB:     return L"FdoDataType_BLOB";     break;
        case FdoDataType_CLOB:     return L"FdoDataType_CLOB";     break;
        default:                   return L"Unknown";              break;
    }
}

FdoCapabilityTest::fctSignature *FdoCapabilityTest::InitSignature (FdoDataType     retDataType,
                                                                   FdoPropertyType retPropertyType,
                                                                   bool            expDataType)
{

    fctSignature *signature = new fctSignature();
    signature->expectDataType = expDataType;
    signature->returnDataType = retDataType;
    signature->returnPropertyType = retPropertyType;
    return signature;
}

FdoCapabilityTest::argObject *FdoCapabilityTest::InitArgument (FdoString   *argName,
                                                               FdoDataType argDataType)
{
    argObject *argument = new argObject();
    argument->name = new wchar_t[wcslen(argName)+1];
    wcscpy(argument->name, argName);
    argument->dataType = argDataType;
    return argument;
}

void FdoCapabilityTest::InitSupportedFunctions()
{
    for (int i = 0; i < 20; i++)
        m_supportedFunctions[i] = NULL;

    // Add the list of expected functions.
    m_supportedFunctions[0] = new fctObject();
    m_supportedFunctions[0]->name = new wchar_t[wcslen(FDO_FUNCTION_AVG)+1];
    wcscpy(m_supportedFunctions[0]->name, FDO_FUNCTION_AVG);
    m_supportedFunctions[0]->isAggregate = true;
    m_supportedFunctions[0]->signature[0] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[0]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[0]->signature[1] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[1]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[0]->signature[2] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[2]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int16);
    m_supportedFunctions[0]->signature[3] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[3]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int32);
    m_supportedFunctions[0]->signature[4] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[4]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int64);
    m_supportedFunctions[0]->signature[5] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[0]->signature[5]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[0]->signatureCount = 6;

    m_supportedFunctions[1] = new fctObject();
    m_supportedFunctions[1]->name = new wchar_t[wcslen(FDO_FUNCTION_CEIL)+1];
    wcscpy(m_supportedFunctions[1]->name, FDO_FUNCTION_CEIL);
    m_supportedFunctions[1]->isAggregate = false;
    m_supportedFunctions[1]->signature[0] = InitSignature(FdoDataType_Decimal, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[1]->signature[0]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[1]->signature[1] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[1]->signature[1]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[1]->signature[2] = InitSignature(FdoDataType_Single, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[1]->signature[2]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[1]->signatureCount = 3;

    m_supportedFunctions[2] = new fctObject();
    m_supportedFunctions[2]->name = new wchar_t[wcslen(FDO_FUNCTION_CONCAT)+1];
    wcscpy(m_supportedFunctions[2]->name, FDO_FUNCTION_CONCAT);
    m_supportedFunctions[2]->isAggregate = false;
    m_supportedFunctions[2]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[2]->signature[0]->arguments[0] = InitArgument(L"strValue1", FdoDataType_String);
    m_supportedFunctions[2]->signature[0]->arguments[1] = InitArgument(L"strValue2", FdoDataType_String);
    m_supportedFunctions[2]->signatureCount = 1;
    m_supportedFunctions[2]->signature[0]->argCount = 2;

    m_supportedFunctions[3] = new fctObject();
    m_supportedFunctions[3]->name = new wchar_t[wcslen(FDO_FUNCTION_COUNT)+1];
    wcscpy(m_supportedFunctions[3]->name, FDO_FUNCTION_COUNT);
    m_supportedFunctions[3]->isAggregate = true;
    m_supportedFunctions[3]->signature[0] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[0]->arguments[0] = InitArgument(L"boolValue", FdoDataType_Boolean);
    m_supportedFunctions[3]->signature[1] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[1]->arguments[0] = InitArgument(L"byteValue", FdoDataType_Byte);
    m_supportedFunctions[3]->signature[2] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[2]->arguments[0] = InitArgument(L"dateTimeValue", FdoDataType_DateTime);
    m_supportedFunctions[3]->signature[3] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[3]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[3]->signature[4] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[4]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[3]->signature[5] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[5]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int16);
    m_supportedFunctions[3]->signature[6] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[6]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int32);
    m_supportedFunctions[3]->signature[7] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[7]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int64);
    m_supportedFunctions[3]->signature[8] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[8]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[3]->signature[9] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[3]->signature[9]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[3]->signatureCount = 10;

    m_supportedFunctions[4] = new fctObject();
    m_supportedFunctions[4]->name = new wchar_t[wcslen(FDO_FUNCTION_FLOOR)+1];
    wcscpy(m_supportedFunctions[4]->name, FDO_FUNCTION_FLOOR);
    m_supportedFunctions[4]->isAggregate = false;
    m_supportedFunctions[4]->signature[0] = InitSignature(FdoDataType_Decimal, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[4]->signature[0]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[4]->signature[1] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[4]->signature[1]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[4]->signature[2] = InitSignature(FdoDataType_Single, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[4]->signature[2]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[4]->signatureCount = 3;

    m_supportedFunctions[5] = new fctObject();
    m_supportedFunctions[5]->name = new wchar_t[wcslen(FDO_FUNCTION_LOWER)+1];
    wcscpy(m_supportedFunctions[5]->name, FDO_FUNCTION_LOWER);
    m_supportedFunctions[5]->isAggregate = false;
    m_supportedFunctions[5]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[5]->signature[0]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[5]->signatureCount = 1;

    m_supportedFunctions[6] = new fctObject();
    m_supportedFunctions[6]->name = new wchar_t[wcslen(FDO_FUNCTION_MAX)+1];
    wcscpy(m_supportedFunctions[6]->name, FDO_FUNCTION_MAX);
    m_supportedFunctions[6]->isAggregate = true;
    m_supportedFunctions[6]->signature[0] = InitSignature(FdoDataType_Byte, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[0]->arguments[0] = InitArgument(L"byteValue", FdoDataType_Byte);
    m_supportedFunctions[6]->signature[1] = InitSignature(FdoDataType_DateTime, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[1]->arguments[0] = InitArgument(L"dateTimeValue", FdoDataType_DateTime);
    m_supportedFunctions[6]->signature[2] = InitSignature(FdoDataType_Decimal, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[2]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[6]->signature[3] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[3]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[6]->signature[4] = InitSignature(FdoDataType_Int16, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[4]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int16);
    m_supportedFunctions[6]->signature[5] = InitSignature(FdoDataType_Int32, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[5]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int32);
    m_supportedFunctions[6]->signature[6] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[6]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int64);
    m_supportedFunctions[6]->signature[7] = InitSignature(FdoDataType_Single, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[7]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[6]->signature[8] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[6]->signature[8]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[6]->signatureCount = 9;

    m_supportedFunctions[7] = new fctObject();
    m_supportedFunctions[7]->name = new wchar_t[wcslen(FDO_FUNCTION_MIN)+1];
    wcscpy(m_supportedFunctions[7]->name, FDO_FUNCTION_MIN);
    m_supportedFunctions[7]->isAggregate = true;
    m_supportedFunctions[7]->signature[0] = InitSignature(FdoDataType_Byte, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[0]->arguments[0] = InitArgument(L"byteValue", FdoDataType_Byte);
    m_supportedFunctions[7]->signature[1] = InitSignature(FdoDataType_DateTime, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[1]->arguments[0] = InitArgument(L"dateTimeValue", FdoDataType_DateTime);
    m_supportedFunctions[7]->signature[2] = InitSignature(FdoDataType_Decimal, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[2]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[7]->signature[3] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[3]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[7]->signature[4] = InitSignature(FdoDataType_Int16, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[4]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int16);
    m_supportedFunctions[7]->signature[5] = InitSignature(FdoDataType_Int32, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[5]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int32);
    m_supportedFunctions[7]->signature[6] = InitSignature(FdoDataType_Int64, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[6]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int64);
    m_supportedFunctions[7]->signature[7] = InitSignature(FdoDataType_Single, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[7]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[7]->signature[8] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[7]->signature[8]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[7]->signatureCount = 9;

    m_supportedFunctions[8] = new fctObject();
    m_supportedFunctions[8]->name = new wchar_t[wcslen(FDO_FUNCTION_SUM)+1];
    wcscpy(m_supportedFunctions[8]->name, FDO_FUNCTION_SUM);
    m_supportedFunctions[8]->isAggregate = true;
    m_supportedFunctions[8]->signature[0] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[0]->arguments[0] = InitArgument(L"decimalValue", FdoDataType_Decimal);
    m_supportedFunctions[8]->signature[1] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[1]->arguments[0] = InitArgument(L"dblValue", FdoDataType_Double);
    m_supportedFunctions[8]->signature[2] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[2]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int16);
    m_supportedFunctions[8]->signature[3] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[3]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int32);
    m_supportedFunctions[8]->signature[4] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[4]->arguments[0] = InitArgument(L"intValue", FdoDataType_Int64);
    m_supportedFunctions[8]->signature[5] = InitSignature(FdoDataType_Double, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[8]->signature[5]->arguments[0] = InitArgument(L"singleValue", FdoDataType_Single);
    m_supportedFunctions[8]->signatureCount = 6;

    m_supportedFunctions[9] = new fctObject();
    m_supportedFunctions[9]->name = new wchar_t[wcslen(FDO_FUNCTION_UPPER)+1];
    wcscpy(m_supportedFunctions[9]->name, FDO_FUNCTION_UPPER);
    m_supportedFunctions[9]->isAggregate = false;
    m_supportedFunctions[9]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[9]->signature[0]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[9]->signatureCount = 1;

    m_numSupportedFunctions = 10;
}

FdoCapabilityTest::argObject::argObject()
: name(NULL)
{
    found    = false;
    dataType = (FdoDataType)-1;
}

FdoCapabilityTest::argObject::~argObject()
{
    if (name != NULL)
    {
        delete[] name;
        name = NULL;
    }
}

FdoCapabilityTest::fctSignature::fctSignature()
{
    found              = false;
    returnDataType     = (FdoDataType)-1;
    returnPropertyType = FdoPropertyType_DataProperty;
    expectDataType     = true;
    argCount           = 1;
    for (int i = 0; i < 10; i++)
        arguments[i] = NULL;
}

FdoCapabilityTest::fctSignature::~fctSignature()
{
    for (int i = 0; i < 10; i++)
    {
        if (arguments[i] != NULL)
        {
            delete[] arguments[i];
            arguments[i] = NULL;
        }
    }
}

FdoCapabilityTest::fctObject::fctObject()
: name(NULL)
{
    found       = false;
    isAggregate = false;
    for (int i = 0; i < 20; i++)
        signature[i] = NULL;
}

FdoCapabilityTest::fctObject::~fctObject()
{
    if (name != NULL)
    {
        delete[] name;
        name = NULL;
    }

    for (int i = 0; i < 20; i++)
    {
        if (signature[i] != NULL)
        {
            delete[] signature[i];
            signature[i] = NULL;
        }
    }
}


