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


// ---------------------------------------------------------------------------
//                     Test Class Constructors/Destructors                    
// ---------------------------------------------------------------------------

FdoCapabilityTest::FdoCapabilityTest ()
:m_expectedSchemaCapabilities(NULL)
{

}  //  FdoCapabilityTest ()

FdoCapabilityTest::~FdoCapabilityTest ()
{

    if (m_expectedSchemaCapabilities != NULL) 
        m_expectedSchemaCapabilities->Clear();

    delete m_expectedSchemaCapabilities;
    m_expectedSchemaCapabilities = NULL;
}  //  ~FdoCapabilityTest ()


// ---------------------------------------------------------------------------
//                   Basic Test Environment Setup Functions                   
// ---------------------------------------------------------------------------

void FdoCapabilityTest::setUp ()
{

    SetProvider();

}  //  setUp ()


// ---------------------------------------------------------------------------
//                             Unit Test Functions                            
// ---------------------------------------------------------------------------

void FdoCapabilityTest::Connection ()
{

    FdoPtr<FdoIConnection> connection;

    try {

	  //const wchar_t **strings;
	  //const wchar_t **strings2;
	  //int count;
	  //int count2;
      int size;
	  //int *types;
      bool lockArray[20];
      int i;

      for (i = 0; i < 20; i++)
        lockArray[i] = false;

      for (i = 0; i < 20; i++)
        m_supportedFunctions[i] = NULL;

      connection = UnitTestUtil::GetConnection(L"", true);

	  FdoPtr<FdoIConnectionCapabilities> connectionCapabilities = connection->GetConnectionCapabilities();
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

      for (i = 0; i < size; i++) {

        switch (lockTypes[i]) {

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

        }  //  switch ..

      }  //  for ( ...

      //for ( i = 0; i < size; i++ )
      //    CPPUNIT_ASSERT( lockArray[i] );

      FdoPtr<FdoICommandCapabilities> commandCapabilities = connection->GetCommandCapabilities();
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

      FdoPtr<FdoIFilterCapabilities> filterCapabilities = connection->GetFilterCapabilities();
      FdoConditionType *conditionTypes = filterCapabilities->GetConditionTypes(size);
      FdoDistanceOperations *distanceOperations = filterCapabilities->GetDistanceOperations(size);
      FdoSpatialOperations *spatialOperations = filterCapabilities->GetSpatialOperations(size);
      bool supportsGeodesicDistance = filterCapabilities->SupportsGeodesicDistance();
      bool nonLiteralGeometricOperations = filterCapabilities->SupportsNonLiteralGeometricOperations();
        
      FdoPtr<FdoIExpressionCapabilities> expressionCapabilities = connection->GetExpressionCapabilities();
      FdoExpressionType *expressionTypes = expressionCapabilities->GetExpressionTypes(size);
      FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCapabilities->GetFunctions();

      m_numSupportedFunctions = 0;
      if (functions) {

          // Initialize the list of expected functions and their signature.
          InitSupportedFunctions();
          m_currFctObject    = NULL;

          // Document the list of supported functions and their signatures. At the same
          // time cross-check against the expected function list.
          printf("\nSupported functions: \n");
          for (int i=0; i<functions->GetCount(); i++) {

            FdoPtr<FdoFunctionDefinition> func = functions->GetItem( i );
            printf("Func: %ls: %ls\n", func->GetName(), func->GetDescription() );
            CheckFunction( func->GetName(), func->IsAggregate() );
            FdoPtr<FdoReadOnlySignatureDefinitionCollection> signatures = func->GetSignatures();
            if (signatures) {

                for (int k=0; k<signatures->GetCount(); k++) {

                  FdoPtr<FdoSignatureDefinition> signature = signatures->GetItem(k);
                  printf("\tSignature%d= return type = %ls \n", k, GetDataType(signature->GetReturnType()));
                  FdoPtr<FdoReadOnlyArgumentDefinitionCollection> args = signature->GetArguments();
                  if (args) {

                      for (int j=0; j<args->GetCount(); j++) {

                        FdoPtr<FdoArgumentDefinition>arg = args->GetItem( j );
                        printf("\t\tArg%d= %ls: %ls\n", j, arg->GetName(), arg->GetDescription() );
                        CheckSignature( signature->GetReturnType(), arg->GetName(), arg->GetDataType() );

                      }  //  for (

                  }  //  if (args)

                }  //  for (int k ...

            }  //  if (signatures) ...

          }  //  for (int i ...

          CheckExpectation();

      }  //  if (functions) ...

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
//        dictionary->SetProperty(L"Username", UnitTestUtil::GetEnviron("username"));
//	    const wchar_t *password = dictionary->GetProperty(L"Password");
//	    dictionary->SetProperty(L"Password", UnitTestUtil::GetEnviron("password"));
//	    dictionary->SetProperty(L"DataStore", UnitTestUtil::GetEnviron("datastore"));
//	    dictionary->SetProperty(L"Service", UnitTestUtil::GetEnviron("service"));
//#else
//	    FdoStringP temp;
//	    temp = UnitTestUtil::GetEnviron("username");
//        dictionary->SetProperty(L"Username", temp);
//	    const wchar_t *password = dictionary->GetProperty(L"Password");
//	    temp = UnitTestUtil::GetEnviron("password");
//	    dictionary->SetProperty(L"Password", temp);
//	    temp = UnitTestUtil::GetEnviron("datastore");
//	    dictionary->SetProperty(L"DataStore", temp);
//	    temp = UnitTestUtil::GetEnviron("service");
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
        for (int i = 0; i < 20; i++)
        {
            if ( m_supportedFunctions[i] ) {
                delete m_supportedFunctions[i];
                m_supportedFunctions[i] = NULL;
            }
        }
    }  //  try ...

    catch (FdoException *ex) {

      if (connection)
          connection->Close ();

        for (int i = 0; i < 20; i++)
        {
            if ( m_supportedFunctions[i] ) {
                delete m_supportedFunctions[i];
                m_supportedFunctions[i] = NULL;
            }
        }
       TestCommonFail(ex);
    }  //  catch ...

}  //  Connection ()

