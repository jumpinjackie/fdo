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
    bool error_found = false;

    // Executing the geometry functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Geometry Functions \n");
    printf("\n");

    try {

      TestXYZMFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    if (error_found)
        throw FdoException::Create(L"Unit tests executed with errors");

}

void TestExpressionFunction::setUp ()

// +---------------------------------------------------------------------------
// | The function represents the class setup function.
// +---------------------------------------------------------------------------

{

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
      printf(" >>> Predeleting Test Schema \n");
      DropTestSchema(L"ExprFct");
      printf(" >>> Establishing test environment \n");
      CreateSC();
      SetupUnitTestEnvironment(m_connection);

      printf(" >>> Testing ... \n");
      printf("\n");
      printf(".......................................................... \n");
      printf("\n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Removing test schema \n");
      DropTestSchema(L"ExprFct");
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

      printf(" >>> Removing test schema \n");
      DropTestSchema(L"ExprFct");
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
}  //  setUp ()

void TestExpressionFunction::tearDown ()

// +---------------------------------------------------------------------------
// | The function represents the class tear-down function.
// +---------------------------------------------------------------------------

{

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
    // Need particular extents to prevent coordinate drift for integer coordinates.
    CreateOrUpdateSpatialContext(m_connection, mSC, -35314400, -28428600, 900684611074.099, 900691496874.099, L"26943");
}

void TestExpressionFunction::CreateOrUpdateSpatialContext(FdoIConnection *conn, FdoStringP& scName, double dXMin, double dYMin, double dXMax, double dYMax, FdoString* scCoordSys)
{
    // Determine if the spatial context already exists:
    FdoPtr<FdoIGetSpatialContexts> getSCs = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
    FdoPtr<FdoISpatialContextReader> scReader = getSCs->Execute();
    bool bExists = false;
    bool bIdentical = false;
    while (scReader->ReadNext())
    {
        if (0==wcscmp(scReader->GetName(), scName))
        {
            bExists = true;
            break;
        }
    }

    if ( !bExists ) 
    {
        getSCs = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
        scReader = getSCs->Execute();
        
        while (scReader->ReadNext())
        {
            if (0==wcscmp(scReader->GetCoordinateSystem(), scCoordSys))
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoByteArray> bytes = scReader->GetExtent();
                FdoPtr<FdoIGeometry> geomObj = gf->CreateGeometryFromFgf(bytes);
                FdoPtr<FdoIEnvelope> env = geomObj->GetEnvelope();
                double eXMin = env->GetMinX();
                double eYMin = env->GetMinY();
                double eXMax = env->GetMaxX();
                double eYMax = env->GetMaxY();

                if ( mSDETests.fuzzyEqual(dXMin,eXMin) && mSDETests.fuzzyEqual(dYMin,eYMin) && mSDETests.fuzzyEqual(dXMax,eXMax) && mSDETests.fuzzyEqual(dYMax,eYMax) ) 
                {
                    scName = scReader->GetName();
                    bExists = true;
                    bIdentical = true;
                    break;
                }
            }
        }
    }
    scReader = NULL;

    if ( !bIdentical ) 
    {
        // Create the spatial context (or update it if it already exists):
        FdoPtr<FdoICreateSpatialContext> createSC = (FdoICreateSpatialContext*)conn->CreateCommand(FdoCommandType_CreateSpatialContext);
        createSC->SetName(scName);
        mSDETests.set_extent(createSC, dXMin, dYMin, dXMax, dYMax);
        createSC->SetCoordinateSystem(scCoordSys);
        createSC->SetUpdateExisting(bExists);
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

void TestExpressionFunction::AddFeature (FdoIConnection * /*current_connection*/,
                 FdoString      * /*class_name*/,
                 int            /*index*/)
{
}

double TestExpressionFunction::GetNullOrdinate()
{
    return 0;
}

double TestExpressionFunction::GetNaNOrdinate()
{
    return 0;
}

