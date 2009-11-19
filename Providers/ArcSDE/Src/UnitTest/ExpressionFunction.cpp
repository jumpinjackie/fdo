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
#include "ExpressionFunction.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestExpressionFunction);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestExpressionFunction, "TestExpressionFunction");


// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

TestExpressionFunction::TestExpressionFunction ()
{
}

TestExpressionFunction::~TestExpressionFunction ()
{
}

void TestExpressionFunction::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{
    m_connection = ArcSDETests::GetConnection ();
    m_connection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
    m_connection->Open ();
}

void TestExpressionFunction::RunAllExpFctTests()
{
    if (mSDETests.CreateSchemaOnly())
        return;

// TODO: By default, Linux unit tests are run in release mode. However, this
// test depends on the ApplySchema command which is unavailable in release
// mode. Therefore, some rework is needed before TestXYZMFunction will work
// on Linux.
#ifdef _WIN32
    bool error_found = false;

    // Executing the geometry functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Geometry Functions \n");
    printf("\n");

    try {

      TestXYZMFunction();
      TestConcatFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    if (error_found)
        throw FdoException::Create(L"Unit tests executed with errors");
#endif
}

void TestExpressionFunction::setUp ()

// +---------------------------------------------------------------------------
// | The function represents the class setup function.
// +---------------------------------------------------------------------------

{
// See note in RunAllExpFctTests
#ifdef _WIN32
    // Declare and initialize all necessary local variables.

    FdoStringP datastore_name;


    try {

      // The following creates the data base used for the tests, opens a con-
      // nnection to it and applies a test schema.

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Started                                           \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      printf(" >>> Establishing necessary connection \n");
      Connect();
      if (mSDETests.CreateSchemaOnly())
      {
          printf(" >>> Predeleting Test Schema \n");
          DropTestSchema(GetSchemaName());
          printf(" >>> Establishing test environment \n");
          CreateSC();
          AddTestSchema(m_connection, GetSchemaName());
      }
      else
      {
          DeleteTestFeatures(m_connection);
          AddTestFeatures(m_connection);
      }

      printf(" >>> Testing ... \n");
      printf("\n");
      printf(".......................................................... \n");
      printf("\n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Disconnecting \n");
      CloseConnection();
      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test setup                             \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw exp;

    }  //  catch ...

    catch ( ... ) {

      printf(" >>> Disconnecting \n");
      CloseConnection();

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test setup                             \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...
#endif
}  //  setUp ()

void TestExpressionFunction::tearDown ()

// +---------------------------------------------------------------------------
// | The function represents the class tear-down function.
// +---------------------------------------------------------------------------

{
// See note in RunAllExpFctTests
#ifdef _WIN32

    // Declare and initialize all necessary local variables.

    bool is_disconnected = false;

    try {

      printf("\n");
      printf(".......................................................... \n");
      printf("\n");
      printf(" >>> Testing completed \n");

      printf(" >>> Disconnecting \n");
      is_disconnected = true;
      CloseConnection();

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Tests successfully executed                       \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());

      if (!is_disconnected) {

          printf(" >>> Disconnecting \n");
          CloseConnection();

      }  //  if (!is_disconnected) ...

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test tear down                         \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw exp;
    
    }  //  catch (FdoExcpetion ...

    catch ( ... ) {

      if (!is_disconnected) {

          printf(" >>> Disconnecting \n");
          CloseConnection();

      }  //  if (!is_disconnected) ...

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test tear down                         \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...
#endif
}  //  tearDown ()

void TestExpressionFunction::DropTestSchema ( FdoString* schema_name)
// +---------------------------------------------------------------------------
// | The function drops the test data store.
// +---------------------------------------------------------------------------

{

    printf(" >>> ... dropping feature schema '%ls' \n",
           schema_name);

    try
    {
        FdoPtr<FdoIDestroySchema> destroy = (FdoIDestroySchema*)m_connection->CreateCommand (FdoCommandType_DestroySchema);
        destroy->SetSchemaName ( schema_name );
        destroy->Execute ();
    }
    catch ( ... )
    {
    }
}  //  DropTestDataStore ()


void TestExpressionFunction::CreateSC()
{
    // Create spatial context:
    mSC = L"TestSC_GeomXYZMFunc";

    if ( ArcSDETestConfig::RdbmsType() == ArcSDETestConfig::ArcSDETestRdbmsType_Oracle )
    {
        CreateOrUpdateSpatialContext(
            m_connection, mSC, 
            0, 0, 9007199254740990L, 9007199254740990L, 
            0, 0, 2147483648L, 2147483648L, 
            L"26943");
    }
    else
    {
        CreateOrUpdateSpatialContext(
            m_connection, mSC, 
            0, 0, 0, 0, 
            0, 0, 1048576, 1048576, 
            L"26943");
    }
}

void TestExpressionFunction::CreateOrUpdateSpatialContext(
    FdoIConnection *conn, FdoStringP& scName, 
    double lXMin, double lYMin, double lXMax, double lYMax, 
    double cXMin, double cYMin, double cXMax, double cYMax, 
    FdoString* scCoordSys)
{
    // Determine if the spatial context already exists:
    FdoPtr<FdoIGetSpatialContexts> getSCs = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> scReader = getSCs->Execute();
    bool bExists = false;

    getSCs = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
    scReader = getSCs->Execute();
    
    while (scReader->ReadNext())
    {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoByteArray> bytes = scReader->GetExtent();
        FdoPtr<FdoIGeometry> geomObj = gf->CreateGeometryFromFgf(bytes);
        FdoPtr<FdoIEnvelope> env = geomObj->GetEnvelope();
        double eXMin = env->GetMinX();
        double eYMin = env->GetMinY();
        double eXMax = env->GetMaxX();
        double eYMax = env->GetMaxY();

        if ( mSDETests.fuzzyEqual(lXMin,eXMin) && mSDETests.fuzzyEqual(lYMin,eYMin) && mSDETests.fuzzyEqual(lXMax,eXMax) && mSDETests.fuzzyEqual(lYMax,eYMax) ) 
        {
            scName = scReader->GetName();
            bExists = true;
            break;
        }
    }

    scReader = NULL;

    if ( !bExists ) 
    {
        // Create the spatial context (or update it if it already exists):
        FdoPtr<FdoICreateSpatialContext> createSC = (FdoICreateSpatialContext*)conn->CreateCommand(FdoCommandType_CreateSpatialContext);
        createSC->SetName(scName);
        mSDETests.set_extent(createSC, cXMin, cYMin, cXMax, cYMax);
        createSC->SetCoordinateSystem(scCoordSys);
        createSC->SetUpdateExisting(true);
        try 
        {
            createSC->Execute();
        }
        catch (FdoException *ex)
        {
            ex->Release();
        }
    }
}

FdoFeatureClass *TestExpressionFunction::CreateFdoFeatureClass (
                                                    FdoString *class_name,
                                                    bool has_elevation,
                                                    bool has_measure)

// +---------------------------------------------------------------------------
// | The function creates a predefined feature class with the caller identify-
// | ing class name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoFeatureClass                     *the_class                     = NULL;

    FdoDataPropertyDefinition           *data_property_definition      = NULL;

    FdoGeometricPropertyDefinition      *geometric_property_definition = NULL;

    FdoPropertyDefinitionCollection     *data_property_definitions     = NULL;

    FdoDataPropertyDefinitionCollection *id_property_definitions       = NULL;

    try {

      // Create the class and set the abstract property.

	  the_class = FdoFeatureClass::Create(class_name, class_name);
	  the_class->SetIsAbstract(false);

      // Define each of the class properties and add it to the proper collection.
      // If the property identifies the primary key add it to the class' primary
      // key collection.

      data_property_definitions = the_class->GetProperties();
      id_property_definitions   = the_class->GetIdentityProperties();

      data_property_definition =
                CreateDataProperty(L"featid", GetFeatIdType(), 0, 0, false);
      data_property_definition->SetIsAutoGenerated(true);
      data_property_definitions->Add(data_property_definition);
      id_property_definitions->Add(data_property_definition);
      FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"i32_val", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"id", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      geometric_property_definition = CreateGeometricProperty(L"RDBMS_GEOM", has_elevation, has_measure);
      geometric_property_definition->SetSpatialContextAssociation( mSC );
      data_property_definitions->Add(geometric_property_definition);
      the_class->SetGeometryProperty(geometric_property_definition);
	  FDO_SAFE_RELEASE(geometric_property_definition);

      FDO_SAFE_RELEASE(id_property_definitions);
      FDO_SAFE_RELEASE(data_property_definitions);


      the_class->SetBaseClass(NULL);

      // Return a handle to the created class.

      return the_class;

    }  //  try ...

    catch ( ... ) {

	  FDO_SAFE_RELEASE(geometric_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);
      FDO_SAFE_RELEASE(id_property_definitions);
      FDO_SAFE_RELEASE(data_property_definitions);

      throw;

    }  //  catch ...

}  //  CreateFdoFeatureClass ()

// Override because ArcSDE doesn't support all the datatypes.
// Note it doesn't populate the geometry.
void TestExpressionFunction::AddFeature (FdoIConnection * current_connection,
                 FdoString      * class_name,
                 int            index)
// +---------------------------------------------------------------------------
// | The function adds a new object for the specified class. The values being
// | added are predefined based on the predefined schema.
// +---------------------------------------------------------------------------
{
    // Declare and initialize all necessary local variables.

    FdoStringP                 id_str;

    FdoPtr<FdoIInsert>         insert_command;

    FdoDateTime                dt;

	FdoPtr<FdoDataValue>       data_value;

	FdoPtr<FdoPropertyValue>   property_value;

    FdoPtr<FdoIFeatureReader>  feature_reader;

    FdoPtr<FdoPropertyValueCollection> property_values;

    try {

      // Create the FdoIInsert command and set the necessary command properties.

      insert_command = 
            (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);
      insert_command->SetFeatureClassName(GetSchemaName() + L":" + class_name);

      // Get hold of the class property set.

	  property_values = insert_command->GetPropertyValues();

      //// Set the new object's properties.

      data_value     = FdoDataValue::Create(index);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"id");
      property_value->SetValue(data_value);


      data_value     = FdoDataValue::Create((index * 10));
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"i32_val");
      property_value->SetValue(data_value);

      // Execute the command.

      feature_reader = insert_command->Execute();

    }  //  try ...
    catch ( FdoException* e ) {
        throw e;
    }
    catch ( ... ) {

      throw FdoException::Create(L"Failed to add a feature");

    }  //  catch ...
}