void FdoCapabilityTest::Datastore()
{
    FdoPtr<FdoIConnection> connection;
    try {

      connection = UnitTestUtil::GetProviderConnectionObject();

      int count;

      FdoPtr<FdoIConnectionInfo> connectionInfo = connection->GetConnectionInfo();
	  FdoPtr<FdoIConnectionPropertyDictionary> dictionary = connectionInfo->GetConnectionProperties();

#ifdef _WIN32
      dictionary->SetProperty(L"Username", UnitTestUtil::GetEnviron("username"));
	  const wchar_t *password = dictionary->GetProperty(L"Password");
	  dictionary->SetProperty(L"Password", UnitTestUtil::GetEnviron("password"));
	  dictionary->SetProperty(L"Service", (FdoStringP)UnitTestUtil::GetEnviron("service"));
#else
	  FdoStringP temp;
	  temp = UnitTestUtil::GetEnviron("username");
      dictionary->SetProperty(L"Username", temp);
	  const wchar_t *password = dictionary->GetProperty(L"Password");
	  temp = UnitTestUtil::GetEnviron("password");
	  dictionary->SetProperty(L"Password", temp);
	  temp = UnitTestUtil::GetEnviron("service");
	  dictionary->SetProperty(L"Service", temp);

#endif

      connection->Open ();
      FdoString ** datastores = dictionary->EnumeratePropertyValues(L"DataStore", count);
      for (int i=0; i<count; i++)
        printf("%d. %ls \n", i, datastores[i]);

      connection->Close ();
    }  //  try ...

    catch (FdoException *ex) {

      if (connection)
          connection->Close ();
      TestCommonFail(ex);
    }  //  catch ...

}  //  Datastore ()

