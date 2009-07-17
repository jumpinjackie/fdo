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

#include "stdafx.h"
#include "TestCommonExpressionFunctionW.h"
#include <math.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <FdoCommonMiscUtil.h>
#include <limits>       // For quiet_NaN()
using namespace std;
// ----------------------------------------------------------------------------
// --                            Helper functions                            --
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

TestCommonExpressionFunctionW::TestCommonExpressionFunctionW ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  TestCommonExpressionFunctionW ()

TestCommonExpressionFunctionW::~TestCommonExpressionFunctionW ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
    m_connection = NULL;

}  //  ~TestCommonExpressionFunctionW ()

// ===========================================================================
// ==                     TESTING THE GEOMETRY FUNCTIONS                    ==
// ===========================================================================

void TestCommonExpressionFunctionW::TestXYZMFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine functions X, Y, Z
// | and M.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoDouble                       exp_x_values[4][5],
                                    exp_y_values[4][5],
                                    exp_z_values[4][5],
                                    exp_m_values[4][5];

    FdoInt32                        exp_xy_indexes[1],
                                    exp_xy_indexes_all[5],
                                    exp_zm_indexes[1];

    FdoStringP                      func_call;

    FdoPtr<FdoIGeometryCapabilities>     geom_caps;  
    FdoInt32                             dimensionalities;

    FdoDouble                       expNull = IsNullOrdinate(GetNullOrdinate()) ? -1 : GetNullOrdinate();
    FdoDouble                       expNaN = IsNullOrdinate(GetNaNOrdinate()) ? -1 : GetNaNOrdinate();

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: X, Y, Z, M Function Testing      \n");
    printf("========================================================== \n");
    printf("\n");

    // The variables exp_*_values are loaded with the predefined, expected
    // values (-1 represents NULL).

    exp_x_values[0][0] = 201.000000;
    exp_x_values[0][1] = 201.000000;
    exp_x_values[0][2] = 202.000000;
    exp_x_values[0][3] = 202.000000;
    exp_x_values[0][4] = -1;
    exp_x_values[1][0] = 301.000000;
    exp_x_values[1][1] = 301.000000;
    exp_x_values[1][2] = 301.000000;
    exp_x_values[2][0] = 400.000000;
    exp_x_values[2][1] = 400.000000;
    exp_x_values[2][2] = 400.000000;
    exp_x_values[3][0] = 301.000000;
    exp_x_values[3][1] = 301.000000;
    exp_x_values[3][2] = 301.000000;

    exp_y_values[0][0] = 202.000000;
    exp_y_values[0][1] = 201.000000;
    exp_y_values[0][2] = 201.000000;
    exp_y_values[0][3] = 202.000000;
    exp_y_values[0][4] = -1;
    exp_y_values[1][0] = 302.000000;
    exp_y_values[1][1] = 302.000000;
    exp_y_values[1][2] = 302.000000;
    exp_y_values[2][0] = 401.000000;
    exp_y_values[2][1] = 401.000000;
    exp_y_values[2][2] = 401.000000;
    exp_y_values[3][0] = 302.000000;
    exp_y_values[3][1] = 302.000000;
    exp_y_values[3][2] = 302.000000;

    exp_z_values[0][0] = -1;
    exp_z_values[0][1] = -1;
    exp_z_values[0][2] = -1;
    exp_z_values[0][3] = -1;
    exp_z_values[0][4] = -1;
    exp_z_values[1][0] = 303.000000;
    exp_z_values[1][1] = expNull;
    exp_z_values[1][2] = expNaN;
    exp_z_values[2][0] = 402.000000;
    exp_z_values[2][1] = expNull;
    exp_z_values[2][2] = expNaN;
    exp_z_values[3][0] = -1;
    exp_z_values[3][1] = -1;
    exp_z_values[3][2] = -1;

    exp_m_values[0][0] = -1;
    exp_m_values[0][1] = -1;
    exp_m_values[0][2] = -1;
    exp_m_values[0][3] = -1;
    exp_m_values[0][4] = -1;
    exp_m_values[1][0] = -1;
    exp_m_values[1][1] = -1;
    exp_m_values[1][2] = -1;
    exp_m_values[2][0] = PointMAlwaysNull() ? -1 : 403.000000;
    exp_m_values[2][1] = expNull;
    exp_m_values[2][2] = expNaN;
    exp_m_values[3][0] = PointMAlwaysNull() ? -1 : 303.000000;
    exp_m_values[3][1] = expNull;
    exp_m_values[3][2] = expNaN;

    exp_xy_indexes[0] = 2;

    for ( int i = 0; i < 5; i++ ) 
        exp_xy_indexes_all[i] = 0;

    exp_zm_indexes[0] = 0;

    // Execute the various tests.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  Ensure the functions to be tested exist.                 \n");
    printf("---------------------------------------------------------- \n");

    CheckXYZMCapabilities();

    geom_caps = m_connection->GetGeometryCapabilities();
    dimensionalities = geom_caps->GetDimensionalities();

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function X on a geometry value. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      func_call = L"(X(RDBMS_GEOM) as x_value)";
      SelectXYZM( NULL, L"x_value", func_call, exp_x_values );
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
    printf("  function Y on a geometry value. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      func_call = L"(Y(RDBMS_GEOM) as y_value)";
      SelectXYZM( NULL, L"y_value", func_call, exp_y_values );
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
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function Z on a geometry value. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      func_call = L"(Z(RDBMS_GEOM) as z_value)";
      SelectXYZM( NULL, L"z_value", func_call, exp_z_values );
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
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function M on a geometry value. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      func_call = L"(M(RDBMS_GEOM) as m_value)";
      SelectXYZM( NULL, L"m_value", func_call, exp_m_values );
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
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to filter based on    \n");
    printf("  an X and Y value.                                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"X(RDBMS_GEOM) = 202 and Y(RDBMS_GEOM) = 201");
      func_call = L"i32_val";
      SelectXYZMFilter( filter, func_call, XY_POINT_CLASS, 0, 1, exp_xy_indexes );
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
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to filter 2d geometries\n");
    printf("  based on a Z and M value. No features are expected to be  \n");
    printf("  selected.                                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"Z(RDBMS_GEOM) = 202 or M(RDBMS_GEOM) = 201");
      func_call = L"i32_val";
      SelectXYZMFilter( filter, func_call, XY_POINT_CLASS, 0, 0, exp_xy_indexes );
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

//TODO: Add this test when Filters enhanced to test for NULL return value from 
// function.
#if 0
    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to filter 2d geometries\n");
    printf("  with null Z and M values. All features are expected to be \n");
    printf("  selected.                                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"z_value NULL");
      func_call = L"(Z(RDBMS_GEOM) as z_value)";
      SelectXYZMFilter( filter, func_call, XY_POINT_CLASS, 0, 5, exp_xy_indexes_all );
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
#endif

    if ( dimensionalities & FdoDimensionality_Z ) {
        printf("\n");
        printf("---------------------------------------------------------- \n");
        printf("8. Test Case:                                              \n");
        printf("  The test executes a select-command to filter features    \n");
        printf("  by Z value. 1 feature will be selected.                  \n");
        printf("---------------------------------------------------------- \n");

        try {

          FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"Z(RDBMS_GEOM) = 303");
          func_call = L"i32_val";
          SelectXYZMFilter( filter, func_call, XYZ_POINT_CLASS, 1, 1, exp_zm_indexes );
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
        printf("9. Test Case:                                              \n");
        printf("  The test executes a select-command to filter features    \n");
        printf("  by Z value. No features will be selected.                  \n");
        printf("---------------------------------------------------------- \n");

        try {

          FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"Z(RDBMS_GEOM) > 303");
          func_call = L"i32_val";
          SelectXYZMFilter( filter, func_call, XYZ_POINT_CLASS, 1, 0, exp_zm_indexes );
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
    }

    if ( dimensionalities & FdoDimensionality_Z ) {
        printf("---------------------------------------------------------- \n");
        printf("10. Test Case:                                              \n");
        printf("  The test executes a select-command to filter features    \n");
        printf("  by M value. 1 feature will be selected.                  \n");
        printf("---------------------------------------------------------- \n");

        try {

          FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"M(RDBMS_GEOM) = 303");
          func_call = L"i32_val";
          SelectXYZMFilter( filter, func_call, XYM_POINT_CLASS, 3, PointMAlwaysNull() ? 0 : 1, exp_zm_indexes );
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
        printf("11. Test Case:                                              \n");
        printf("  The test executes a select-command to filter features    \n");
        printf("  by Z value. No features will be selected.                  \n");
        printf("---------------------------------------------------------- \n");

        try {

          FdoPtr<FdoFilter> filter      = FdoFilter::Parse(L"M(RDBMS_GEOM) < 303");
          func_call = L"i32_val";
          SelectXYZMFilter( filter, func_call, XYZ_POINT_CLASS, 1, 0, exp_zm_indexes );
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
    }
}  //  TestXYZMFunction ()

// ----------------------------------------------------------------------------
// --                    Test Environment Setup Functions                    --
// ----------------------------------------------------------------------------

void TestCommonExpressionFunctionW::AddFeature (
                                        FdoIConnection *current_connection,
                                        FdoString      *class_name,
                                        int            index)

// +---------------------------------------------------------------------------
// | The function adds a new object for the specified class. The values being
// | added are predefined based on the predefined schema.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    double                     coordinate_buffer[7];

    FdoByte                    byte_value;

    FdoFloat                   flt_value;

    FdoDouble                  dbl_value;

    FdoStringP                 id_str;

    FdoBoolean                 bool_value           = false;

    FdoIInsert                 *insert_command      = NULL;

    FdoDateTime                dt;

	FdoDataValue               *data_value          = NULL;

    FdoByteArray               *byte_array          = NULL;

    FdoILineString             *line_str            = NULL;

    FdoGeometryValue           *geometry_value      = NULL;

	FdoPropertyValue           *property_value      = NULL;

    FdoIFeatureReader          *feature_reader      = NULL;

    FdoFgfGeometryFactory      *geometry_factory    = NULL;

    FdoPropertyValueCollection *property_values     = NULL;

    try {

      // Create the FdoIInsert command and set the necessary command properties.

      insert_command = 
            (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);
      insert_command->SetFeatureClassName(class_name);

      // Get hold of the class property set.

	  property_values = insert_command->GetPropertyValues();

      // Add the geometry information for the new object.

      coordinate_buffer[0] = 100.0 + index;
      coordinate_buffer[1] = 100.0 + index;
      coordinate_buffer[2] = 0;
      coordinate_buffer[3] = 101.0 + index;
      coordinate_buffer[4] = 101.0 + index;
      coordinate_buffer[5] = 0;

      geometry_factory = FdoFgfGeometryFactory::GetInstance();
      line_str         = geometry_factory->CreateLineString(
                                    FdoDimensionality_XY|FdoDimensionality_Z,
                                    6, 
                                    coordinate_buffer);
      byte_array       = geometry_factory->GetFgf(line_str);
      geometry_value   = FdoGeometryValue::Create(byte_array);

      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
      property_value->SetValue(geometry_value);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(property_value);

      // Set the new object's properties.

      data_value     = FdoDataValue::Create(index);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"id");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      bool_value     = ((index % 2) == 0);
      data_value     = FdoDataValue::Create(bool_value);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"bool_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      byte_value     = (index % 10) + 65;
      data_value     = FdoDataValue::Create(byte_value);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"byte_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      if (index != 21) {

          dt.year        = 2007;
          dt.month       = 9;
          dt.day         = ((index % 30) == 0) ? 1 : (index % 30);
          dt.hour        = 21;
          dt.minute      = 0;
          dt.seconds     = 0;
          data_value     = FdoDataValue::Create(dt);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"dt_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 21) ...

      if (index != 22) {

          dt.year        = 2008;
          dt.month       = 5;
          dt.day         = ((index % 31) == 0) ? 1 : (index % 31);
          dt.hour        = 21;
          dt.minute      = 0;
          dt.seconds     = 0;
          data_value     = FdoDataValue::Create(dt);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"dt2_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 22) ...

      if (index != 21) {

          dbl_value      = index + (3.8 * index/8.9);
          data_value     = FdoDataValue::Create(dbl_value, FdoDataType_Decimal);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"dcl_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 21) ...

      if (index != 22) {

          switch (index) {

            case 17: dbl_value = 30.36776;                    break;
            case 20: dbl_value = 30.3;                        break;
            default: dbl_value = index + (6.34 * index/5.77); break;

          }  //  switch ...

          data_value     = FdoDataValue::Create(dbl_value, FdoDataType_Double);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"dbl_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 22) ...

      data_value     = FdoDataValue::Create((index * 4));
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"i16_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      data_value     = FdoDataValue::Create((index * 10));
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"i32_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      switch (index) {

        case 17: flt_value = (FdoFloat) 30.36776;              break;
        case 20: flt_value = (FdoFloat) 30.3;                  break;
        default: flt_value = (FdoFloat) (2.4 * (index /6.99)); break;

      }  //  switch ....

      data_value     = FdoDataValue::Create(flt_value);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"sgl_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      if (index == 20) {

          data_value     = FdoDataValue::Create(L"30.3");
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"str_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index == 20) ...
      else {

        id_str         = FdoStringP::Format(L"  %d  ", index);
        data_value     = FdoDataValue::Create((FdoString *)id_str);
        property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"str_val");
        property_value->SetValue(data_value);
        FDO_SAFE_RELEASE(data_value);
        FDO_SAFE_RELEASE(property_value);

      }  //  else ...

      id_str         = FdoStringP::Format(L"The Color is: %d", (index + 2109));
      data_value     = FdoDataValue::Create((FdoString *)id_str);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"str2_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      id_str         = FdoStringP::Format(L"9/20/2007");
      data_value     = FdoDataValue::Create((FdoString *)id_str);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"dtstr_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      // Execute the command.

      feature_reader = insert_command->Execute();

      // Clean up.

      FDO_SAFE_RELEASE(feature_reader);
      FDO_SAFE_RELEASE(property_values);
      FDO_SAFE_RELEASE(insert_command);

      return;
    
    }  //  try ...

    catch ( ... ) {

      FDO_SAFE_RELEASE(feature_reader);
      FDO_SAFE_RELEASE(property_value);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_values);
      FDO_SAFE_RELEASE(insert_command);
      throw FdoException::Create(L"Failed to add a feature");

    }  //  catch ...

}  //  AddFeature ()

void TestCommonExpressionFunctionW::AddXYZMFeature (
                                        FdoIConnection *current_connection,
                                        FdoString*     /* class_name */)

// +---------------------------------------------------------------------------
// | The function adds a new object for the specified class. The values being
// | added are predefined based on the predefined schema.
// +---------------------------------------------------------------------------

{
   // Declare and initialize all necessary local variables.

    double                     coordinate_2D_buffer[4][2];
	double                     coordinate_3D_buffer[4];
	double                     coordinate_3D_null_buffer[4];
	double                     coordinate_3D_nan_buffer[4];
	double                     coordinate_4D_buffer[5];
	double                     coordinate_4D_null_buffer[5];
	double                     coordinate_4D_nan_buffer[5];
	double                     coordinate_line_buffer[7];
    FdoIInsert                 *insert_command      = NULL;
    FdoILineString             *line_str            = NULL;
	FdoIPoint				   *point               = NULL;
    FdoGeometryValue           *geometry_value      = NULL;
	FdoPropertyValue           *property_value      = NULL;
    FdoFgfGeometryFactory      *geometry_factory    = NULL;
    FdoPropertyValueCollection *property_values     = NULL;
    FdoByteArray               *byte_array          = NULL;
	FdoIFeatureReader          *feature_reader      = NULL;
    FdoIGeometryCapabilities   *geom_caps           = NULL;  
    FdoInt32                    dimensionalities;

    try {

      // Find out supported dimensionalities

      geom_caps = current_connection->GetGeometryCapabilities();
      dimensionalities = geom_caps->GetDimensionalities();

      // Create the FdoIInsert command and set the necessary command properties.

      insert_command = 
            (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);
      insert_command->SetFeatureClassName(XY_POINT_CLASS);

      // Get hold of the class property set.

	  property_values = insert_command->GetPropertyValues();

      // Add the geometry information for the new object.
      coordinate_2D_buffer[0][0] = 201.0;
      coordinate_2D_buffer[0][1] = 202.0;
      coordinate_2D_buffer[1][0] = 201.0;
      coordinate_2D_buffer[1][1] = 201.0;
      coordinate_2D_buffer[2][0] = 202.0;
      coordinate_2D_buffer[2][1] = 201.0;
      coordinate_2D_buffer[3][0] = 202.0;
      coordinate_2D_buffer[3][1] = 202.0;
	  
	  coordinate_3D_buffer[0] = 301.0;
      coordinate_3D_buffer[1] = 302.0;
      coordinate_3D_buffer[2] = 303.0;

	  coordinate_3D_null_buffer[0] = 301.0;
      coordinate_3D_null_buffer[1] = 302.0;
      coordinate_3D_null_buffer[2] = GetNullOrdinate();

	  coordinate_3D_nan_buffer[0] = 301.0;
      coordinate_3D_nan_buffer[1] = 302.0;
      coordinate_3D_nan_buffer[2] = GetNaNOrdinate();

	  coordinate_4D_buffer[0] = 400.0;
      coordinate_4D_buffer[1] = 401.0;
      coordinate_4D_buffer[2] = 402.0;
      coordinate_4D_buffer[3] = 403.0;

	  coordinate_4D_null_buffer[0] = 400.0;
      coordinate_4D_null_buffer[1] = 401.0;
      coordinate_4D_null_buffer[2] = GetNullOrdinate();
      coordinate_4D_null_buffer[3] = GetNullOrdinate();

	  coordinate_4D_nan_buffer[0] = 400.0;
      coordinate_4D_nan_buffer[1] = 401.0;
      coordinate_4D_nan_buffer[2] = GetNaNOrdinate();
      coordinate_4D_nan_buffer[3] = GetNaNOrdinate();

	  coordinate_line_buffer[0] = 301.0;
      coordinate_line_buffer[1] = 302.0;
	  coordinate_line_buffer[2] = 311.0;
      coordinate_line_buffer[3] = 312.0;

      geometry_factory = FdoFgfGeometryFactory::GetInstance();

      int idx = 0;
      for ( idx = 0; idx < 4; idx++ ) {
          // Add a point XY geometry.

          point = geometry_factory->CreatePoint(FdoDimensionality_XY,
                                                coordinate_2D_buffer[idx]);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 0, idx, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);
      }

      // Add a non-point (line) geometry.

      line_str = geometry_factory->CreateLineString(
                                    FdoDimensionality_XY,
                                    4, 
                                    coordinate_line_buffer);
      byte_array = geometry_factory->GetFgf(line_str);
      geometry_value = FdoGeometryValue::Create(byte_array);
      property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
      property_value->SetValue(geometry_value);

	  feature_reader = insert_command->Execute();
      feature_reader->ReadNext();
      SetXYZMIndex( 0, idx, GetFeatId(feature_reader) );
      FDO_SAFE_RELEASE(feature_reader);

      if ( dimensionalities & FdoDimensionality_Z ) {
          insert_command = 
                (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);

          insert_command->SetFeatureClassName(XYZ_POINT_CLASS);

          // Get hold of the class property set.

	      property_values = insert_command->GetPropertyValues();

          // Add a point XYZ geometry.

          point = geometry_factory->CreatePoint(
                                        FdoDimensionality_XY | FdoDimensionality_Z,
                                        coordinate_3D_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 1, 0, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                                        FdoDimensionality_XY | FdoDimensionality_Z,
                                        coordinate_3D_null_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 1, 1, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                                        FdoDimensionality_XY | FdoDimensionality_Z,
                                        coordinate_3D_nan_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 1, 2, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);
      }

	  // Add a point XYZM geometry.

      if ( (dimensionalities & FdoDimensionality_Z) && (dimensionalities & FdoDimensionality_M) ) {
          insert_command = 
                (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);

          insert_command->SetFeatureClassName(XYZM_POINT_CLASS);

          // Get hold of the class property set.

	      property_values = insert_command->GetPropertyValues();

          point = geometry_factory->CreatePoint(
                   FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M,
                   coordinate_4D_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 2, 0, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                   FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M,
                   coordinate_4D_null_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 2, 1, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                   FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M,
                   coordinate_4D_nan_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 2, 2, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);
      }

	  // Add a point XYM geometry.

      if ( dimensionalities & FdoDimensionality_M ) {
          insert_command = 
                (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);

          insert_command->SetFeatureClassName(XYM_POINT_CLASS);

          // Get hold of the class property set.

	      property_values = insert_command->GetPropertyValues();

          point = geometry_factory->CreatePoint(
                                    FdoDimensionality_XY |  FdoDimensionality_M,
                                    coordinate_3D_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 3, 0, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                                    FdoDimensionality_XY |  FdoDimensionality_M,
                                    coordinate_3D_null_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 3, 1, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);

          point = geometry_factory->CreatePoint(
                                    FdoDimensionality_XY |  FdoDimensionality_M,
                                    coordinate_3D_nan_buffer);
          byte_array = geometry_factory->GetFgf(point);
          geometry_value = FdoGeometryValue::Create(byte_array);
          property_value = TestCommonMiscUtil::AddNewProperty(property_values, L"RDBMS_GEOM");
          property_value->SetValue(geometry_value);
	      feature_reader = insert_command->Execute();
          feature_reader->ReadNext();
          SetXYZMIndex( 3, 2, GetFeatId(feature_reader) );
          FDO_SAFE_RELEASE(feature_reader);
      }

	  // Clean up.

	  FDO_SAFE_RELEASE(point);
	  FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
	  FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(property_value);
      FDO_SAFE_RELEASE(feature_reader);
	  FDO_SAFE_RELEASE(insert_command);
	  FDO_SAFE_RELEASE(geom_caps);

	}  //  try ...

    catch ( FdoException* ) {
        throw;
    }
    catch ( ... ) {

      throw FdoException::Create(L"Failed to add XYZM feature");

    }  //  catch ...

} // AddXYZMFeature()

void TestCommonExpressionFunctionW::AddTestSchema (
                                        FdoIConnection *current_connection,
                                        FdoString      *schema_name)

// +---------------------------------------------------------------------------
// | The function establishes the test schema in the FDO data store the pro-
// | vided connection identifies.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoClass                           *schema_class         = NULL;

    FdoFeatureClass                    *schema_feature_class = NULL;

	FdoIApplySchema                    *apply_schema_cmd     = NULL;

    FdoClassCollection                 *classes              = NULL;

    FdoPtr<FdoFeatureSchemaCollection> schemas;

	FdoFeatureSchema                   *schema               = NULL;

    FdoIGeometryCapabilities           *geom_caps           = NULL;  
    FdoInt32                            dimensionalities;

    try {

      // Find out supported dimensionalities

      geom_caps = current_connection->GetGeometryCapabilities();
      dimensionalities = geom_caps->GetDimensionalities();

      // Create a apply-schema command.

      apply_schema_cmd = 
        (FdoIApplySchema*) current_connection->CreateCommand(
                                                   FdoCommandType_ApplySchema);

      // Create the feature schema collection.

      schemas = FdoFeatureSchemaCollection::Create(NULL);

      // Create the lock test schema.

	  schema  = FdoFeatureSchema::Create(schema_name,
                                         L"Expression Function Test Schema");
      classes = schema->GetClasses();

      // Create the test classes for the unit test and add them to the schema.

      printf(" >>> ...... adding feature classes \n");
      printf(" >>> ......... adding class exfct_c1 \n");
      schema_feature_class = CreateFdoFeatureClass(L"exfct_c1");
      classes->Add(schema_feature_class);
      FDO_SAFE_RELEASE(schema_feature_class);

	  // Create class with XYZM geometry

      printf(" >>> ......... adding class xy_point \n");
      schema_feature_class = CreateFdoFeatureClass(XY_POINT_CLASS, false, false);
      classes->Add(schema_feature_class);
      FDO_SAFE_RELEASE(schema_feature_class);

      if ( dimensionalities & FdoDimensionality_Z ) {
          printf(" >>> ......... adding class xyz_point \n");
          schema_feature_class = CreateFdoFeatureClass(XYZ_POINT_CLASS, true, false);
          classes->Add(schema_feature_class);
          FDO_SAFE_RELEASE(schema_feature_class);
      }

      if ( dimensionalities & FdoDimensionality_M ) {
          printf(" >>> ......... adding class xym_point \n");
          schema_feature_class = CreateFdoFeatureClass(XYM_POINT_CLASS, false, true);
          classes->Add(schema_feature_class);
          FDO_SAFE_RELEASE(schema_feature_class);
      }

      if ( (dimensionalities & FdoDimensionality_Z) && (dimensionalities & FdoDimensionality_M) ) {
          printf(" >>> ......... adding class xyzm_point \n");
          schema_feature_class = CreateFdoFeatureClass(XYZM_POINT_CLASS, true, true);
          classes->Add(schema_feature_class);
          FDO_SAFE_RELEASE(schema_feature_class);
      }

      printf(" >>> ...... adding non-feature classes \n");
      printf(" >>> ......... adding class exfct_c2 \n");
      schema_class = CreateFdoClass(L"exfct_c2");
      if ( schema_class ) 
          classes->Add(schema_class);
      FDO_SAFE_RELEASE(schema_class);

      // Add the test schema to the schema collection.

      schemas->Add(schema);

      // Set the active schema and create it.

      printf(" >>> ...... creating the schema \n");
      apply_schema_cmd->SetFeatureSchema(schema);
      apply_schema_cmd->Execute();

      // Release the memory.

      FDO_SAFE_RELEASE(classes);
      FDO_SAFE_RELEASE(schema);
      FDO_SAFE_RELEASE(apply_schema_cmd);
      FDO_SAFE_RELEASE(geom_caps);

    }  //  try ...

    catch ( ... ) {

      FDO_SAFE_RELEASE(classes);
      FDO_SAFE_RELEASE(schema);
      FDO_SAFE_RELEASE(apply_schema_cmd);
      FDO_SAFE_RELEASE(geom_caps);

      throw;

    }  //  catch ...

}  //  AddTestSchema ()