FdoStringP TestExpressionFunction::GetSchemaName()
{
    return ArcSDETestConfig::UserNameMetadcov() + L"_" + TestCommonExpressionFunction::GetSchemaName();
}

double TestExpressionFunction::GetNullOrdinate()
{
    return 0;
}

double TestExpressionFunction::GetNaNOrdinate()
{
    return 0;
}

void TestExpressionFunction::TestConcatFunction()
// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CONCAT
// | when used as a select-parameter.
// | NOTE: The default test is overwritten because the SQL Server returns a
// |       different format for the date/time string than other providers.
// |       This has to be reflected in the test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoIFeatureReader*        data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: CONCAT Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type INT32. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "99".

      func_call   = L"(Concat(id, id) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"99");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of properties of               \n");
    printf("  type INT32. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      func_call   = L"(Concat(id, Concat(id, id)) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"999");
      printf(" >>> Test 2.1 succeeded \n");

        // Other tests with variable number of arguments.
      func_call   = L"(Concat(id, id, id, id) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9999");
      printf(" >>> Test 2.2 succeeded \n");

      func_call   = L"(Concat(id, id, id, id, id) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"99999");
      printf(" >>> Test 2.3 succeeded \n");

      func_call   = L"(Concat(id, id, id, id, id, id) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"999999");
      printf(" >>> Test 2.4 succeeded \n");

      func_call   = L"(Concat(id, Concat(id, Concat(id, Concat(id, Concat(id, id))))) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"999999");
      printf(" >>> Test 2.5 succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...
}

void TestExpressionFunction::CheckReaderString (
                                    FdoIFeatureReader *data_reader,
                                    FdoInt32          expected_id_value,
                                    FdoString         *expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool      is_valid_result = false;

    FdoInt32  data_count      = 0,
              id_prop_val;

    FdoString *cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val  = (data_reader->IsNull(L"cmp_id"))
                  ? NULL
                  : data_reader->GetString(L"cmp_id");

      is_valid_result =
        ((id_prop_val == expected_id_value) &&
         (((cmp_id_val == NULL) && (expected_cmp_id_value == NULL)) ||
          ((cmp_id_val != NULL) && (expected_cmp_id_value != NULL) &&
                         (wcscmp(cmp_id_val, expected_cmp_id_value) == 0))));

      // Expect just one row. 
      // Besides for some reason it crashes in ArcSDEReader::ColumnDefinition::ClearCache()
      // at when  delete[] (wchar_t*)mValuePointer;
      break;

    }  //  while (data_reader-> ...

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReaderString ()