void FdoCapabilityTest::Schema ()
{

    bool           schemaCapabilityCheckFailed = false;
    FdoPtr<FdoIConnection> connection;

    try {

      // Initialize the expected schema capability data. The data is used in
      // cross-checks against the data returned by the corresponding APIs.

      InitExpectedSchemaCapabilityData();

      // Get the connection.

      connection = UnitTestUtil::GetProviderConnectionObject();

      printf("------------------------------------\n");
      printf(">>> Checking Schema Capabilities <<<\n");
      printf("------------------------------------\n\n");
      FdoPtr<FdoISchemaCapabilities> schemaCapabilities =
                                        connection->GetSchemaCapabilities();

      // Check the supported class types.

      try {

        Schema_CheckClassTypes(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {
        schemaCapabilityCheckFailed = true;
      }  //  catch ...

      // Check the supported data types.

      try {

        Schema_CheckDataTypes(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {
        schemaCapabilityCheckFailed = true;
      }  //  catch ...

      // Check the supported identity property data types.

      try {

        Schema_CheckIdPropDataTypes(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      // Check the list of data types for autogenerated properties.

      try {

        Schema_CheckAutoGeneratedDataTypes(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      // Check the data value size limits.

      try {

        Schema_CheckMaxDataValueLimits(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      // Check the name size limits.

      try {

        Schema_CheckNameSizeLimits(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      // Check the list of reserved characters.

      try {

        Schema_CheckReservedCharacters(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      // Check the schema capability flags.

      try {

        Schema_CheckFlags(schemaCapabilities);

      }  //  try ...
      catch ( FdoException* exc ){
        exc->Release();
        schemaCapabilityCheckFailed = true;
      }
      catch ( ... ) {

        schemaCapabilityCheckFailed = true;

      }  //  catch ...

      connection->Close ();

      if (schemaCapabilityCheckFailed)
          throw FdoException::Create(
                 L"Unexpected failures in schema capability checking (see previous messages for details)");

    }  //  try ...

    catch (FdoException *ex) {

      if (connection)
          connection->Close ();

      TestCommonFail(ex);
    }  //  catch ...

}  //  Schema ()


// ---------------------------------------------------------------------------
//                            Supporting Functions                            
// ---------------------------------------------------------------------------

void FdoCapabilityTest::CheckExpectation()
{

    int                             i,
                                    j,
                                    k;
    bool                            issuesFound    = false;
    FdoCapabilityTest::FctSignature *currSignature = NULL;
    FdoCapabilityTest::ArgObject    *currArgument  = NULL;

    for (i = 0; i < m_numSupportedFunctions; i++) {

      m_currFctObject = m_supportedFunctions[i];
      if (!m_currFctObject->found) {

        printf("\tFunction %ls expected, not found\n", m_currFctObject->name);
        issuesFound = true;
        break;

      }  //  if (!m_currFctObject->found) ..

      // The function was found, but what about the signatures?

      for (j = 0; j < m_currFctObject->signatureCount; j++) {

        currSignature = m_currFctObject->signature[j];
        if (!currSignature->found) {

            printf("\tSignature for function %ls expected, not found: ",
                   m_currFctObject->name);
            printf("Return type: %ls ",
                   GetDataType(currSignature->returnDataType));
            for (int l = 0; l < currSignature->argCount; l++) {

              currArgument = currSignature->arguments[l];
              printf("Arg%d: name=%ls, type=%ls; ",
                     l,
                     currArgument->name,
                     GetDataType(currArgument->dataType));

            }  //  for ( ...

            printf("\n");
            issuesFound = true;
            continue;

        }  //  if (!currSignature->found) ...

        // OK, the signature was found, but also all the arguments?

        for (k = 0; k < currSignature->argCount; k++) {

          currArgument = currSignature->arguments[k];
          if (!currArgument->found) {

              printf("\tIncomplete signature for function %ls expected: ",
                     m_currFctObject->name );
              printf("Return type: %ls ",
                     GetDataType(currSignature->returnDataType));
              printf("Arg%d: name=%ls, type=%ls; ",
                     k,
                     currArgument->name,
                     GetDataType(currArgument->dataType));
              printf("\n");
              issuesFound = true;
              break;

          }  //  if (!currArgument->found) ...

        }  //  for (k = 0; ...

      }  //  for (j = 0; ...

    }  //  for (i = 0; ...

    if (issuesFound)
        throw FdoException::Create(
                                L"Unexpected issues in function definitions");

}  //  CheckExpectation ()

void FdoCapabilityTest::CheckFunction(FdoString *name, bool isAggregate)
{

    int  i;
    bool found;

    m_currFctObject = NULL;

    for (i = 0, found = false; i < m_numSupportedFunctions && !found; i++) {

      if ((wcscmp(m_supportedFunctions[i]->name, name) == 0   ) &&
          (m_supportedFunctions[i]->isAggregate == isAggregate)    ) {

          m_supportedFunctions[i]->found = true;
          m_currFctObject = m_supportedFunctions[i];
          found = true;

        }  //  if (( ...

    }  //  for (i = 0, ...

}  //  CheckFunction ()

void FdoCapabilityTest::CheckSignature (FdoDataType fctReturnType,
                                        FdoString   *argName,
                                        FdoDataType argDataType)
{

    int                             i,
                                    j;
    bool                            afound,
                                    sfound;
    FdoCapabilityTest::FctSignature *currSignature = NULL;
    FdoCapabilityTest::ArgObject    *currArgument = NULL;

    if (m_currFctObject == NULL)
        return;

    for (i = 0, sfound = false;
         i < m_currFctObject->signatureCount && !sfound;
         i++) {

      currSignature = m_currFctObject->signature[i];
      if (currSignature->returnDataType == fctReturnType) {

          for (j = 0, afound = false;
               j < currSignature->argCount && !afound;
               j++) {

            currArgument = currSignature->arguments[j];
            if ((wcscmp(currArgument->name, argName) == 0) &&
                (currArgument->dataType == argDataType   )    ) {

                currArgument->found = true;
                afound = true;

            }  //  if ((wcscmp( ...

          }  //  for (j=0, ...

          if (afound) {

              currSignature->found = true;
              sfound = true;

          }  //  if (afound) ...

      }  //  if (currSignature-> ...

    }  //  for (i=0, ...

}  //  Checksignature ()

FdoStringP FdoCapabilityTest::GetDataValueLimitErrorMsg (
                                                    FdoString *dataTypeId,
                                                    FdoInt64  expectedValue,
                                                    FdoInt64  actualValue)
{

    FdoStringP msg;

#ifdef _WIN32

    msg =
        FdoStringP::Format(
                    L"%ls Data Value Limit (exp.: %I64d; retrieved: %I64d)",
                    dataTypeId,
                    expectedValue,
                    actualValue);

#else

    msg =
        FdoStringP::Format(
                    L"%ls Data Value Limit (exp.: %lld; retrieved: %lld)",
                    dataTypeId,
                    expectedValue,
                    actualValue);

#endif

    return msg;

}  //  GetDataValueLimitErrorMsg ()

FdoString *FdoCapabilityTest::GetDataType(FdoDataType dataType)
{
    switch (dataType) {

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

    }  //  switch ...

}  //  GetDataType ()

FdoStringP FdoCapabilityTest::GetUncheckedDataTypes (
                                    std::vector<DataTypeElement*> dataTypes)
{

    bool        uncheckedItemFound = false;
    FdoStringP  dataTypeString;

    size_t dataTypesCount = dataTypes.size();
    for (size_t i = 0; i < dataTypesCount; i++)
      if (!dataTypes[i]->checked) {

          if (uncheckedItemFound)
              dataTypeString = dataTypeString + L", ";
          dataTypeString = 
                dataTypeString + MapDataTypeToString(dataTypes[i]->dataType);
          uncheckedItemFound = true;

      }  //  if ( ...

    return dataTypeString;

}  //  GetUncheckedDataTypes ()

FdoStringP FdoCapabilityTest::GetUnexpectedFlagValueErrorString (
                                                    FdoString *flagName,
                                                    bool      expected_value,
                                                    bool      actualValue)
{

    FdoStringP msg =
                 FdoStringP::Format(L"%ls (exp.: '%ls'; retrieved: '%ls')",
                                    flagName,
                                    ((expected_value) ? L"true" : L"false"),
                                    ((actualValue)    ? L"true" : L"false"));
    return msg;

}  //  GetUnexpectedFlagValueErrorString ()


FdoCapabilityTest::ArgObject *FdoCapabilityTest::InitArgument (
                                                    FdoString   *argName,
                                                    FdoDataType argDataType)
{

    ArgObject *argument = new ArgObject();
    argument->name = new wchar_t[wcslen(argName)+1];
    wcscpy(argument->name, argName);
    argument->dataType = argDataType;
    return argument;

}  //  InitArgument ()

FdoCapabilityTest::ClassTypeElement
                *FdoCapabilityTest::InitClassTypeElement (
                                                    FdoClassType classType)
{

    ClassTypeElement *classTypeElement = new ClassTypeElement();
    classTypeElement->classType = classType;
    classTypeElement->checked   = false;
    return classTypeElement;

}  //  InitClassTypeElement ()

FdoCapabilityTest::DataTypeElement
                *FdoCapabilityTest::InitDataTypeElement (FdoDataType dataType)
{

    DataTypeElement *dataTypeElement = new DataTypeElement();
    dataTypeElement->dataType        = dataType;
    dataTypeElement->checked         = false;
    return dataTypeElement;

}  //  InitDataTypeElement ()

void FdoCapabilityTest::InitExpectedSchemaCapabilityData ()
{

    // If there is no new object yet to keep the expected schema capability
    // data create it. Otherwise ensure it is cleared.

    if (m_expectedSchemaCapabilities == NULL)
        m_expectedSchemaCapabilities = new ExpectedSchemaCapabilities();
    else
      m_expectedSchemaCapabilities->Clear();

    // Add the expected class types supported by the provider.

    InitSchemaCapabilitiesClassTypes();

    // Add the list of data types that can be used for autogenerated properties
    // with the provider.

    InitSchemaCapabilitiesAutogeneratedDataTypes();

    // Add the expected data types supported by the provider.

    InitSchemaCapabilitiesDataTypes();

    // Add the expected data types supported for identity properties.

    InitSchemaCapabilitiesIdPropDataTypes();

    // Add the expected data value limitations.

    InitSchemaCapabilitiesMaxDataValueLimits();

    // Add the expected schema name size limits.

    InitSchemaCapabilitiesNameSizeLimits();

    // Add the list of reserved characters.

    InitSchemaCapabilitiesReservedCharacters();

    // Add the expected flag values.

    InitSchemaCapabilitiesFlags();

}  //  InitExpectedSchemaCapabilityData ()

void FdoCapabilityTest::InitSchemaCapabilitiesAutogeneratedDataTypes ()
{

    DataTypeElement *newDataTypeElement = NULL;

    // Add the expected data types supported for autogenerated properties.

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int64);
    m_expectedSchemaCapabilities->autogeneratedDataTypes.push_back(
                                                        newDataTypeElement);

}  //  InitSchemaCapabilitiesAutogeneratedDataTypes ()

void FdoCapabilityTest::InitSchemaCapabilitiesClassTypes ()
{

    ClassTypeElement *newClassTypeElement = NULL;

    newClassTypeElement = InitClassTypeElement(FdoClassType_Class);
    m_expectedSchemaCapabilities->classTypes.push_back(newClassTypeElement);

    newClassTypeElement = InitClassTypeElement(FdoClassType_FeatureClass);
    m_expectedSchemaCapabilities->classTypes.push_back(newClassTypeElement);

}  //  InitSchemaCapabilitiesClassTypes ()

void FdoCapabilityTest::InitSchemaCapabilitiesDataTypes ()
{

    DataTypeElement *newDataTypeElement = NULL;

    // Add the expected data types supported for identity properties.

    newDataTypeElement = InitDataTypeElement(FdoDataType_Boolean);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Byte);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_DateTime);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Decimal);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Double);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int16);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int32);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int64);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Single);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_String);
    m_expectedSchemaCapabilities->dataTypes.push_back(newDataTypeElement);

}  //  InitSchemaCapabilitiesDataTypes ()

void FdoCapabilityTest::InitSchemaCapabilitiesFlags ()
{

    // Only set those flags that need to be changed to FALSE.

    m_expectedSchemaCapabilities->supportsNetworkModel = false;

}  //  InitSchemaCapabilitiesFlags ()

void FdoCapabilityTest::InitSchemaCapabilitiesIdPropDataTypes ()
{

    DataTypeElement *newDataTypeElement = NULL;

    // Add the expected data types supported for identity properties.

    newDataTypeElement = InitDataTypeElement(FdoDataType_Boolean);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Byte);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_DateTime);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Decimal);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Double);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int16);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int32);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Int64);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_Single);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

    newDataTypeElement = InitDataTypeElement(FdoDataType_String);
    m_expectedSchemaCapabilities->idPropTypes.push_back(newDataTypeElement);

}  //  InitSchemaCapabilitiesIdPropDataTypes ()