FdoDataPropertyDefinition *TestCommonExpressionFunctionW::CreateDataProperty (
                                                  FdoString     *property_name,
                                                  FdoDataType   data_type,
                                                  FdoInt32      data_size,
                                                  FdoInt32      scale,
                                                  bool          is_nullable)

// +---------------------------------------------------------------------------
// | The function creates a data property object based on the provided data and
// | returns it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDataPropertyDefinition *data_property_definition = NULL;

    // Create the property object and initialize it using the given data.

	data_property_definition =
                            FdoDataPropertyDefinition::Create(property_name,
                                                              property_name);
	data_property_definition->SetDataType(data_type);

    if (data_type == FdoDataType_String)
        data_property_definition->SetLength(data_size);

    if (data_type == FdoDataType_Decimal) {

        data_property_definition->SetPrecision(data_size);
        data_property_definition->SetScale(scale);

    }  //  if (data_type == FdoDataType_Decimal) ...

	data_property_definition->SetNullable(is_nullable);

    // Return the handle on the created object.

    return data_property_definition;

}  //  CreateDataProperty ()

FdoFeatureClass *TestCommonExpressionFunctionW::CreateFdoFeatureClass (
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
           CreateDataProperty(L"bool_val", FdoDataType_Boolean, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"byte_val", FdoDataType_Byte, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dt_val", FdoDataType_DateTime, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dt2_val", FdoDataType_DateTime, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dcl_val", FdoDataType_Decimal, 8, 2, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dbl_val", FdoDataType_Double, 10, 5, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"i16_val", FdoDataType_Int16, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"i32_val", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"sgl_val", FdoDataType_Single, 10, 3, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"str_val", FdoDataType_String, 30, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"str2_val", FdoDataType_String, 30, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dtstr_val", FdoDataType_String, 30, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"id", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      geometric_property_definition = CreateGeometricProperty(L"RDBMS_GEOM", has_elevation, has_measure);
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

FdoClass* TestCommonExpressionFunctionW::CreateFdoClass (FdoString* /*class_name*/)
{
    return (FdoClass*) NULL;
}

FdoGeometricPropertyDefinition
                *TestCommonExpressionFunctionW::CreateGeometricProperty (
                                                    FdoString *property_name,
                                                    bool has_elevation,
                                                    bool has_measure)

// +---------------------------------------------------------------------------
// | The function creates the named geometry property and returns it back to
// | the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoGeometricPropertyDefinition *geometric_property_definition = NULL;

    // Create the property object and initialize it using the given data.

	geometric_property_definition = 
                        FdoGeometricPropertyDefinition::Create(property_name,
                                                               property_name);
    geometric_property_definition->SetGeometryTypes(
                                                FdoGeometricType_Point   | 
                                                FdoGeometricType_Curve   | 
                                                FdoGeometricType_Surface | 
                                                FdoGeometricType_Solid    );
    geometric_property_definition->SetHasElevation(has_elevation);
    geometric_property_definition->SetHasMeasure(has_measure);

    // Return the handle on the created object.

    return geometric_property_definition;

}  //  CreateGeometricProperty ()

void TestCommonExpressionFunctionW::InsertTestFeatures(FdoIConnection* connection)

// +---------------------------------------------------------------------------
// | The function inserts the test features for the filter optinmizer test
// | cases.
// +---------------------------------------------------------------------------

{

    std::vector<FdoSpatialCondition*> allCond;
    printf("\nInsert features");

    try {

      // Note:
      // EB = envelope Big geometry; ES = envelope Small geometry
      // B = Big geometry; S = Small geometry
      // NI = no intersection
      // TO = touche outside of the geometry
      // TI = touche inside of the geometry

      FdoSpatialCondition *cond = NULL;

      // Define the spatial filters.
      // geom - out EB, out ES - NI.

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5536366.97310883 1964015.95271029, 5618464.31544299 1927535.52167126, 5866059.47432348 1950987.22770124, 5536366.97310883 1964015.95271029))')"));
      allCond.push_back(cond);

      // geom - in EB, out ES - NI.

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5527679.42371172 2264545.2193847, 5527679.42371172 2220681.84366703, 5594573.55513252 2220681.84366703, 5565035.88505614 2263242.34739065, 5527679.42371172 2264545.2193847))')"));
      allCond.push_back(cond);

      // geom - in EB, in ES - NI.

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5672117.79766474 2133761.8362255, 5639252.139427 2037628.58952983, 5719107.54066284 2007485.11266832, 5672117.79766474 2133761.8362255))')"));
      allCond.push_back(cond);

      // geom - in EB, out ES - TO.B - NI.

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5816766.1271512 2262328.88622243, 5735027.92795496 2179902.49038579, 5803867.44957874 2179902.49038579, 5816766.1271512 2262328.88622243))')"));
      allCond.push_back(cond);

      // geom - in EB, in ES - TO.B - NI.

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5816766.1271512 2262328.88622243, 5735027.92795496 2179902.49038579, 5838419.22614045 2165830.30743391, 5816766.1271512 2262328.88622243))')"));
      allCond.push_back(cond);

      // geom - in EB, out ES - TI.B - NI

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5534910.27144644 2174970.99624234, 5629331.98126834 2207377.9551015, 5650470.1478844 2182389.96947706, 5534910.27144644 2174970.99624234))')"));
      allCond.push_back(cond);

      // geom - in B, out ES - NI

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2225006.95144691, 5662882.8202307 2203553.48628294, 5704168.60432706 2199480.04451405, 5721823.71187219 2215773.81496862, 5695850.38275576 2225006.95144691))')"));
      allCond.push_back(cond);

      // geom - in B, in ES - NI

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5715848.13648738 2173410.01111093, 5721552.09326288 2167164.06515053, 5688958.05363445 2161189.68411475, 5679994.69055723 2176125.64008319, 5715848.13648738 2173410.01111093))')"));
      allCond.push_back(cond);

      // geom - intersect B, out ES

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5545488.90050734 2131652.22023764, 5620636.20332382 2115149.16822162, 5491626.09211908 2080405.89984133, 5545488.90050734 2131652.22023764))')"));
      allCond.push_back(cond);

      // geom - intersect B, intersect ES, TO.S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2116555.74670371, 5662970.07317681 1975615.54612141, 5767354.11879699 2012972.41972645, 5695850.38275576 2116555.74670371))')"));
      allCond.push_back(cond);

      // geom - intersect B, intersect S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5757465.1057966 2064771.42915467, 5902547.1817913 2086051.68186123, 5932084.85186769 1996587.76607765, 5757465.1057966 2064771.42915467))')"));
      allCond.push_back(cond);

      // geom - intersect B, intersect ES

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5730821.11604607 1991114.52578425, 5802483.85746115 2029157.47939835, 5848988.4522226 1971388.55089996, 5730821.11604607 1991114.52578425))')"));
      allCond.push_back(cond);

      // geom - in B, intersect S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5643240.8518349 2210213.75303545, 5704542.36311135 2133423.34857475, 5655219.30873943 2241211.71405062, 5643240.8518349 2210213.75303545))')"));
      allCond.push_back(cond);

      // geom - in B, TO.S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5774147.82229251 2166139.89621237, 5708065.43711587 2182738.28663024, 5695850.38275576 2160706.01616614, 5774147.82229251 2166139.89621237))')"));
      allCond.push_back(cond);

      // geom - in B, in S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5734318.09348169 2143809.66071967, 5721552.09326288 2106605.55157138, 5757133.92270997 2096286.16384209, 5734318.09348169 2143809.66071967))')"));
      allCond.push_back(cond);

      // geom - in B, TI.S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2160706.01616614, 5730821.11604607 2149221.37454525, 5774147.82229251 2166139.89621237, 5695850.38275576 2160706.01616614))')"));
      allCond.push_back(cond);

      // geom - contains B & S - NI

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5485357.60096315 2371545.41859256 0, 5906012.79496403 2371545.41859256 0, 6072002.10683594 2267376.24251062 0, 6051163.49846932 1854443.06455299 0, 5611483.69752058 1831194.59505873 0, 5443080.69663456 1955186.43911943 0, 5443080.69663456 2344774.45279884 0, 5485357.60096315 2371545.41859256 0))')"));
      allCond.push_back(cond);

      // geom - contains B & S, TO.B

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5629331.98126834 2207377.9551015, 5534910.27144644 2174970.99624234, 5440966.85141813 2092563.76776649, 5484652.98453481 1905871.50106395, 5803138.99578732 1854443.06455299, 6069658.83183594 2017573.62051062, 6033548.12234384 2310958.4944062, 5865145.12145782 2410997.37005064, 5481834.52492434 2397611.88546429, 5502268.36269459 2287005.52430045, 5629331.98126834 2207377.9551015))')"));
      allCond.push_back(cond);

      // geom - contains S, intersect B

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5541726.80537841 2303913.50349677 0, 5700265.19661062 2329275.4714465 0, 6030025.04630503 2329275.4714465 0, 5958858.92537476 1917143.48719483 0, 5726335.95360182 1836126.08751268 0, 5468446.83923171 1880509.53311422 0, 5457877.61314957 2272915.54248159 0, 5541726.80537841 2303913.50349677 0))')"));
      allCond.push_back(cond);

      // geom - contains B & S, intersect EB

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5529748.35050817 2396202.8876203 0, 5708720.57747633 2303209.00457478 0, 5951108.15890309 2283483.02800098 0, 5962382.00141358 1967162.92417231 0, 5820754.3719128 1864306.05283989 0, 5496631.44143934 1907280.49890794 0, 5444489.92745695 2054520.81584189 0, 5461400.68918838 2369431.92182658 0, 5529748.35050817 2396202.8876203 0))')"));
      allCond.push_back(cond);

      // geom - contains B & S, intersect EB & ES

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5462105.30358243 2417337.86203808 0, 5462105.30358243 1855852.06408648 0, 5956745.08015833 1869237.54698334 0, 5798911.30535446 2084814.27793507 0, 6083575.79110981 2318707.98423761 0, 5593163.70496676 2458903.30857264 0, 5462105.30358243 2417337.86203808 0))')"));
      allCond.push_back(cond);

      // geom - intersect B & S

      cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5645305.22098345 2472288.793159 0, 5830618.98360091 1834012.59074671 0, 6170948.05937746 2470879.79362552 0, 5645305.22098345 2472288.793159 0))')"));
      allCond.push_back(cond);

      FdoPtr<FdoIInsert>insertCommand =
            (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
      insertCommand->SetFeatureClassName(L"bayarea_county");

      FdoPtr<FdoPropertyValueCollection> propertyValues;
      FdoPtr<FdoPropertyValue> propertyValue;
      FdoPtr<FdoIFeatureReader> reader;
      for (size_t i = 0; i < allCond.size(); i++) {

        FdoSpatialCondition *spCond = allCond.at(i);
        FdoPtr<FdoGeometryValue> geomVal =
                        static_cast<FdoGeometryValue*>(spCond->GetGeometry());
        propertyValues = insertCommand->GetPropertyValues();
            
        propertyValue = propertyValues->FindItem( L"SHPGEOM" );
        if (propertyValue == NULL) {

            propertyValue =  FdoPropertyValue::Create();
            propertyValue->SetName( L"SHPGEOM" );
            propertyValues->Add( propertyValue );

        }  //  if (propertyValue == NULL) ...
            
        propertyValue->SetValue(geomVal);
        reader = insertCommand->Execute();
        if (reader != NULL)
            reader->Close();

      }  //  for (size_t i = 0; ...

    }  //  try ...

    catch(FdoException* exc) {

      printf ("\nException: %ls", exc->GetExceptionMessage());
      exc->Release();

    }  //  catch ... 

    for (size_t i = 0; i < allCond.size(); i++) {

      FdoSpatialCondition* ptr = allCond.at(i);
      FDO_SAFE_RELEASE(ptr);

    }  //  for (size_t i = 0; ...

    printf("...Ready");

}  //  InsertTestFeatures ()

void TestCommonExpressionFunctionW::SetupUnitTestEnvironment (
                                        FdoIConnection *current_connection)

// +---------------------------------------------------------------------------
// | The function controls the establishing of the test environment in the FDO
// | data store the provided connection identifies.
// +---------------------------------------------------------------------------

{

    // Add the test schema and some data.

    try {

      // Add the test schema.

      printf(" >>> ... adding test schema \n");
      AddTestSchema(current_connection, L"ExprFct");

      // Load the features used in the unit tests.
      //   > The spatial features.
      printf(" >>> ... adding data for feature classes \n");
      printf(" >>> ...... for class exfct_c1 \n");
      for (int i = 0; i < 31; i++)
        AddFeature(current_connection, L"exfct_c1", i);

      printf(" >>> ...... for class %ls \n", XYZM_POINT_CLASS);
	  AddXYZMFeature(current_connection, XYZM_POINT_CLASS); 	

   }  //  try ...

   catch (FdoException *exp) {

     printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
     throw exp;

   }  //  catch ...

   catch ( ... ) {

     throw;

   }  //  catch ...

}  //  SetupUnitTestEnvironment ()