void FdoCapabilityTest::InitSchemaCapabilitiesMaxDataValueLimits ()
{

    m_expectedSchemaCapabilities->blobDataValueLimit     = (FdoInt64)4000000000LL;
    m_expectedSchemaCapabilities->booleanDataValueLimit  = (FdoInt64)sizeof(FdoBoolean);
    m_expectedSchemaCapabilities->byteDataValueLimit     = (FdoInt64)sizeof(FdoByte);
    m_expectedSchemaCapabilities->clobDataValueLimit     = (FdoInt64)4000000000LL;
    m_expectedSchemaCapabilities->dateTimeDataValueLimit = (FdoInt64)sizeof(FdoDateTime);
    m_expectedSchemaCapabilities->decimalDataValueLimit  = (FdoInt64)(38+127);
    m_expectedSchemaCapabilities->doubleDataValueLimit   = (FdoInt64)sizeof(FdoDouble);
    m_expectedSchemaCapabilities->int16DataValueLimit    = (FdoInt64)sizeof(FdoInt16);
    m_expectedSchemaCapabilities->int32DataValueLimit    = (FdoInt64)sizeof(FdoInt32);
    m_expectedSchemaCapabilities->int64DataValueLimit    = (FdoInt64)sizeof(FdoInt64);
    m_expectedSchemaCapabilities->singleDataValueLimit   = (FdoInt64)sizeof(FdoFloat);
    m_expectedSchemaCapabilities->stringDataValueLimit   = (FdoInt64)4000;

}  //  InitSchemaCapabilitiesMaxDataValueLimits ()

void FdoCapabilityTest::InitSchemaCapabilitiesNameSizeLimits ()
{

    m_expectedSchemaCapabilities->classNameLimit     = 255;
    m_expectedSchemaCapabilities->datastoreNameLimit = 30;
    m_expectedSchemaCapabilities->descriptionLimit   = 255;
    m_expectedSchemaCapabilities->propertyNameLimit  = 255;
    m_expectedSchemaCapabilities->schemaNameLimit    = 255;

}  //  InitSchemaCapabilitiesNameSizeLimits ()

void FdoCapabilityTest::InitSchemaCapabilitiesReservedCharacters ()
{

    m_expectedSchemaCapabilities->reservedCharacters = L".:";

}  //  InitSchemaCapabilitiesReservedCharacters ()

FdoCapabilityTest::FctSignature
        *FdoCapabilityTest::InitSignature (FdoDataType     retDataType,
                                           FdoPropertyType retPropertyType,
                                           bool            expDataType)
{

    FctSignature *signature = new FctSignature();
    signature->expectDataType = expDataType;
    signature->returnDataType = retDataType;
    signature->returnPropertyType = retPropertyType;
    return signature;

}  //  InitSignature ()

void FdoCapabilityTest::InitSupportedFunctions ()
{

    for (int i = 0; i < 20; i++)
        m_supportedFunctions[i] = NULL;

    // Add the list of expected functions.
    m_supportedFunctions[0] = new FctObject();
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

    m_supportedFunctions[1] = new FctObject();
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

    m_supportedFunctions[2] = new FctObject();
    m_supportedFunctions[2]->name = new wchar_t[wcslen(FDO_FUNCTION_CONCAT)+1];
    wcscpy(m_supportedFunctions[2]->name, FDO_FUNCTION_CONCAT);
    m_supportedFunctions[2]->isAggregate = false;
    m_supportedFunctions[2]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[2]->signature[0]->arguments[0] = InitArgument(L"strValue1", FdoDataType_String);
    m_supportedFunctions[2]->signature[0]->arguments[1] = InitArgument(L"strValue2", FdoDataType_String);
    m_supportedFunctions[2]->signatureCount = 1;
    m_supportedFunctions[2]->signature[0]->argCount = 2;

    m_supportedFunctions[3] = new FctObject();
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

    m_supportedFunctions[4] = new FctObject();
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

    m_supportedFunctions[5] = new FctObject();
    m_supportedFunctions[5]->name = new wchar_t[wcslen(FDO_FUNCTION_LOWER)+1];
    wcscpy(m_supportedFunctions[5]->name, FDO_FUNCTION_LOWER);
    m_supportedFunctions[5]->isAggregate = false;
    m_supportedFunctions[5]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[5]->signature[0]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[5]->signatureCount = 1;

    m_supportedFunctions[6] = new FctObject();
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

    m_supportedFunctions[7] = new FctObject();
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

    m_supportedFunctions[8] = new FctObject();
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

    m_supportedFunctions[9] = new FctObject();
    m_supportedFunctions[9]->name = new wchar_t[wcslen(FDO_FUNCTION_UPPER)+1];
    wcscpy(m_supportedFunctions[9]->name, FDO_FUNCTION_UPPER);
    m_supportedFunctions[9]->isAggregate = false;
    m_supportedFunctions[9]->signature[0] = InitSignature(FdoDataType_String, FdoPropertyType_DataProperty, true);
    m_supportedFunctions[9]->signature[0]->arguments[0] = InitArgument(L"strValue", FdoDataType_String);
    m_supportedFunctions[9]->signatureCount = 1;

    m_numSupportedFunctions = 10;

}  //  InitSupportedFunctions ()

FdoStringP FdoCapabilityTest::MapClassTypeToString (FdoClassType classType)
{

    switch (classType) {

      case FdoClassType_Class:
        return(L"FdoClassType_Class");
        break;

      case FdoClassType_FeatureClass:
        return(L"FdoClassType_FeatureClass");
        break;

      default:
        throw FdoException::Create(L"Unexpected class type to map");
        break;

    }  //  switch ...

}  //  MapDataTypeToString ()

FdoStringP FdoCapabilityTest::MapDataTypeToString (FdoDataType dataType)
{

    switch (dataType) {

      case FdoDataType_Boolean:
        return(L"FdoDataType_Boolean");
        break;

      case FdoDataType_Byte:
        return(L"FdoDataType_Byte");
        break;

      case FdoDataType_DateTime:
        return(L"FdoDataType_DateTime");
        break;

      case FdoDataType_Decimal:
        return(L"FdoDataType_Decimal");
        break;

      case FdoDataType_Double:
        return(L"FdoDataType_Double");
        break;

      case FdoDataType_Int16:
        return(L"FdoDataType_Int16");
        break;

      case FdoDataType_Int32:
        return(L"FdoDataType_Int32");
        break;

      case FdoDataType_Int64:
        return(L"FdoDataType_Int64");
        break;

      case FdoDataType_Single:
        return(L"FdoDataType_Single");
        break;

      case FdoDataType_String:
        return(L"FdoDataType_String");
        break;

      case FdoDataType_BLOB:
        return(L"FdoDataType_BLOB");
        break;

      case FdoDataType_CLOB:
        return(L"FdoDataType_CLOB");
        break;

      default:
        throw FdoException::Create(L"Unexpected data type to map");
        break;

    }  //  switch ...

}  //  MapDataTypeToString ()

void FdoCapabilityTest::MarkDataType (std::vector<DataTypeElement*> dataTypes,
                                      FdoDataType                   dataType)
{

    size_t dataTypesCount = dataTypes.size();
    for (size_t i = 0; i < dataTypesCount; i++)
      if (dataTypes[i]->dataType == dataType) {

          dataTypes[i]->checked = true;
          break;

      }  //  if ( ...

}  //  MarkDataTypes ()

void FdoCapabilityTest::Schema_CheckAutoGeneratedDataTypes (
                                FdoISchemaCapabilities *schemaCapabilities)
{

    printf("\n");
    printf(">>> Checking supported data types for autogenerated identity properties \n");
    printf(">>> ------------------------------------------------------------------- \n");

    FdoInt32 listCount;
    FdoDataType *supportedDataTyes =
                schemaCapabilities->GetSupportedAutoGeneratedTypes(listCount);

    for (FdoInt32 i = 0; i < listCount; i++)
      MarkDataType(m_expectedSchemaCapabilities->autogeneratedDataTypes,
                   supportedDataTyes[i]);

    // Cross check the returned supported data types versus the list of
    // expected data types.

    FdoStringP uncheckedDataTypes =
                GetUncheckedDataTypes(
                        m_expectedSchemaCapabilities->autogeneratedDataTypes);

    if (uncheckedDataTypes == NULL)
        printf(">>> ... All expected data types for autogenerated properties are supported\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(
            L"%ls '%ls' \n",
            L"These expected data types are not supported for autogenerated identity properties:",
            (FdoString *) uncheckedDataTypes);
      printf(">>> ... These expected data types are not supported for autogenerated identity properties: '%ls' \n",
             (FdoString *) uncheckedDataTypes);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckAutoGeneratedDataTypes ()

void FdoCapabilityTest::Schema_CheckClassTypes (
                                    FdoISchemaCapabilities *schemaCapabilities)
{

    printf(">>> Checking supported class types \n");
    printf(">>> ------------------------------ \n");

    FdoInt32 listCount;
    FdoClassType *supportedClassTyes =
                                schemaCapabilities->GetClassTypes(listCount);

    for (FdoInt32 i = 0; i < listCount; i++)
      Schema_MarkClassTypes(supportedClassTyes[i]);

    // Cross check the returned supported class types versus the list of
    // expected class types.

    Schema_CrossCheckClassTypes();

}  //  Schema_CheckClassTypes ()

void FdoCapabilityTest::Schema_CheckDataTypes (
                                    FdoISchemaCapabilities *schemaCapabilities)
{

    printf("\n");
    printf(">>> Checking supported data types \n");
    printf(">>> ----------------------------- \n");

    FdoInt32 listCount;
    FdoDataType *supportedDataTyes =
                                schemaCapabilities->GetDataTypes(listCount);

    for (FdoInt32 i = 0; i < listCount; i++)
      MarkDataType(m_expectedSchemaCapabilities->dataTypes,
                   supportedDataTyes[i]);

    // Cross check the returned supported data types versus the list of
    // expected data types.

    FdoStringP uncheckedDataTypes =
                    GetUncheckedDataTypes(
                                    m_expectedSchemaCapabilities->dataTypes);

    if (uncheckedDataTypes == NULL)
        printf(">>> ... All expected data types are supported\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(L"%ls '%ls' \n",
                           L"These expected data types are not supported:",
                           (FdoString *) uncheckedDataTypes);
      printf(">>> ... These expected data types are not supported: '%ls' \n",
             (FdoString *) uncheckedDataTypes);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckDataTypes ()

void FdoCapabilityTest::Schema_CheckFlags (
                                    FdoISchemaCapabilities *schemaCapabilities)
{

    bool       flagValue,
               expFlagValue,
               unexpectedFlagValueFound = false;
    FdoStringP parameter,
               errMsgParameter;

    printf("\n");
    printf(">>> Checking schema capability flags \n");
    printf(">>> -------------------------------- \n");

    flagValue    = schemaCapabilities->SupportsAssociationProperties();
    expFlagValue = m_expectedSchemaCapabilities->supportsAssociationProperties;
    if (flagValue != expFlagValue) {

        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                        L"Supports Association Properties",
                                        expFlagValue,
                                        flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...


    flagValue    = schemaCapabilities->SupportsAutoIdGeneration();
    expFlagValue = m_expectedSchemaCapabilities->supportsAutoIdGeneration;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                            L"Supports Auto Id Generation",
                                            expFlagValue,
                                            flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsCompositeId();
    expFlagValue = m_expectedSchemaCapabilities->supportsCompositeId;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                    L"Supports Composite Id",
                                                    expFlagValue,
                                                    flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    =
        schemaCapabilities->SupportsCompositeUniqueValueConstraints();
    expFlagValue =
        m_expectedSchemaCapabilities->supportsCompositeUniqueValueConstraints;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                            L"Supports Composite Unique Value Constraints",
                            expFlagValue,
                            flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    =
        schemaCapabilities->SupportsDataStoreScopeUniqueIdGeneration();
    expFlagValue =
        m_expectedSchemaCapabilities->supportsDataStoreScopeUniqueIdGeneration;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                            L"Supports Data Store Scope Unique Id Generation",
                            expFlagValue,
                            flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsDefaultValue();
    expFlagValue = m_expectedSchemaCapabilities->supportsDefaultValue;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                    L"Supports Default Value",
                                                    expFlagValue,
                                                    flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    =
        schemaCapabilities->SupportsExclusiveValueRangeConstraints();
    expFlagValue =
        m_expectedSchemaCapabilities->supportsExclusiveValueRangeConstraints;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                L"Supports Exclusive Value Range Constraints",
                                expFlagValue,
                                flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    =
        schemaCapabilities->SupportsInclusiveValueRangeConstraints();
    expFlagValue =
        m_expectedSchemaCapabilities->supportsInclusiveValueRangeConstraints;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                L"Supports Inclusive Value Range Constraints",
                                expFlagValue,
                                flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsInheritance();
    expFlagValue = m_expectedSchemaCapabilities->supportsInheritance;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                    L"Supports Inheritance",
                                                    expFlagValue,
                                                    flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsMultipleSchemas();
    expFlagValue = m_expectedSchemaCapabilities->supportsMultipleSchemas;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                L"Supports Multiple Schemas",
                                                expFlagValue,
                                                flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsNetworkModel();
    expFlagValue = m_expectedSchemaCapabilities->supportsNetworkModel;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                    L"Supports Network Model",
                                                    expFlagValue,
                                                    flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsNullValueConstraints();
    expFlagValue = m_expectedSchemaCapabilities->supportsNullValueConstraints;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                        L"Supports NULL Value Constraints",
                                        expFlagValue,
                                        flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsObjectProperties();
    expFlagValue = m_expectedSchemaCapabilities->supportsObjectProperties;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                L"Supports Object Properties",
                                                expFlagValue,
                                                flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsSchemaModification();
    expFlagValue = m_expectedSchemaCapabilities->supportsSchemaModification;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                            L"Supports Schema Modification",
                                            expFlagValue,
                                            flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsSchemaOverrides();
    expFlagValue = m_expectedSchemaCapabilities->supportsSchemaOverrides;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                                L"Supports Schema Overrides",
                                                expFlagValue,
                                                flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    =
        schemaCapabilities->SupportsUniqueValueConstraints();
    expFlagValue =
        m_expectedSchemaCapabilities->supportsUniqueValueConstraints;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                        L"Supports Unique Value Constraints",
                                        expFlagValue,
                                        flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    flagValue    = schemaCapabilities->SupportsValueConstraintsList();
    expFlagValue = m_expectedSchemaCapabilities->supportsValueConstraintsList;
    if (flagValue != expFlagValue) {

        if (unexpectedFlagValueFound)
            errMsgParameter = errMsgParameter + L", ";
        unexpectedFlagValueFound = true;
        parameter = GetUnexpectedFlagValueErrorString(
                                        L"Supports Value Constraints List",
                                        expFlagValue,
                                        flagValue);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (flagValue != ...

    if (!unexpectedFlagValueFound)
        printf(">>> ... All flags set as expected\n");
    else {

      FdoStringP errMsg =
                 FdoStringP::Format(L"%ls '%ls' \n",
                                    L"These flags are not set as expected:",
                                    (FdoString *) errMsgParameter);
      printf(">>> ... These flags are not set as expected: '%ls' \n",
             (FdoString *) errMsgParameter);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckFlags ()

void FdoCapabilityTest::Schema_CheckIdPropDataTypes (
                                    FdoISchemaCapabilities *schemaCapabilities)
{

    printf("\n");
    printf(">>> Checking supported data types for identity properties \n");
    printf(">>> ----------------------------------------------------- \n");

    FdoInt32 listCount;
    FdoDataType *supportedIdDataTyes =
            schemaCapabilities->GetSupportedIdentityPropertyTypes(listCount);

    for (FdoInt32 i = 0; i < listCount; i++)
      MarkDataType(m_expectedSchemaCapabilities->idPropTypes,
                   supportedIdDataTyes[i]);

    // Cross check the returned supported data types versus the list of
    // expected data types.

    FdoStringP uncheckedDataTypes =
                    GetUncheckedDataTypes(
                                m_expectedSchemaCapabilities->idPropTypes);

    if (uncheckedDataTypes == NULL)
        printf(">>> ... All expected data types for identity properties are supported\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(
            L"%ls '%ls' \n",
            L"These expected data types are not supported for identity properties:",
            (FdoString *) uncheckedDataTypes);
      printf(">>> ... These expected data types are not supported for identity properties: '%ls' \n",
             (FdoString *) uncheckedDataTypes);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckIdPropDataTypes ()

void FdoCapabilityTest::Schema_CheckMaxDataValueLimits (
                                FdoISchemaCapabilities *schemaCapabilities)
{

    bool       limitationErrorFound = false;
    FdoInt64   currExpLimit;
    FdoStringP errMsgParameter;
    FdoStringP parameter;

    printf("\n");
    printf(">>> Checking data value size limits \n");
    printf(">>> ------------------------------- \n");

    // Get the data value size limits the provider has.

    FdoInt64 blobValueLength     =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                            FdoDataType_BLOB);
    FdoInt64 clobValueLength     =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                            FdoDataType_CLOB);
    FdoInt64 decimalValueLength  =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Decimal);
    FdoInt64 booleanValueLength  =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Boolean);
    FdoInt64 byteValueLength     =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                            FdoDataType_Byte);
    FdoInt64 dateTimeValueLength =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_DateTime);
    FdoInt64 doubleValueLength   =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Double);
    FdoInt64 int16ValueLength    =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Int16);
    FdoInt64 int32ValueLength    =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Int32);
    FdoInt64 int64ValueLength    =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Int64);
    FdoInt64 singleValueLength   =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_Single);
    FdoInt64 stringValueLength   =
                    schemaCapabilities->GetMaximumDataValueLength(
                                                        FdoDataType_String);

    // Cross check the returned data value size limitations against the expected
    // data value size limitations.

    currExpLimit = m_expectedSchemaCapabilities->blobDataValueLimit;

    if (blobValueLength != currExpLimit) {

        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(L"BLOB", currExpLimit, blobValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (blobValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->booleanDataValueLimit;

    if (booleanValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                    L"Boolean", currExpLimit, blobValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (booleanValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->byteDataValueLimit;

    if (byteValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(L"Byte", currExpLimit, byteValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (byteValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->clobDataValueLimit;

    if (clobValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(L"CLOB", currExpLimit, clobValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (clobValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->dateTimeDataValueLimit;

    if (dateTimeValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                            L"DateTime", currExpLimit, dateTimeValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (dateTimeValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->decimalDataValueLimit;

    if (decimalValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                L"Decimal", currExpLimit, decimalValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (decimalValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->doubleDataValueLimit;

    if (doubleValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                L"Double", currExpLimit, doubleValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (doubleValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->int16DataValueLimit;

    if (int16ValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                    L"Int16", currExpLimit, int16ValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (int16ValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->int32DataValueLimit;

    if (int32ValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                    L"Int32", currExpLimit, int32ValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (int32ValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->int64DataValueLimit;

    if (int64ValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                    L"Int64", currExpLimit, int64ValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (int64ValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->singleDataValueLimit;

    if (singleValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                L"Single", currExpLimit, singleValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (singleValueLength != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->stringDataValueLimit;

    if (stringValueLength != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter =
            GetDataValueLimitErrorMsg(
                                L"String", currExpLimit, stringValueLength);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (stringValueLength != currExpLimit) ...

    if (!limitationErrorFound)
        printf(">>> ... Found all expected data value size limitations\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(
                L"%ls '%ls' \n",
                L"These data value size limits differ from expected values:",
                (FdoString *) errMsgParameter);
      printf(">>> ... These data value size limits differ from expected values: '%ls' \n",
             (FdoString *) errMsgParameter);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckMaxDataValueLimits ()

void FdoCapabilityTest::Schema_CheckNameSizeLimits (
                                FdoISchemaCapabilities *schemaCapabilities)
{

    bool       limitationErrorFound = false;
    FdoInt32   currExpLimit;
    FdoStringP errMsgParameter;
    FdoStringP parameter;

    printf("\n");
    printf(">>> Checking name size limits \n");
    printf(">>> ------------------------- \n");

    // Get the name size limits the provider has.

    FdoInt32 classNameLimit     =
                schemaCapabilities->GetNameSizeLimit(
                                            FdoSchemaElementNameType_Class);
    FdoInt32 datastoreNameLimit =
                schemaCapabilities->GetNameSizeLimit(
                                        FdoSchemaElementNameType_Datastore);
    FdoInt32 descriptionLimit   =
                schemaCapabilities->GetNameSizeLimit(
                                        FdoSchemaElementNameType_Description);
    FdoInt32 propertyNameLimit  =
                schemaCapabilities->GetNameSizeLimit(
                                        FdoSchemaElementNameType_Property);
    FdoInt32 schemaNameLimit    =
                schemaCapabilities->GetNameSizeLimit(
                                            FdoSchemaElementNameType_Schema);

    // Cross check the returned name size limitations to the expected name
    // size limitations.

    currExpLimit = m_expectedSchemaCapabilities->classNameLimit;

    if (classNameLimit != currExpLimit) {

        limitationErrorFound = true;
        parameter = FdoStringP::Format(
                            L"Class Name Limit (exp.: %d; retrieved: %d)",
                            currExpLimit,
                            classNameLimit);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (classNameLimit != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->datastoreNameLimit;

    if (datastoreNameLimit != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter = FdoStringP::Format(
                        L"Data Store Name Limit (exp.: %d; retrieved: %d)\n",
                        currExpLimit,
                        datastoreNameLimit);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (datastoreNameLimit != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->descriptionLimit;

    if (descriptionLimit != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter = FdoStringP::Format(
                        L"Description Limit (exp.: %d; retrieved: %d)",
                        currExpLimit,
                        descriptionLimit);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (descriptionLimit != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->propertyNameLimit;

    if (propertyNameLimit != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter = FdoStringP::Format(
                        L"Property Limit (exp.: %d; retrieved: %d)",
                        currExpLimit,
                        propertyNameLimit);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (propertyNameLimit != currExpLimit) ...

    currExpLimit = m_expectedSchemaCapabilities->schemaNameLimit;

    if (schemaNameLimit != currExpLimit) {

        if (limitationErrorFound)
            errMsgParameter = errMsgParameter + L", ";
        limitationErrorFound = true;
        parameter = FdoStringP::Format(
                        L"Schema Name Limit (exp.: %d; retrieved: %d)",
                        currExpLimit,
                        schemaNameLimit);
        errMsgParameter = errMsgParameter + parameter;

    }  //  if (schemaNameLimit != currExpLimit) ...

    if (!limitationErrorFound)
        printf(">>> ... Found all expected name size limitations\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(
            L"%ls '%ls' \n",
            L"These name size limits differ from expected values:",
            (FdoString *) errMsgParameter);
      printf(">>> ... These name size limits differ from expected values: '%ls' \n",
             (FdoString *) errMsgParameter);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckNameSizeLimits ()

void FdoCapabilityTest::Schema_CheckReservedCharacters (
                                FdoISchemaCapabilities *schemaCapabilities)
{

    printf("\n");
    printf(">>> Checking reserved characters \n");
    printf(">>> ---------------------------- \n");

    // Get the list of reserved characters.

    FdoStringP reservedCharacters =
                        schemaCapabilities->GetReservedCharactersForName();

    // Cross check the returned string containing the list of reserved
    // characters with the list of expected reserved characters.

    if (reservedCharacters == m_expectedSchemaCapabilities->reservedCharacters)
        printf(">>> ... Found all expected reserved characters\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(
            L"List of reserved characters differ (exp.: '%ls'; retrieved: '%ls')\n",
            (FdoString *) m_expectedSchemaCapabilities->reservedCharacters,
            (FdoString *) reservedCharacters);
      printf(">>> ... List of reserved characters differ (exp.: '%ls'; retrieved: '%ls')\n",
            (FdoString *) m_expectedSchemaCapabilities->reservedCharacters,
            (FdoString *) reservedCharacters);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CheckReservedCharacters ()

void FdoCapabilityTest::Schema_CrossCheckClassTypes ()
{

    bool         crossCheckErrorFound = false;
    FdoStringP   errMsgParameter;
    FdoClassType currClassType;

    size_t supportedClassTypesCount =
                            m_expectedSchemaCapabilities->classTypes.size();
    for (size_t i = 0; i < supportedClassTypesCount; i++)
      if (!m_expectedSchemaCapabilities->classTypes[i]->checked) {

          if (crossCheckErrorFound)
              errMsgParameter = errMsgParameter + L", ";
          currClassType = 
             m_expectedSchemaCapabilities->classTypes[i]->classType;
          errMsgParameter = 
                        errMsgParameter + MapClassTypeToString(currClassType);
          crossCheckErrorFound = true;

      }  //  if ( ...

    if (!crossCheckErrorFound)
        printf(">>> ... All expected class types are supported\n");
    else {

      FdoStringP errMsg =
        FdoStringP::Format(L"%ls '%ls' \n",
                           L"These expected class types are not supported:",
                           (FdoString *) errMsgParameter);
      printf(">>> ... These expected class types are not supported: '%ls' \n",
             (FdoString *) errMsgParameter);
      
      throw FdoException::Create(errMsg);

    }  //  else ...

}  //  Schema_CrossCheckClassTypes ()

void FdoCapabilityTest::Schema_MarkClassTypes (FdoClassType classType)
{

    size_t supportedClassTypesCount =
                            m_expectedSchemaCapabilities->classTypes.size();
    for (size_t i = 0; i < supportedClassTypesCount; i++)
      if (m_expectedSchemaCapabilities->classTypes[i]->classType
                                                                == classType) {

          m_expectedSchemaCapabilities->classTypes[i]->checked = true;
          break;

      }  //  if ( ...

}  //  Schema_MarkClassTypes ()


// ---------------------------------------------------------------------------
//                           Helper Class Functions                           
// ---------------------------------------------------------------------------

FdoCapabilityTest::ArgObject::ArgObject ()
: name(NULL)
{

    found    = false;
    dataType = (FdoDataType)-1;

}  //  ArgObject ()

FdoCapabilityTest::ArgObject::~ArgObject ()
{

    if (name != NULL) {

        delete[] name;
        name = NULL;

    }  //  if (name != NULL) ...

}  //  ~ArgObject ()

FdoCapabilityTest::ClassTypeElement::ClassTypeElement ()
{

    checked   = false;
    classType = (FdoClassType)-1;

}  //  ClassTypeElement ()

FdoCapabilityTest::ClassTypeElement::~ClassTypeElement ()
{

}  //  ~ClassTypeElement ()

FdoCapabilityTest::DataTypeElement::DataTypeElement ()
{

    checked  = false;
    dataType = (FdoDataType)-1;

}  //  DataTypeElement ()

FdoCapabilityTest::DataTypeElement::~DataTypeElement ()
{

}  //  ~DataTypeElement ()

FdoCapabilityTest::ExpectedSchemaCapabilities::ExpectedSchemaCapabilities ()
{

    // Initialize all class properties if necessary.

    SetToZero();

}  //  ExpectedSchemaCapabilities ()

FdoCapabilityTest::ExpectedSchemaCapabilities::~ExpectedSchemaCapabilities ()
{

}  //  ~ExpectedSchemaCapabilities ()

void FdoCapabilityTest::ExpectedSchemaCapabilities::Clear ()
{

    size_t i = 0;

    // Clear the various class lists.

    size_t numAutogeneratedDataTypes = autogeneratedDataTypes.size();
    for (i = 0; i < numAutogeneratedDataTypes; i++) {

      delete autogeneratedDataTypes[i];
      autogeneratedDataTypes[i] = NULL;

    };  //  for (i = 0; ...

    size_t numClassTypeElements = classTypes.size();
    for (i = 0; i < numClassTypeElements; i++) {

      delete classTypes[i];
      classTypes[i] = NULL;

    };  //  for (i = 0; ...

    size_t numDataTypeElements = dataTypes.size();
    for (i = 0; i < numDataTypeElements; i++) {

      delete dataTypes[i];
      dataTypes[i] = NULL;

    };  //  for (size_t i = 0; ...

    numDataTypeElements = idPropTypes.size();
    for (i = 0; i < numDataTypeElements; i++) {

      delete idPropTypes[i];
      idPropTypes[i] = NULL;

    };  //  for (size_t i = 0; ...

    SetToZero();

}  //  Clear ()

void FdoCapabilityTest::ExpectedSchemaCapabilities::SetToZero ()
{

    // Initialize all class properties if necessary.

    classNameLimit     = 0;
    datastoreNameLimit = 0;
    descriptionLimit   = 0;
    propertyNameLimit  = 0;
    schemaNameLimit    = 0;

    blobDataValueLimit     = 0;
    booleanDataValueLimit  = 0;
    byteDataValueLimit     = 0;
    clobDataValueLimit     = 0;
    dateTimeDataValueLimit = 0;
    decimalDataValueLimit  = 0;
    doubleDataValueLimit   = 0;
    int16DataValueLimit    = 0;
    int32DataValueLimit    = 0;
    int64DataValueLimit    = 0;
    singleDataValueLimit   = 0;
    stringDataValueLimit   = 0;

    supportsAssociationProperties            = true;
    supportsAutoIdGeneration                 = true;
    supportsCompositeId                      = true;
    supportsCompositeUniqueValueConstraints  = true;
    supportsDataStoreScopeUniqueIdGeneration = true;
    supportsDefaultValue                     = true;
    supportsExclusiveValueRangeConstraints   = true;
    supportsInclusiveValueRangeConstraints   = true;
    supportsInheritance                      = true;
    supportsMultipleSchemas                  = true;
    supportsNetworkModel                     = true;
    supportsNullValueConstraints             = true;
    supportsObjectProperties                 = true;
    supportsSchemaModification               = true;
    supportsSchemaOverrides                  = true;
    supportsUniqueValueConstraints           = true;
    supportsValueConstraintsList             = true;

}  //  SetToZero ()

FdoCapabilityTest::FctSignature::FctSignature ()
{

    found              = false;
    returnDataType     = (FdoDataType)-1;
    returnPropertyType = FdoPropertyType_DataProperty;
    expectDataType     = true;
    argCount           = 1;

    for (int i = 0; i < 10; i++)
      arguments[i] = NULL;

}  //  FctSignature ()

FdoCapabilityTest::FctSignature::~FctSignature ()
{

    for (int i = 0; i < 10; i++) {

      if (arguments[i] != NULL) {

          delete arguments[i];
          arguments[i] = NULL;

      }  //  if (arguments[i] != NULL) ...

    }  //  for (int i = 0; ...

}  //  ~FctSignature ()

FdoCapabilityTest::FctObject::FctObject ()
: name(NULL)
{

    found       = false;
    isAggregate = false;

    for (int i = 0; i < 20; i++)
      signature[i] = NULL;

}  //  FctObject ()

FdoCapabilityTest::FctObject::~FctObject ()
{

    if (name != NULL) {

        delete[] name;
        name = NULL;

    }  //  if (name != NULL) ...

    for (int i = 0; i < 20; i++) {

      if (signature[i] != NULL) {

          delete signature[i];
          signature[i] = NULL;

      }  //  if (signature[i] != NULL) ...

    }  //  for (int i = 0; ...

}  //  ~FctObject ()


